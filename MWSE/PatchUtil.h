#pragma once

namespace mwse::patch {
	void installPatches();
	void installPostLuaPatches();
	void installPostInitializationPatches();

	bool installMiniDumpHook();
}
