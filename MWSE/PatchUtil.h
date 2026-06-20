#pragma once

namespace mwse::patch {
	void installPatches();
	void installPostLuaPatches();
	void installPostInitializationPatches();

	void uninstallPatches();

	bool installMiniDumpHook();

	// Drive the deferred local-map tile completion + compositor; called once per frame from the
	// WorldController::tickClock wrapper (the per-frame MWSE injection point).
	void drainDeferredMapTiles();
	// Force-complete every deferred local-map tile immediately, so a pending tile's raw Cell* can
	// never outlive its cell across a cross/teardown boundary. A no-op when nothing is pending.
	void completeDeferredMapTilesNow();
}
