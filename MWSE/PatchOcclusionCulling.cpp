#include "PatchOcclusionCulling.h"

#include "Log.h"
#include "MaskedOcclusionCulling.h"
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
#include "NITriangle.h"

#include <cstdint>
#include <ostream>
#include <vector>

namespace mwse::patch::occlusion {

	// MSOC tile-buffer resolution. Decoupled from the game viewport: the
	// rasterizer works in NDC via the camera's world-to-clip matrix, then maps
	// NDC to this framebuffer via its own internal scale/offset.
	constexpr unsigned int kMsocWidth = 512;
	constexpr unsigned int kMsocHeight = 256;

	// World-space occluder radius band. Shapes outside [min, max] are not used
	// as occluders (but are still visibility-tested). The lower bound skips
	// clutter too small to occlude usefully; the upper bound skips landscape
	// chunks (cell-spanning, ~6k-7k radius) which are handled by the dedicated
	// terrain pre-pass.
	constexpr float kOccluderRadiusMin = 256.0f;
	constexpr float kOccluderRadiusMax = 4096.0f;

	// Safety margin for the inside-occluder guard. Rasterizing an object whose
	// bounding sphere contains the camera is pathological: its AABB spans the
	// entire screen and falsely occludes everything behind. A small margin
	// guards against near-miss cases (camera just outside but intersecting).
	constexpr float kInsideOccluderMargin = 64.0f;

	static moc::MaskedOcclusionCulling g_msoc;

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

	// Per-frame scratch for transformed occluder vertices.
	static std::vector<float> g_vertexScratch;

	// Per-frame diagnostics. Reset at the top of each worldCamera traversal.
	static uint64_t g_recursiveCalls = 0;
	static uint64_t g_recursiveAppCulled = 0;
	static uint64_t g_recursiveFrustumCulled = 0;
	static uint64_t g_rasterizedAsOccluder = 0;
	static uint64_t g_skippedInside = 0;

	static void rasterizeTriShape(NI::TriShape* shape) {
		auto data = shape->getModelData();
		if (!data) {
			return;
		}
		const auto vertexCount = data->vertexCount;
		const auto triangleCount = data->triangleListLength;
		if (vertexCount == 0 || triangleCount == 0 || data->vertex == nullptr || data->triangleList == nullptr) {
			return;
		}

		g_vertexScratch.resize(size_t(vertexCount) * 3);

		const auto& xf = shape->worldTransform;
		const auto& R = xf.rotation;
		const auto& T = xf.translation;
		const float s = xf.scale;

		for (unsigned short i = 0; i < vertexCount; ++i) {
			const auto& v = data->vertex[i];
			// world = R*v*s + T, unrolled to avoid temporary Vector3 allocations.
			const float rx = R.m0.x * v.x + R.m0.y * v.y + R.m0.z * v.z;
			const float ry = R.m1.x * v.x + R.m1.y * v.y + R.m1.z * v.z;
			const float rz = R.m2.x * v.x + R.m2.y * v.y + R.m2.z * v.z;
			g_vertexScratch[i * 3 + 0] = rx * s + T.x;
			g_vertexScratch[i * 3 + 1] = ry * s + T.y;
			g_vertexScratch[i * 3 + 2] = rz * s + T.z;
		}

		// NI::Triangle is three contiguous unsigned shorts; the Triangle array is
		// thus equivalent to a uint16_t[triangleCount*3] index buffer.
		auto indices = reinterpret_cast<const uint16_t*>(data->triangleList);
		g_msoc.RenderTriangles(g_vertexScratch.data(), sizeof(float) * 3,
			vertexCount, indices, triangleCount);
	}

	// Returns true if the camera lies inside (or within kInsideOccluderMargin of)
	// the shape's bounding sphere. Used to suppress self-occluding rasterization
	// when the player stands inside a building or wall.
	static bool cameraInsideShape(NI::Camera* camera, NI::AVObject* obj) {
		const auto& eye = camera->worldTransform.translation;
		const auto& c = obj->worldBoundOrigin;
		const float r = obj->worldBoundRadius + kInsideOccluderMargin;
		const float dx = eye.x - c.x;
		const float dy = eye.y - c.y;
		const float dz = eye.z - c.z;
		return (dx * dx + dy * dy + dz * dz) <= r * r;
	}

	// Tree walk for the terrain pre-pass. landscapeMode=true disables the size
	// filter (each landscape tile is a large single NiTriShape that must always
	// be rasterized in full). Only used for worldLandscapeRoot now — object
	// occluders are rasterized opportunistically during the main traversal.
	static void walkAndRasterize(NI::AVObject* obj, bool landscapeMode) {
		if (obj == nullptr || obj->getAppCulled()) {
			return;
		}

		if (obj->isInstanceOfType(NI::RTTIStaticPtr::NiTriShape)) {
			auto shape = static_cast<NI::TriShape*>(obj);
			if (!landscapeMode && shape->worldBoundRadius < kOccluderRadiusMin) {
				return;
			}
			rasterizeTriShape(shape);
			return;
		}

		if (obj->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) {
			auto node = static_cast<NI::Node*>(obj);
			const auto end = node->children.endIndex;
			for (unsigned int i = 0; i < end; ++i) {
				NI::AVObject* child = node->children.storage[i];
				if (child) {
					walkAndRasterize(child, landscapeMode);
				}
			}
		}
	}

	static void uploadCameraTransform(NI::Camera* cam) {
		// NiCamera::worldToCamera (TES3::Matrix44 at 0x90) is the combined
		// world-to-clip matrix, row-major; see NiCamera::WorldToCameraMatrix at
		// 0x6CCC70. It already includes the projection, so MSOC consumes it
		// directly.
		const float* m = reinterpret_cast<const float*>(&cam->worldToCamera);
		g_msoc.SetWorldToClip(m);
	}

	// Drop-in replacement for NiAVObject::CullShow (0x6EB480). Installed at all
	// seven call sites; does not forward to the engine. Behaviour matches the
	// engine exactly when g_msocActive is false; when true, an MSOC sphere
	// query and (for qualifying NiTriShapes) occluder rasterisation are
	// wedged between the frustum test and the Display call.
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
				// An ancestor already proved the bound fully inside this plane.
				continue;
			}
			// NiPlane layout matches TES3::Vector4: (normal.x, y, z, constant).
			const auto* plane = static_cast<const TES3::Vector4*>(camera->cullingPlanePtrs.storage[i]);
			const float d = plane->x * self->worldBoundOrigin.x
				+ plane->y * self->worldBoundOrigin.y
				+ plane->z * self->worldBoundOrigin.z
				- plane->w;
			if (d <= -boundRadius) {
				// WhichSide == 2: fully outside this plane. Bail.
				if (g_msocActive) ++g_recursiveFrustumCulled;
				restoreIgnoreBits();
				return;
			}
			if (d >= boundRadius) {
				// WhichSide == 1: fully inside. Mark so descendants skip it.
				mask[word] |= bit;
				setBits[word] |= bit;
			}
			// else WhichSide == 0: intersecting. Test inherited by descendants.
		}

		if (g_msocActive) {
			const auto result = g_msoc.TestSphere(&self->worldBoundOrigin.x, boundRadius);
			if (result == moc::CullingResult::OCCLUDED) {
				restoreIgnoreBits();
				return;
			}
			if (self->isInstanceOfType(NI::RTTIStaticPtr::NiTriShape)
				&& boundRadius >= kOccluderRadiusMin
				&& boundRadius <= kOccluderRadiusMax) {
				if (!cameraInsideShape(camera, self)) {
					rasterizeTriShape(static_cast<NI::TriShape*>(self));
					++g_rasterizedAsOccluder;
				}
				else {
					++g_skippedInside;
				}
			}
		}

		self->vTable.asAVObject->display(self, camera);

		restoreIgnoreBits();
	}

	// Wrapper for the single top-level site (NiCamera::Click at 0x6CC874).
	// Drives MSOC setup + terrain pre-pass only for the worldCamera main pass;
	// non-main cameras are forwarded to the engine's untouched CullShow at
	// 0x6EB480 so armCamera/menuCamera/shadowCamera behaviour is identical to
	// the unpatched game.
	static void __fastcall CullShow_replacement_topLevel(NI::AVObject* self, void* /*edx*/, NI::Camera* camera) {
		auto wc = TES3::WorldController::get();
		NI::Camera* mainCamera = wc ? wc->worldCamera.cameraData.camera.get() : nullptr;

		if (camera != mainCamera) {
			CullShow_original(self, camera);
			return;
		}

		g_msoc.ClearBuffer();
		g_msoc.ResetStats();
		g_recursiveCalls = 0;
		g_recursiveAppCulled = 0;
		g_recursiveFrustumCulled = 0;
		g_rasterizedAsOccluder = 0;
		g_skippedInside = 0;
		uploadCameraTransform(camera);

		// Terrain pre-pass (OpenMW's TerrainOccluder analog). Landscape is
		// rasterised upfront so subsequent object queries see the ground
		// silhouette; object occluders are rasterised opportunistically inside
		// CullShow_replacement, matching OpenMW's pass-1 behaviour.
		auto dh = TES3::DataHandler::get();
		if (dh && dh->worldLandscapeRoot) {
			walkAndRasterize(dh->worldLandscapeRoot, /*landscapeMode=*/true);
		}

		g_msocActive = true;
		CullShow_replacement(self, nullptr, camera);
		g_msocActive = false;

		static unsigned int frameCounter = 0;
		if ((++frameCounter % 300) == 0) {
			const auto covered = g_msoc.CountCoveredTiles();
			const auto& s = g_msoc.GetStats();
			log::getLog() << "MSOC: frame " << frameCounter
				<< " tris=" << s.occluderTrianglesRasterized
				<< "/" << s.occluderTrianglesSubmitted
				<< " coveredTiles=" << covered
				<< " sphereOccluded=" << s.sphereTestsOccluded
				<< "/" << s.sphereTestsTotal
				<< " recursive=" << g_recursiveCalls
				<< " appCulled=" << g_recursiveAppCulled
				<< " frustumCulled=" << g_recursiveFrustumCulled
				<< " rasterizedOccluders=" << g_rasterizedAsOccluder
				<< " insideSkipped=" << g_skippedInside << std::endl;
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

		g_msoc.SetResolution(kMsocWidth, kMsocHeight);

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
