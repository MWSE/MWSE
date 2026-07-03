#pragma once

namespace mwse::patch {
	void installPatches();
	void installPostLuaPatches();
	void installPostInitializationPatches();

	void uninstallPatches();

	bool installMiniDumpHook();

	// Drive the deferred local-map tile completion + compositor; called once per frame from the
	// EnterFrame hook, which runs in menu mode too so pendings cannot stall while a menu is open.
	void drainDeferredMapTiles();
	// Drop every deferred local-map tile without completing it; called at game-load entry so stale
	// pending world-map paints from the pre-load world are never replayed onto the restored map.
	void discardDeferredMapTiles();
}
