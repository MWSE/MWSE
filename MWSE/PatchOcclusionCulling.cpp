#include "PatchOcclusionCulling.h"

#include "Log.h"
#include "MemoryUtil.h"
#include "MWSEConfig.h"

#include "TES3DataHandler.h"
#include "TES3WorldController.h"

#include "NIAVObject.h"
#include "NICamera.h"
#include "NIDefines.h"
#include "NIGeometryData.h"
#include "NINode.h"
#include "NITArray.h"
#include "NITransform.h"
#include "NITriShape.h"
#include "NITriShapeData.h"

#include "external/msoc/MaskedOcclusionCulling.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <ostream>
#include <vector>

namespace mwse::patch::occlusion {

	// MSOC tile-buffer resolution. Decoupled from the game viewport: the
	// rasterizer works in NDC via the camera's world-to-clip matrix, then maps
	// NDC to this framebuffer via its own internal scale/offset.
	constexpr unsigned int kMsocWidth = 512;
	constexpr unsigned int kMsocHeight = 256;

	// Clip-space w floor. Intel MSOC treats this as the near plane. In
	// Morrowind units (1 unit ~= 1.4 cm) 1.0 is a safe floor below the
	// engine's own near plane and well above the numerical noise that
	// explodes NDC after perspective divide.
	constexpr float kNearClipW = 1.0f;

	// Lower bound on occluder world-bound radius. Shapes smaller than this are
	// still visibility-tested but never used as occluders; they don't produce
	// enough tile coverage to be worth the rasterisation cost.
	constexpr float kOccluderRadiusMin = 256.0f;

	// Upper bound on occluder world-bound radius. A Morrowind cell spans
	// 8192 world units and a typical large canton building sits around
	// 1500; anything larger than this threshold is almost certainly a
	// massive composite mesh (terrain-like prop, skybox geometry, a giant
	// cave volume) whose AABB spans the frustum and whose per-frame
	// vertex transform is expensive. Those aren't useful occluders: their
	// near face rasterises close to the camera so they over-occlude, and
	// their triangle counts dominate frame cost. Still visibility-tested,
	// just never rasterised.
	constexpr float kOccluderRadiusMax = 4096.0f;

	// Safety margin for the inside-occluder guard. Rasterizing an object whose
	// bounding sphere contains the camera is pathological: its AABB spans the
	// entire screen and falsely occludes everything behind. A small margin
	// guards against near-miss cases (camera just outside but intersecting).
	constexpr float kInsideOccluderMargin = 64.0f;

	// Minimum AABB extent (world units). A shape is rejected when *two or more*
	// axes fall below this — i.e. it's pencil-shaped (column, sign, banner)
	// and its cube approximation would falsely occlude things visible through
	// the gaps. Walls and floors (one thin axis, two large) still qualify.
	constexpr float kOccluderMinDimension = 128.0f;

	// Depth slack applied to testee sphere wMin. Sphere-center projection
	// gives wMin = c.w - r*g_wGradMag, which is the *exact* closest-w of the
	// bounding sphere — tighter than a corner-based AABB wMin. That tightness
	// plus single-pass "rasterize-then-test" traversal means a near-coplanar
	// occluder (wall, ground) rasterised just before us can fill our rect at a
	// 1/w effectively equal to our own, and TestRect's ties-go-to-OCCLUDED
	// rule hides us (doors flush to walls, shapes flush to terrain, leaves on
	// branches). Expand the sphere's effective radius by this factor for the
	// depth test *only* (the NDC rect keeps the true radius so we don't widen
	// the tile footprint). A 10% slack is ~4x larger than float-precision tie
	// bands and still small enough that genuinely-behind-a-wall meshes still
	// cull.
	constexpr float kDepthSlackFactor = 1.10f;

	// Intel MSOC is allocated on the heap via Create()/Destroy(). We leak
	// this on process exit (same pattern as other long-lived MWSE globals).
	static ::MaskedOcclusionCulling* g_msoc = nullptr;

	// World-to-clip matrix, transposed from NI's row-major M*v layout into
	// Intel's column-major v*M layout (consecutive memory = one column).
	// Intel reads clip.x = v.x*m[0] + v.y*m[4] + v.z*m[8] + m[12].
	// Refreshed each frame at the top-level entry of CullShow_detour.
	static float g_worldToClip[16];

	// Reusable buffers for rasterizeTriShape. Grown on demand, never shrunk;
	// MSOC is invoked single-threaded from the worldCamera pass so a single
	// module-level buffer is sufficient.
	static std::vector<float> g_occluderVerts;
	static std::vector<unsigned int> g_occluderIndices;

	// Per-frame matrix metrics used by testSphereVisible.
	// g_ndcRadiusX/Y: L2 norm of the world->clip.x / clip.y coefficients, so
	// the NDC half-extent of a sphere of radius r at clip-w cw is
	// r * g_ndcRadiusX / cw (resp. Y). Reads the leading 3 entries of each
	// row; the trailing entry is translation and doesn't contribute to the
	// gradient.
	// g_wGradMag: L2 norm of clip.w coefficients, so the worst-case clip-w
	// offset from the sphere center to its near surface is r * g_wGradMag.
	// For a standard perspective projection after a pure-rotation world->view
	// this is 1.0; computing it from the matrix handles any scaling.
	static float g_ndcRadiusX = 0.0f;
	static float g_ndcRadiusY = 0.0f;
	static float g_wGradMag = 0.0f;

	// True only while TES3Game_static::renderMainScene (0x41C400) is on the
	// stack. Gates MSOC activation so every Click tree that fires outside
	// the per-frame main scene — load-screen splash, offscreen UI targets,
	// chargen race preview, MGE water reflection weather passes, etc. (see
	// rendering-engine-notes §1.6) — runs the pure engine-equivalent path.
	// Those callers wield cameras with setups we haven't validated for
	// occlusion culling, and the main-scene frame is the only place where
	// MSOC's cost is paid back.
	static bool g_inRenderMainScene = false;

	// True only while the scene graph is being traversed for the worldCamera
	// main pass. CullShow_detour gates MSOC work on this so shadow-manager,
	// water-refraction, armCamera and other non-main Clicks inside
	// renderMainScene still run the pure engine-equivalent path.
	static bool g_msocActive = false;

	// Per-frame diagnostics. Reset at the top of each worldCamera traversal.
	static uint64_t g_recursiveCalls = 0;
	static uint64_t g_recursiveAppCulled = 0;
	static uint64_t g_recursiveFrustumCulled = 0;
	static uint64_t g_rasterizedAsOccluder = 0;
	static uint64_t g_skippedInside = 0;
	static uint64_t g_skippedThin = 0;
	static uint64_t g_queryTested = 0;
	static uint64_t g_queryOccluded = 0;
	static uint64_t g_queryViewCulled = 0;
	static uint64_t g_queryNearClip = 0;
	static uint64_t g_deferred = 0;
	// Diagnostic: shapes tested inline with radius below the defer
	// threshold. These SHOULD have been queued; if this is nonzero the
	// defer type gate is rejecting them (unexpected geometry type).
	static uint64_t g_smallInlineTested = 0;

	// Deferred-display queue for small NiTriShape leaves. Small shapes are
	// queued during the main traversal and drained *after* all occluder
	// rasterisation is complete, so their MSOC visibility test runs
	// against the fully-populated depth buffer instead of whatever
	// occluders happened to precede them in scene-graph order. Avoids the
	// OpenMW two-pass cost (single traversal) and still gives big shapes
	// priority for rasterisation.
	//
	// Only NiTriShape leaves are deferred. NiNodes must stay inline so
	// their subtree keeps contributing occluders during the main pass;
	// other leaf types (Particles, etc.) are rare enough to keep inline.
	struct PendingDisplay {
		NI::AVObject* shape;
		NI::Camera* camera;
	};
	static std::vector<PendingDisplay> g_pendingDisplays;

	// Fill g_worldToClip by transposing NI::Camera::worldToCamera into
	// Intel's column-major v*M layout. NI stores row-major M*v — clip[r] =
	// Σ_c ni[r*4+c]*v[c] (verified against NiCamera::ScreenSpaceBoundBound
	// at 0x6CCEC0). Intel's TransformVerts computes out[r] =
	// Σ_c v[c]*mtx[r*4+c] with mtx indexed column-major as mtx[col*4+row],
	// so the effective formula is out[r] = Σ_c v[c]*mtx[c*4+r]. For Intel
	// to produce the engine's clip coords we need mtx[c*4+r] = ni[r*4+c].
	static void uploadCameraTransform(NI::Camera* cam) {
		const float* ni = reinterpret_cast<const float*>(&cam->worldToCamera);
		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; ++col) {
				g_worldToClip[col * 4 + row] = ni[row * 4 + col];
			}
		}

		// Per-frame metrics for sphere projection. g_ndcRadiusX is the
		// operator norm of the x-row coefficients (ignoring translation):
		// an upper bound on |d(x_clip)/d(pos)| used to scale the sphere
		// radius in clip space. Similarly for y and w. After the transpose
		// into Intel's column-major layout, NI's row-r coefficients live at
		// strided positions m[0+r], m[4+r], m[8+r] (column 0..2, row r).
		const float* m = g_worldToClip;
		g_ndcRadiusX = std::sqrt(m[0] * m[0] + m[4] * m[4] + m[8] * m[8]);
		g_ndcRadiusY = std::sqrt(m[1] * m[1] + m[5] * m[5] + m[9] * m[9]);
		g_wGradMag = std::sqrt(m[3] * m[3] + m[7] * m[7] + m[11] * m[11]);
	}

	// Project a world-space point using the cached Intel-layout matrix.
	// Produces clip-space (x, y, w); z is unused by Intel's rasterizer.
	// Uses the same column-major v*M formula as Intel's TransformVerts so
	// sphere screen rects stay aligned with rasterised occluder depth.
	struct ClipXYW {
		float x, y, w;
	};
	static inline ClipXYW projectWorld(float wx, float wy, float wz) {
		const float* m = g_worldToClip;
		return {
			wx * m[0] + wy * m[4] + wz * m[8] + m[12],
			wx * m[1] + wy * m[5] + wz * m[9] + m[13],
			wx * m[3] + wy * m[7] + wz * m[11] + m[15],
		};
	}

	// Direct sphere -> NDC-rect + wmin projection, then Intel's TestRect.
	// Projects only the sphere center and derives the NDC half-extent and
	// near-surface clip-w from per-frame matrix metrics (see
	// uploadCameraTransform). Tighter than an 8-corner world AABB projection
	// by ~sqrt(3) and stable under camera rotation — neither the NDC radius
	// nor wMin depend on how the sphere's bbox aligns with camera axes, so
	// small-mesh queries don't flicker across TestRect's hiZ thresholds.
	//
	// If the sphere's near surface straddles the near plane we bail as
	// VISIBLE: TestRect can't project a straddling rect safely.
	static ::MaskedOcclusionCulling::CullingResult testSphereVisible(
		const TES3::Vector3& center, float radius) {
		const ClipXYW c = projectWorld(center.x, center.y, center.z);

		const float wMin = c.w - radius * g_wGradMag * kDepthSlackFactor;
		if (wMin <= kNearClipW) {
			++g_queryNearClip;
			return ::MaskedOcclusionCulling::VISIBLE;
		}

		const float invW = 1.0f / c.w;
		const float cxNdc = c.x * invW;
		const float cyNdc = c.y * invW;
		const float rxNdc = radius * g_ndcRadiusX * invW;
		const float ryNdc = radius * g_ndcRadiusY * invW;

		float ndcMinX = std::max(cxNdc - rxNdc, -1.0f);
		float ndcMinY = std::max(cyNdc - ryNdc, -1.0f);
		float ndcMaxX = std::min(cxNdc + rxNdc, 1.0f);
		float ndcMaxY = std::min(cyNdc + ryNdc, 1.0f);
		if (ndcMinX >= ndcMaxX || ndcMinY >= ndcMaxY) {
			return ::MaskedOcclusionCulling::VIEW_CULLED;
		}

		return g_msoc->TestRect(ndcMinX, ndcMinY, ndcMaxX, ndcMaxY, wMin);
	}

	// Rasterise a shape's actual triangles (in world space) as an occluder.
	// Using real triangles instead of the shape's bounding box prevents the
	// classic "sign hanging off a wall gets falsely occluded because it
	// lives inside the wall's AABB volume" failure — small meshes embedded
	// in a large mesh's bbox no longer flicker across tile boundaries.
	// Returns true if we actually rasterised; false if the shape was skipped
	// (no data, too thin on some axis, or camera inside the tight AABB).
	static bool rasterizeTriShape(NI::TriShape* shape, const TES3::Vector3& eye) {
		// Skip skinned meshes (NPCs, creatures). Their vertices live in
		// bind-pose space and need skinInstance->deform() to produce the
		// current animated positions — rasterising the bind-pose verts
		// would draw the mesh in its T-pose at world origin, far from
		// where the engine actually renders it. Moving actors are also
		// poor occluders in practice.
		if (shape->skinInstance) {
			return false;
		}

		auto data = shape->getModelData();
		if (!data) {
			return false;
		}
		// Virtual accessors: the raw vertexCount / triangleListLength
		// fields are allocation sizes, while getActive*() returns the
		// logically-valid subset (e.g. after LOD trimming). Using the
		// raw values over-reads and Intel's gather crashes on the stale
		// tail entries.
		const unsigned short vertexCount = data->getActiveVertexCount();
		if (vertexCount == 0 || data->vertex == nullptr) {
			return false;
		}
		const unsigned short triCount = data->getActiveTriangleCount();
		const NI::Triangle* tris = data->getTriList();
		if (triCount == 0 || tris == nullptr) {
			return false;
		}

		const auto& xf = shape->worldTransform;
		const auto& R = xf.rotation;
		const auto& T = xf.translation;
		const float s = xf.scale;

		// Transform every vertex once into the reusable world-space buffer.
		// We also accumulate the tight AABB for the inside-guard and
		// thin-axis gate. Not cached across frames: TriShapeData pointers
		// can be freed and reused.
		g_occluderVerts.resize(static_cast<size_t>(vertexCount) * 3);
		float* out = g_occluderVerts.data();
		float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
		float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
		for (unsigned short i = 0; i < vertexCount; ++i) {
			const auto& v = data->vertex[i];
			const float rx = R.m0.x * v.x + R.m0.y * v.y + R.m0.z * v.z;
			const float ry = R.m1.x * v.x + R.m1.y * v.y + R.m1.z * v.z;
			const float rz = R.m2.x * v.x + R.m2.y * v.y + R.m2.z * v.z;
			const float wx = rx * s + T.x;
			const float wy = ry * s + T.y;
			const float wz = rz * s + T.z;
			out[i * 3 + 0] = wx;
			out[i * 3 + 1] = wy;
			out[i * 3 + 2] = wz;
			if (wx < minX) minX = wx;
			if (wx > maxX) maxX = wx;
			if (wy < minY) minY = wy;
			if (wy > maxY) maxY = wy;
			if (wz < minZ) minZ = wz;
			if (wz > maxZ) maxZ = wz;
		}

		// Reject pencil-shaped meshes: their silhouette area is tiny and
		// the per-frame rasterisation cost isn't worth the near-zero
		// occlusion contribution. Walls/floors with a single thin axis
		// still qualify. See kOccluderMinDimension.
		const float dx = maxX - minX;
		const float dy = maxY - minY;
		const float dz = maxZ - minZ;
		const int thinAxes = (dx < kOccluderMinDimension ? 1 : 0)
			+ (dy < kOccluderMinDimension ? 1 : 0)
			+ (dz < kOccluderMinDimension ? 1 : 0);
		if (thinAxes >= 2) {
			++g_skippedThin;
			return false;
		}

		// Inside-guard: rasterising a mesh the camera sits inside would
		// cover the screen with near-face depths and falsely occlude
		// everything behind the far face. Tight AABB + small margin is
		// enough here since the real triangles are strictly inside it.
		const float m = kInsideOccluderMargin;
		if (eye.x >= minX - m && eye.x <= maxX + m &&
			eye.y >= minY - m && eye.y <= maxY + m &&
			eye.z >= minZ - m && eye.z <= maxZ + m) {
			++g_skippedInside;
			return false;
		}

		// Expand NI's 16-bit triangle list into MSOC's 32-bit index
		// buffer. Indices beyond vertexCount-1 would out-of-bounds read
		// g_occluderVerts inside Intel's gather; clamp defensively.
		g_occluderIndices.resize(static_cast<size_t>(triCount) * 3);
		unsigned int* idx = g_occluderIndices.data();
		unsigned int outTri = 0;
		for (unsigned short i = 0; i < triCount; ++i) {
			const unsigned short a = tris[i].vertices[0];
			const unsigned short b = tris[i].vertices[1];
			const unsigned short c = tris[i].vertices[2];
			if (a >= vertexCount || b >= vertexCount || c >= vertexCount) {
				continue;
			}
			idx[outTri * 3 + 0] = a;
			idx[outTri * 3 + 1] = b;
			idx[outTri * 3 + 2] = c;
			++outTri;
		}
		if (outTri == 0) {
			return false;
		}

		// VertexLayout(12, 4, 8): stride 12 bytes, y at offset 4, z at
		// offset 8 — tightly-packed float[3] per vertex.
		// BACKFACE_NONE because NIF winding is not guaranteed consistent
		// and we want every face to contribute to occluder depth.
		g_msoc->RenderTriangles(g_occluderVerts.data(), idx,
			static_cast<int>(outTri), g_worldToClip,
			::MaskedOcclusionCulling::BACKFACE_NONE,
			::MaskedOcclusionCulling::CLIP_PLANE_ALL,
			::MaskedOcclusionCulling::VertexLayout(12, 4, 8));
		return true;
	}

	// Body of the engine's CullShow, with MSOC query + occluder rasterisation
	// wedged between the frustum test and the Display call. Replaces the
	// engine's function at 0x6EB480 completely — our detour JMPs here.
	// Behaviour matches the engine 1:1 when g_msocActive is false.
	static void __fastcall cullShowBody(NI::AVObject* self, void* /*edx*/, NI::Camera* camera) {
		if (g_msocActive) ++g_recursiveCalls;
		if (self->getAppCulled()) {
			if (g_msocActive) ++g_recursiveAppCulled;
			return;
		}

		// Hierarchical frustum test, mirroring the engine's loop at 0x6EB4B7.
		// setBits tracks bits WE flipped in usedCullingPlanesBitfield so we
		// can unflip them before returning (the engine's LABEL_10 cleanup).
		uint32_t setBits[4] = { 0, 0, 0, 0 };
		const int nPlanes = camera->countCullingPlanes;
		auto* mask = camera->usedCullingPlanesBitfield;

		auto restoreIgnoreBits = [&]() {
			for (int j = 0; j < nPlanes; ++j) {
				const uint32_t jbit = 1u << (j & 0x1F);
				if (jbit & setBits[j >> 5]) {
					mask[j >> 5] &= ~jbit;
				}
			}
		};

		const float boundRadius = self->worldBoundRadius;
		for (int i = nPlanes - 1; i >= 0; --i) {
			const uint32_t bit = 1u << (i & 0x1F);
			const int word = i >> 5;
			if ((bit & mask[word]) != 0) {
				continue;
			}
			const auto* plane = static_cast<const TES3::Vector4*>(camera->cullingPlanePtrs.storage[i]);
			const float d = plane->x * self->worldBoundOrigin.x
				+ plane->y * self->worldBoundOrigin.y
				+ plane->z * self->worldBoundOrigin.z
				- plane->w;
			if (d <= -boundRadius) {
				if (g_msocActive) ++g_recursiveFrustumCulled;
				restoreIgnoreBits();
				return;
			}
			if (d >= boundRadius) {
				mask[word] |= bit;
				setBits[word] |= bit;
			}
		}

		if (g_msocActive) {
			// Small leaf geometry: defer display until the main
			// traversal has finished populating the depth buffer. Their
			// MSOC test then runs against the enriched buffer instead
			// of whatever occluders happened to precede them in
			// scene-graph order. NiTriBasedGeom covers both NiTriShape
			// and NiTriStrips (Morrowind ships many decorative meshes
			// as strip geometry). Geometry leaves have no children so
			// deferring their display doesn't skip any subtree. Shapes
			// that would themselves rasterise (radius >= min) stay
			// inline so the buffer keeps growing during the main pass.
			if (boundRadius < kOccluderRadiusMin
				&& self->isInstanceOfType(NI::RTTIStaticPtr::NiTriBasedGeom)) {
				g_pendingDisplays.push_back({ self, camera });
				++g_deferred;
				restoreIgnoreBits();
				return;
			}
			if (boundRadius < kOccluderRadiusMin) {
				++g_smallInlineTested;
			}

			++g_queryTested;
			const auto result = testSphereVisible(self->worldBoundOrigin, boundRadius);
			if (result == ::MaskedOcclusionCulling::OCCLUDED) {
				++g_queryOccluded;
				restoreIgnoreBits();
				return;
			}
			if (result == ::MaskedOcclusionCulling::VIEW_CULLED) {
				++g_queryViewCulled;
			}
			if (self->isInstanceOfType(NI::RTTIStaticPtr::NiTriShape)
				&& boundRadius <= kOccluderRadiusMax) {
				const auto& eye = camera->worldTransform.translation;
				if (rasterizeTriShape(static_cast<NI::TriShape*>(self), eye)) {
					++g_rasterizedAsOccluder;
				}
			}
		}

		self->vTable.asAVObject->display(self, camera);

		restoreIgnoreBits();
	}

	// Drain the deferred-display queue built during the main traversal.
	// Each entry is re-tested against the now-complete depth buffer and
	// displayed if still visible. Must run before g_msocActive is cleared
	// so any counters it updates land in the current frame's log line.
	static void drainPendingDisplays() {
		for (const auto& p : g_pendingDisplays) {
			++g_queryTested;
			const auto result = testSphereVisible(
				p.shape->worldBoundOrigin, p.shape->worldBoundRadius);
			if (result == ::MaskedOcclusionCulling::OCCLUDED) {
				++g_queryOccluded;
				continue;
			}
			if (result == ::MaskedOcclusionCulling::VIEW_CULLED) {
				++g_queryViewCulled;
			}
			p.shape->vTable.asAVObject->display(p.shape, p.camera);
		}
		g_pendingDisplays.clear();
	}

	// Function-level detour installed at NiAVObject::CullShow (0x6EB480).
	// Every direct caller in the engine — NiNode::Display, all 4
	// NiBSPNode::Display quadrants, NiSwitchNode::Display, NiCamera::Click —
	// now lands here (verified via IDA get_callers: exactly 7 direct
	// callers, no indirect dispatch). The top-level entry for the
	// main-scene worldCamera pass (g_inRenderMainScene set by
	// renderMainScene_wrapper, camera matches mainCamera, not already
	// inside an MSOC traversal) drives the per-frame MSOC setup/teardown.
	// Every other entry — recursive descent inside an active traversal,
	// non-main-camera Clicks like shadow/water/arm, Clicks from outside
	// renderMainScene (load screen, screenshots, etc.) — just runs the
	// body, matching the engine 1:1.
	static void __fastcall CullShow_detour(NI::AVObject* self, void* edx, NI::Camera* camera) {
		bool isTopLevel = false;
		if (!g_msocActive && g_inRenderMainScene) {
			auto wc = TES3::WorldController::get();
			NI::Camera* mainCamera = wc ? wc->worldCamera.cameraData.camera.get() : nullptr;
			isTopLevel = (camera == mainCamera);
		}

		if (isTopLevel) {
			g_msoc->ClearBuffer();
			g_recursiveCalls = 0;
			g_recursiveAppCulled = 0;
			g_recursiveFrustumCulled = 0;
			g_rasterizedAsOccluder = 0;
			g_skippedInside = 0;
			g_skippedThin = 0;
			g_queryTested = 0;
			g_queryOccluded = 0;
			g_queryViewCulled = 0;
			g_queryNearClip = 0;
			g_deferred = 0;
			g_smallInlineTested = 0;
			uploadCameraTransform(camera);
			g_msocActive = true;
		}

		cullShowBody(self, edx, camera);

		if (isTopLevel) {
			drainPendingDisplays();
			g_msocActive = false;

			// Cumulative counters survive across frames so rare OCCLUDED
			// events — scenes where one building sits squarely behind
			// another — show up even when the 300-frame sampling misses
			// them. Logged alongside per-frame counts so we can spot when
			// the ratio totalOccluded/totalTested is nonzero.
			static uint64_t totalTested = 0;
			static uint64_t totalOccluded = 0;
			static uint64_t totalViewCulled = 0;
			totalTested += g_queryTested;
			totalOccluded += g_queryOccluded;
			totalViewCulled += g_queryViewCulled;

			// Always log frames with any OCCLUDED so per-test spikes are
			// captured regardless of the 300-frame baseline sampling.
			// That's the reconciliation channel for "I see culling but
			// the counters say 0" — if OCCLUDED events happen, every
			// frame they occur will appear in the log.
			static unsigned int frameCounter = 0;
			++frameCounter;
			const bool baselineTick = (frameCounter % 300) == 0;
			const bool hadOccluded = g_queryOccluded > 0;
			if (baselineTick || hadOccluded) {
				log::getLog() << "MSOC: frame " << frameCounter
					<< " rasterized=" << g_rasterizedAsOccluder
					<< " queryOccluded=" << g_queryOccluded
					<< "/" << g_queryTested
					<< " viewCulled=" << g_queryViewCulled
					<< " nearClip=" << g_queryNearClip
					<< " deferred=" << g_deferred
					<< " smallInline=" << g_smallInlineTested
					<< " recursive=" << g_recursiveCalls
					<< " appCulled=" << g_recursiveAppCulled
					<< " frustumCulled=" << g_recursiveFrustumCulled
					<< " insideSkipped=" << g_skippedInside
					<< " thinSkipped=" << g_skippedThin
					<< " cumul=" << totalOccluded << "/" << totalTested
					<< "(vc=" << totalViewCulled << ")" << std::endl;
			}
		}
	}

	// Wrapper for TES3Game_static::renderMainScene (0x41C400). Installed at
	// its 3 known call sites (renderNextFrame, takeScreenshot,
	// createSaveScreenshot; verified via IDA get_callers). Sets
	// g_inRenderMainScene while the function is on the stack so
	// CullShow_detour knows the per-frame main scene is active. The
	// wasActive save/restore makes this reentry-safe even though reentry
	// isn't expected — nesting would just run the inner call with the
	// flag already set, and the outer restore is a no-op.
	using RenderMainSceneFn = void(__cdecl*)();
	static const auto renderMainScene_original = reinterpret_cast<RenderMainSceneFn>(0x41C400);

	static void __cdecl renderMainScene_wrapper() {
		const bool wasActive = g_inRenderMainScene;
		g_inRenderMainScene = true;
		renderMainScene_original();
		g_inRenderMainScene = wasActive;
	}

	void installPatches() {
		auto& log = log::getLog();

		log << "MSOC: installPatches entered; Configuration::EnableDX8OcclusionCulling="
			<< (Configuration::EnableDX8OcclusionCulling ? "true" : "false")
			<< std::endl;

		if (!Configuration::EnableDX8OcclusionCulling) {
			log << "MSOC: disabled via Configuration::EnableDX8OcclusionCulling." << std::endl;
			return;
		}

		g_msoc = ::MaskedOcclusionCulling::Create();
		if (!g_msoc) {
			log << "MSOC: MaskedOcclusionCulling::Create() returned null; occlusion disabled." << std::endl;
			return;
		}
		g_msoc->SetResolution(kMsocWidth, kMsocHeight);
		g_msoc->SetNearClipPlane(kNearClipW);

		// Function-level detour at NiAVObject::CullShow. The 5-byte
		// prologue (sub esp,14h; push ebx; push esi) gets overwritten
		// with a JMP to our detour, which reimplements the body
		// end-to-end (no trampoline needed — we never call back into
		// 0x6EB480). This replaces the previous 7-call-site patch and
		// is equivalent in coverage: IDA get_callers shows exactly 7
		// direct callers, all Display methods + NiCamera::Click.
		genJumpUnprotected(0x6EB480, reinterpret_cast<DWORD>(CullShow_detour));

		// Call-site wrappers for renderMainScene. Three known callers
		// (IDA get_callers on 0x41C400): the per-frame path, screenshot
		// capture, save-thumbnail capture. Call-site wrapping avoids
		// the function-prologue trampoline problem — we need to
		// actually call the original — and the enforcement check
		// catches address drift on new game builds.
		unsigned renderMainSceneInstalled = 0;
		const DWORD wrapperAddr = reinterpret_cast<DWORD>(renderMainScene_wrapper);
		static const uintptr_t kRenderMainSceneCallSites[3] = {
			0x41C08E, // TES3Game::renderNextFrame
			0x42E655, // WorldControllerRenderTarget::takeScreenshot
			0x4B50FF, // TES3File_static::createSaveScreenshot
		};
		for (auto site : kRenderMainSceneCallSites) {
			if (genCallEnforced(site, 0x41C400, wrapperAddr)) {
				++renderMainSceneInstalled;
			}
			else {
				log << "MSOC: failed to wrap renderMainScene call at 0x"
					<< std::hex << site << std::dec << std::endl;
			}
		}

		log << "MSOC: CullShow detour installed at 0x6EB480; wrapped "
			<< renderMainSceneInstalled << " / 3 renderMainScene call sites ("
			<< kMsocWidth << "x" << kMsocHeight << " tile buffer)." << std::endl;
	}

}
