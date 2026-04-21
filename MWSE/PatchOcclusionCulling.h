#pragma once

namespace NI {
	struct Light;
}

namespace mwse::patch::occlusion {

	// Install hooks for DX8 Masked Software Occlusion Culling.
	// _Claude_ Hooks always install — EnableMSOC is a pure runtime gate,
	// re-checked every frame inside the detour. MCM can toggle culling
	// on/off at any time. When the gate is off, the detour falls through
	// to vanilla cullShowBody at zero overhead. Resources (g_msoc + the
	// ~57MB ring-buffer threadpool) are allocated lazily on the first
	// MCM toggle-on (or eagerly at startup if EnableMSOC begins true) and
	// torn down on toggle-off (workers joined, buffers freed). Toggling
	// off→on costs one threadpool spin-up (~1ms); toggling on→off blocks
	// the next render frame for the worker join (bounded, typically <1ms
	// because workers were already suspended).
	void installPatches();

	// _Claude_ Observer callback fired once per iteration of
	// NiDX8LightManager::updateLights for every NiLight* the renderer walks
	// — before the MSOC cull decision, before the enabled check. Observers
	// see every iterated light (disabled or enabled, occluded or not) and
	// must treat the pointer as read-only.
	//
	// Use: external systems (e.g. MGE-XE) that want to snapshot the live
	// renderer-iterated light list without re-patching 0x6BB7D4. Without
	// this registry, only the first patcher wins the site.
	//
	// Constraints:
	//   - Runs inside the render-thread hot loop. Keep work minimal: dedup
	//     + cheap publish. No logging, no allocation on the steady path.
	//   - Do not mutate the light or the scene graph.
	//   - Observers fire whenever the detour runs, which is every frame
	//     once the patches are installed. EnableMSOC OFF still walks the
	//     light list (the detour just short-circuits the cull decision),
	//     so observers see the same data either way — set OcclusionCullLights
	//     off if you want observation without the cull side-effect.
	//
	// Thread safety: register before rendering starts. The vector is
	// iterated lock-free from the render thread; post-frame mutation is
	// undefined. Double-register of the same pointer is a no-op.
	using LightObservedCallback = void(__cdecl*)(NI::Light* light);
	void registerLightObservedCallback(LightObservedCallback cb);
	void unregisterLightObservedCallback(LightObservedCallback cb);

	// _Claude_ Mask query API for consumers that want to reuse MWSE's
	// CPU occlusion mask (e.g. MGE-XE culling its own draw passes
	// before submission to D3D). Result codes are frozen in ABI —
	// values match the mwse_testOcclusion* C exports below.
	enum MaskQueryResult {
		kMaskQueryVisible    = 0,  // not occluded; draw the thing
		kMaskQueryOccluded   = 1,  // fully behind the mask; safe to skip
		kMaskQueryViewCulled = 2,  // rect collapsed; outside the frustum
		kMaskQueryNotReady   = 3,  // mask not populated this frame
	};

	// True once the depth buffer reflects the complete vanilla main-scene
	// occluder set. Flips false at the next frame's ClearBuffer. Callers
	// should either gate expensive queries on this or accept that a
	// !ready query returns kMaskQueryNotReady (treat as visible).
	bool isOcclusionMaskReady();

	// World-space sphere test. Uses the same projection as the engine's
	// main-scene CullShow — if the caller's draw pass uses a different
	// camera (different near/far plane, different projection), the
	// verdict is not meaningful for that pass.
	MaskQueryResult testOcclusionSphere(
		float worldX, float worldY, float worldZ, float radius);

	// World-space AABB test. Converts to bounding sphere internally —
	// same looseness as the engine's own NiAVObject::worldBoundRadius.
	MaskQueryResult testOcclusionAABB(
		float minX, float minY, float minZ,
		float maxX, float maxY, float maxZ);

}

// Stable C-ABI exports for out-of-tree consumers (MGE-XE etc.). Resolve
// with GetProcAddress(GetModuleHandleA("MWSE.dll"), "mwse_...").
// NI::Light* is passed as void* across the ABI boundary; consumers are
// expected to know the engine offsets they need.
extern "C" __declspec(dllexport)
void __cdecl mwse_registerLightObservedCallback(void(__cdecl* cb)(void* niLight));

extern "C" __declspec(dllexport)
void __cdecl mwse_unregisterLightObservedCallback(void(__cdecl* cb)(void* niLight));

// _Claude_ MSOC mask query exports. Return values are the stable
// MWSE_OCC_* codes (match mwse::patch::occlusion::MaskQueryResult):
//   0 = Visible     (draw it)
//   1 = Occluded    (safe to skip)
//   2 = ViewCulled  (outside the frustum)
//   3 = NotReady    (mask empty or unbuilt this frame — treat as Visible)
extern "C" __declspec(dllexport)
int __cdecl mwse_isOcclusionMaskReady();

extern "C" __declspec(dllexport)
int __cdecl mwse_testOcclusionSphere(float worldX, float worldY, float worldZ, float radius);

extern "C" __declspec(dllexport)
int __cdecl mwse_testOcclusionAABB(
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ);
