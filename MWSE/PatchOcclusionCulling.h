#pragma once

namespace mwse::patch::occlusion {

	// Install hooks for DX8 Masked Software Occlusion Culling.
	// No-op unless Configuration::EnableMSOC is set.
	void installPatches();

}
