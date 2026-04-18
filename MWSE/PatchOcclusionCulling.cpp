#include "PatchOcclusionCulling.h"

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

#include <vector>

namespace mwse::patch::occlusion {

	// MSOC tile-buffer resolution. Decoupled from the game viewport: the
	// rasterizer works in NDC via the camera's world-to-clip matrix, then maps
	// NDC to this framebuffer via its own internal scale/offset.
	constexpr unsigned int kMsocWidth = 512;
	constexpr unsigned int kMsocHeight = 256;

	// Minimum world-space occluder radius. Shapes under this are skipped by the
	// object-root occluder walk. Landscape bypasses the filter (see installPatches
	// notes: landscape is a single large NiTriShape, no smaller leaves).
	constexpr float kOccluderRadiusMin = 256.0f;

	static moc::MaskedOcclusionCulling g_msoc;

	// True only while the scene graph is being traversed for the worldCamera
	// main pass. The recursive CullShow detour gates its sphere test on this so
	// that armCamera/menuCamera/shadowCamera passes are unaffected.
	static bool g_msocActive = false;

	// Original CullShow; all seven call sites are redirected to our detours,
	// which forward here for the actual work.
	using CullShowFn = void(__thiscall*)(NI::AVObject*, NI::Camera*);
	static const auto CullShow_original = reinterpret_cast<CullShowFn>(0x6EB480);

	// Per-frame scratch for transformed occluder vertices.
	static std::vector<float> g_vertexScratch;

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

	// Tree walk for the occluder pass. landscapeMode=true disables the size
	// filter (the landscape root leads to a single large NiTriShape that must
	// always be rasterized in full). landscapeMode=false applies the radius
	// filter at each leaf.
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

	// Top-level CullShow replacement at the NiCamera::Click site (0x6CC874).
	// Runs once per camera per frame; we only drive MSOC for the worldCamera
	// main pass and forward unchanged for all other cameras.
	static void __fastcall detour_CullShow_topLevel(NI::AVObject* self, void* /*edx*/, NI::Camera* camera) {
		auto wc = TES3::WorldController::get();
		NI::Camera* mainCamera = wc ? wc->worldCamera.cameraData.camera.get() : nullptr;

		if (camera != mainCamera) {
			CullShow_original(self, camera);
			return;
		}

		g_msoc.ClearBuffer();
		uploadCameraTransform(camera);

		auto dh = TES3::DataHandler::get();
		if (dh) {
			if (dh->worldLandscapeRoot) {
				walkAndRasterize(dh->worldLandscapeRoot, /*landscapeMode=*/true);
			}
			if (dh->worldObjectRoot) {
				walkAndRasterize(dh->worldObjectRoot, /*landscapeMode=*/false);
			}
		}

		g_msocActive = true;
		CullShow_original(self, camera);
		g_msocActive = false;
	}

	// Recursive CullShow replacement at the six in-graph sites. Tests the
	// object's worldBound against the MSOC buffer and skips the subtree if it
	// is fully occluded; otherwise falls through to the original CullShow,
	// which runs the usual frustum test and Display dispatch.
	static void __fastcall detour_CullShow_recursive(NI::AVObject* self, void* /*edx*/, NI::Camera* camera) {
		if (g_msocActive && self && !self->getAppCulled()) {
			auto result = g_msoc.TestSphere(&self->worldBoundOrigin.x, self->worldBoundRadius);
			if (result == moc::CullingResult::OCCLUDED) {
				return;
			}
		}
		CullShow_original(self, camera);
	}

	void installPatches() {
		if (!Configuration::EnableDX8OcclusionCulling) {
			return;
		}

		g_msoc.SetResolution(kMsocWidth, kMsocHeight);

		// Top-level site inside NiCamera::Click.
		genCallEnforced(0x6CC874, 0x6EB480, reinterpret_cast<DWORD>(detour_CullShow_topLevel));

		// Recursive sites: NiNode::Display, NiBSPNode::Display (4 quadrants),
		// NiSwitchNode::Display. Enumerated via xrefs to NiAVObject::CullShow
		// (0x6EB480); all seven call sites are covered.
		genCallEnforced(0x6C91C0, 0x6EB480, reinterpret_cast<DWORD>(detour_CullShow_recursive));
		genCallEnforced(0x6D653B, 0x6EB480, reinterpret_cast<DWORD>(detour_CullShow_recursive));
		genCallEnforced(0x6D6547, 0x6EB480, reinterpret_cast<DWORD>(detour_CullShow_recursive));
		genCallEnforced(0x6D6559, 0x6EB480, reinterpret_cast<DWORD>(detour_CullShow_recursive));
		genCallEnforced(0x6D6565, 0x6EB480, reinterpret_cast<DWORD>(detour_CullShow_recursive));
		genCallEnforced(0x6D83E2, 0x6EB480, reinterpret_cast<DWORD>(detour_CullShow_recursive));
	}

}
