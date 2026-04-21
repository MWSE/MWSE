#pragma once

namespace NI {
	struct Light;
}

namespace mwse::patch::occlusion {

	// Install hooks for DX8 Masked Software Occlusion Culling.
	// No-op unless Configuration::EnableMSOC is set.
	void installPatches();

	// _Claude_ Returns true iff `light` is latched-OCCLUDED by the per-frame
	// MSOC cache populated at render time. Pure read — no testSphereVisible
	// call, no cache writes. Safe to call from the scene-graph game pass
	// (one frame behind render, matches the D3D8 enable hook's lag). Returns
	// false on cache miss, stale fingerprint, hysteresis not yet latched,
	// OcclusionCullLights off, EnableMSOC off, or backend not initialised.
	bool isLightLatchedOccluded(const NI::Light* light);

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
	//   - Observers fire only while the hook is installed
	//     (Configuration::EnableMSOC == true at startup). If you need
	//     observation without culling, set EnableMSOC on and leave
	//     OcclusionCullLights off — the hook stays, cull short-circuits.
	//
	// Thread safety: register before rendering starts. The vector is
	// iterated lock-free from the render thread; post-frame mutation is
	// undefined. Double-register of the same pointer is a no-op.
	using LightObservedCallback = void(__cdecl*)(NI::Light* light);
	void registerLightObservedCallback(LightObservedCallback cb);
	void unregisterLightObservedCallback(LightObservedCallback cb);

}

// Stable C-ABI exports for out-of-tree consumers (MGE-XE etc.). Resolve
// with GetProcAddress(GetModuleHandleA("MWSE.dll"), "mwse_...").
// NI::Light* is passed as void* across the ABI boundary; consumers are
// expected to know the engine offsets they need.
extern "C" __declspec(dllexport)
void __cdecl mwse_registerLightObservedCallback(void(__cdecl* cb)(void* niLight));

extern "C" __declspec(dllexport)
void __cdecl mwse_unregisterLightObservedCallback(void(__cdecl* cb)(void* niLight));
