#pragma once

namespace mwse {
	namespace patch {
		void installPatches();
		void installPostLuaPatches();

		bool installMiniDumpHook();
	}
}
