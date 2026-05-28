#pragma once

#include "TES3Defines.h"

#include "NIScreenPolygon.h"

namespace TES3 {
	struct LoadScreenManager {
		struct Path {
			char path[48];
			char unknown_30;

			Path() = delete;
			~Path() = delete;
		};
		int state; // 0x0
		CutscenePlayer* cutscenePlayer; // 0x4
		char unknown_0x8;
		int unknown_0xC;
		NI::Pointer<NI::ScreenPolygon> screenPolygon; // 0x10
		int countSplashScreens; // 0x14
		int countSplashScreens2; // 0x18 // Duplicate?
		int unknown_0x1C;
		int unknown_0x20;
		int unknown_0x24;
		char unknown_0x28;
		char unknown_0x29;
		Path* paths; // 0x2C

		LoadScreenManager() = delete;
		~LoadScreenManager() = delete;

		void open(const char* movieFile, bool canSkip = true, bool alwaysFalse1 = false, bool alwaysFalse2 = false, bool unknown1 = false, bool alwaysFalse3 = false, bool unknown2 = true);
		bool render(bool unknown1 = true, bool unknown2 = true, bool unknown3 = true);
		void clearMovieState(bool unknown = true);

		bool play(const char* movieFile, bool canSkip = true);
		bool shouldContinueRendering() const;
	};
	static_assert(sizeof(LoadScreenManager) == 0x30, "TES3::LoadScreenManager failed size validation");
	static_assert(sizeof(LoadScreenManager::Path) == 0x31, "TES3::LoadScreenManager::Path failed size validation");
}
