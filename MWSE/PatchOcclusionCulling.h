#pragma once

namespace mwse::patch::occlusion {

	// Install hooks for DX8 Masked Software Occlusion Culling.
	// No-op unless Configuration::EnableDX8OcclusionCulling is set.
	void installPatches();

}
