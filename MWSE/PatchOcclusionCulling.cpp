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
#include <cstdint>
#include <limits>
#include <ostream>

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

	// Shrink factor applied to the per-occluder AABB around its centre before
	// rasterising. Keeps the occluder strictly inside the real geometry for
	// thick convex shapes; for thin/planar meshes the shrunken box still
	// approximates the surface with bounded error. OpenMW exposes this as a
	// user setting; 0.8 matches their default feel.
	constexpr float kOccluderShrinkFactor = 0.8f;

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

	// Intel MSOC is allocated on the heap via Create()/Destroy(). We leak
	// this on process exit (same pattern as other long-lived MWSE globals).
	static ::MaskedOcclusionCulling* g_msoc = nullptr;

	// World-to-clip matrix, transposed from NI's row-major M*v layout into
	// Intel's column-major v*M layout (consecutive memory = one column).
	// Intel reads clip.x = v.x*m[0] + v.y*m[4] + v.z*m[8] + m[12].
	// Refreshed each frame at the top of CullShow_replacement_topLevel.
	static float g_worldToClip[16];

	// Belt-and-braces fallback for non-worldCamera traversals (armCamera,
	// menuCamera, shadowCamera, etc.). CullShow_replacement is designed to be
	// 1:1 with 0x6EB480 when g_msocActive is false, but forwarding the
	// non-main passes to the untouched engine function isolates MSOC risk to
	// the worldCamera main pass alone.
	using CullShowFn = void(__thiscall*)(NI::AVObject*, NI::Camera*);
	static const auto CullShow_original = reinterpret_cast<CullShowFn>(0x6EB480);

	// True only while the scene graph is being traversed for the worldCamera
	// main pass. CullShow_replacement gates MSOC work on this so armCamera /
	// menuCamera / shadowCamera passes run the pure engine-equivalent path.
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

	// Fill g_worldToClip from NI::Camera::worldToCamera. NI stores the full
	// world-to-clip matrix row-major with M*v convention (NI[0..3] is the
	// X-equation coefficients). Intel expects column-major with v*M
	// convention, which is the transpose of NI's layout.
	static void uploadCameraTransform(NI::Camera* cam) {
		const float* ni = reinterpret_cast<const float*>(&cam->worldToCamera);
		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; ++col) {
				g_worldToClip[col * 4 + row] = ni[row * 4 + col];
			}
		}
	}

	// Project a world-space point using the cached Intel-layout matrix.
	// Produces clip-space (x, y, w); z is unused by Intel's rasterizer.
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

	// Conservative sphere -> NDC-rect + wmin projection, then Intel's TestRect.
	// Treats the sphere as its axis-aligned world-space bbox ({center +/- r}
	// on each axis) and projects all 8 corners. Matches OpenMW's
	// testVisibleAABB for AABBs; for a sphere the bbox is at worst sqrt(3)
	// loose, which is tolerable because TestRect is conservative anyway.
	//
	// If any corner lies behind the near plane (clip.w <= kNearClipW) we
	// bail early as VISIBLE: the straddling case needs full 3D clipping that
	// TestRect does not perform, and conservatively treating the sphere as
	// visible is the correct fallback.
	static ::MaskedOcclusionCulling::CullingResult testSphereVisible(
		const TES3::Vector3& center, float radius) {
		const float r = radius;
		const float corners[8][3] = {
			{ center.x - r, center.y - r, center.z - r },
			{ center.x + r, center.y - r, center.z - r },
			{ center.x - r, center.y + r, center.z - r },
			{ center.x + r, center.y + r, center.z - r },
			{ center.x - r, center.y - r, center.z + r },
			{ center.x + r, center.y - r, center.z + r },
			{ center.x - r, center.y + r, center.z + r },
			{ center.x + r, center.y + r, center.z + r },
		};

		float ndcMinX = FLT_MAX, ndcMinY = FLT_MAX;
		float ndcMaxX = -FLT_MAX, ndcMaxY = -FLT_MAX;
		float wMin = FLT_MAX;

		for (int i = 0; i < 8; ++i) {
			const ClipXYW c = projectWorld(corners[i][0], corners[i][1], corners[i][2]);
			if (c.w <= kNearClipW) {
				++g_queryNearClip;
				return ::MaskedOcclusionCulling::VISIBLE;
			}
			const float invW = 1.0f / c.w;
			const float ndcX = c.x * invW;
			const float ndcY = c.y * invW;
			ndcMinX = std::min(ndcMinX, ndcX);
			ndcMaxX = std::max(ndcMaxX, ndcX);
			ndcMinY = std::min(ndcMinY, ndcY);
			ndcMaxY = std::max(ndcMaxY, ndcY);
			wMin = std::min(wMin, c.w);
		}

		// Off-screen: Intel's TestRect returns VIEW_CULLED for us, but
		// clamping to [-1, 1] upfront is cheaper and gives us a hook to
		// short-circuit the call if the rect collapses.
		ndcMinX = std::max(ndcMinX, -1.0f);
		ndcMinY = std::max(ndcMinY, -1.0f);
		ndcMaxX = std::min(ndcMaxX, 1.0f);
		ndcMaxY = std::min(ndcMaxY, 1.0f);
		if (ndcMinX >= ndcMaxX || ndcMinY >= ndcMaxY) {
			return ::MaskedOcclusionCulling::VIEW_CULLED;
		}

		return g_msoc->TestRect(ndcMinX, ndcMinY, ndcMaxX, ndcMaxY, wMin);
	}

	// Rasterise a shape's shrunken world-space AABB (12 triangles) as an
	// occluder. A 12-tri cube is dramatically better at filling whole tiles
	// and thus at promoting hiZ0 — which is what sphere/rect queries
	// actually compare against. OpenMW's CellOcclusionCallback uses the same
	// pattern.
	// Returns true if we actually rasterised; false if the shape was skipped
	// (no data, too thin on some axis, or camera inside the tight AABB).
	static bool rasterizeTriShape(NI::TriShape* shape, const TES3::Vector3& eye) {
		auto data = shape->getModelData();
		if (!data) {
			return false;
		}
		const auto vertexCount = data->vertexCount;
		if (vertexCount == 0 || data->vertex == nullptr) {
			return false;
		}

		const auto& xf = shape->worldTransform;
		const auto& R = xf.rotation;
		const auto& T = xf.translation;
		const float s = xf.scale;

		// Tight world-space AABB: transform every vertex once per frame.
		// Not cached: TriShapeData pointers can be freed and reused, and a
		// stale cache entry would produce a wrong occluder for a different
		// mesh. O(V) per occluder for ~dozens of occluders per frame is
		// negligible (<1ms total).
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
			if (wx < minX) minX = wx;
			if (wx > maxX) maxX = wx;
			if (wy < minY) minY = wy;
			if (wy > maxY) maxY = wy;
			if (wz < minZ) minZ = wz;
			if (wz > maxZ) maxZ = wz;
		}

		// Reject shapes that are thin on two or more axes (pencil-shaped):
		// their cube approximation is a fat slab that would falsely occlude
		// things visible through gaps. Walls/floors with a single thin axis
		// still qualify. See kOccluderMinDimension for the threshold.
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

		// Inside-guard using the tight AABB (expanded by kInsideOccluderMargin).
		// A tight-AABB test only trips when the camera is genuinely inside
		// the mesh's volume (as opposed to worldBoundRadius, NI's loose
		// bounding sphere that on a canton wall encloses the walkway).
		const float m = kInsideOccluderMargin;
		if (eye.x >= minX - m && eye.x <= maxX + m &&
			eye.y >= minY - m && eye.y <= maxY + m &&
			eye.z >= minZ - m && eye.z <= maxZ + m) {
			++g_skippedInside;
			return false;
		}

		// Shrink AABB around its centre.
		const float cx = (minX + maxX) * 0.5f;
		const float cy = (minY + maxY) * 0.5f;
		const float cz = (minZ + maxZ) * 0.5f;
		const float hx = (maxX - minX) * 0.5f * kOccluderShrinkFactor;
		const float hy = (maxY - minY) * 0.5f * kOccluderShrinkFactor;
		const float hz = (maxZ - minZ) * 0.5f * kOccluderShrinkFactor;

		const float x0 = cx - hx, x1 = cx + hx;
		const float y0 = cy - hy, y1 = cy + hy;
		const float z0 = cz - hz, z1 = cz + hz;

		const float verts[8 * 3] = {
			x0, y0, z0, // 0
			x1, y0, z0, // 1
			x0, y1, z0, // 2
			x1, y1, z0, // 3
			x0, y0, z1, // 4
			x1, y0, z1, // 5
			x0, y1, z1, // 6
			x1, y1, z1, // 7
		};

		// Intel's RenderTriangles takes unsigned int (32-bit) indices.
		static const unsigned int indices[36] = {
			0, 1, 3,  0, 3, 2, // -Z
			4, 6, 7,  4, 7, 5, // +Z
			0, 4, 5,  0, 5, 1, // -Y
			2, 3, 7,  2, 7, 6, // +Y
			0, 2, 6,  0, 6, 4, // -X
			1, 5, 7,  1, 7, 3, // +X
		};

		// VertexLayout(12, 4, 8): stride 12 bytes, y at offset 4, z at
		// offset 8 — i.e. tightly-packed float[3] per vertex.
		// BACKFACE_NONE because we want nearest-depth to win regardless of
		// which face is toward the camera.
		g_msoc->RenderTriangles(verts, indices, 12, g_worldToClip,
			::MaskedOcclusionCulling::BACKFACE_NONE,
			::MaskedOcclusionCulling::CLIP_PLANE_ALL,
			::MaskedOcclusionCulling::VertexLayout(12, 4, 8));
		return true;
	}

	// Drop-in replacement for NiAVObject::CullShow (0x6EB480). Installed at all
	// seven call sites; does not forward to the engine. Behaviour matches the
	// engine exactly when g_msocActive is false; when true, an MSOC query
	// and (for qualifying NiTriShapes) occluder rasterisation are wedged
	// between the frustum test and the Display call.
	static void __fastcall CullShow_replacement(NI::AVObject* self, void* /*edx*/, NI::Camera* camera) {
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
				&& boundRadius >= kOccluderRadiusMin) {
				const auto& eye = camera->worldTransform.translation;
				if (rasterizeTriShape(static_cast<NI::TriShape*>(self), eye)) {
					++g_rasterizedAsOccluder;
				}
			}
		}

		self->vTable.asAVObject->display(self, camera);

		restoreIgnoreBits();
	}

	// Wrapper for the single top-level site (NiCamera::Click at 0x6CC874).
	// Drives MSOC setup only for the worldCamera main pass; non-main cameras
	// are forwarded to the engine's untouched CullShow at 0x6EB480 so
	// armCamera/menuCamera/shadowCamera behaviour is identical to the
	// unpatched game.
	static void __fastcall CullShow_replacement_topLevel(NI::AVObject* self, void* /*edx*/, NI::Camera* camera) {
		auto wc = TES3::WorldController::get();
		NI::Camera* mainCamera = wc ? wc->worldCamera.cameraData.camera.get() : nullptr;

		if (camera != mainCamera) {
			CullShow_original(self, camera);
			return;
		}

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
		uploadCameraTransform(camera);

		g_msocActive = true;
		CullShow_replacement(self, nullptr, camera);
		g_msocActive = false;

		static unsigned int frameCounter = 0;
		if ((++frameCounter % 300) == 0) {
			log::getLog() << "MSOC: frame " << frameCounter
				<< " rasterized=" << g_rasterizedAsOccluder
				<< " queryOccluded=" << g_queryOccluded
				<< "/" << g_queryTested
				<< " viewCulled=" << g_queryViewCulled
				<< " nearClip=" << g_queryNearClip
				<< " recursive=" << g_recursiveCalls
				<< " appCulled=" << g_recursiveAppCulled
				<< " frustumCulled=" << g_recursiveFrustumCulled
				<< " insideSkipped=" << g_skippedInside
				<< " thinSkipped=" << g_skippedThin << std::endl;
		}
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

		static const uintptr_t kSites[7] = {
			0x6CC874, // top-level: NiCamera::Click
			0x6C91C0, // NiNode::Display
			0x6D653B, // NiBSPNode::Display (quadrant 0)
			0x6D6547, // NiBSPNode::Display (quadrant 1)
			0x6D6559, // NiBSPNode::Display (quadrant 2)
			0x6D6565, // NiBSPNode::Display (quadrant 3)
			0x6D83E2, // NiSwitchNode::Display
		};

		unsigned installed = 0;
		for (unsigned i = 0; i < 7; ++i) {
			const uintptr_t to = (i == 0)
				? reinterpret_cast<uintptr_t>(CullShow_replacement_topLevel)
				: reinterpret_cast<uintptr_t>(CullShow_replacement);
			if (genCallEnforced(kSites[i], 0x6EB480, to)) {
				++installed;
			}
			else {
				log << "MSOC: failed to install CullShow hook at 0x" << std::hex << kSites[i] << std::dec << std::endl;
			}
		}
		log << "MSOC: installed " << installed << " / 7 CullShow hooks ("
			<< kMsocWidth << "x" << kMsocHeight << " tile buffer)." << std::endl;
	}

}
