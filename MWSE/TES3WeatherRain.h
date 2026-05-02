#pragma once

#include "TES3Weather.h"

namespace TES3 {
	struct WeatherRain : Weather {
		char soundIDRainLoop[260]; // 0x318
		float rainRadius; // 0x41C
		float rainHeightMin; // 0x420
		float rainHeightMax; // 0x424
		float rainThreshold; // 0x428
		float rainEntranceSpeed; // 0x42C
		float raindropsMax; // 0x430
		Sound* soundRainLoop; // 0x434
		bool rainPlaying;  // 0x438

		WeatherRain() = delete;
		~WeatherRain() = delete;

		//
		// Custom functions.
		//

		bool setRainLoopSoundID(const char* id);

	};
	static_assert(sizeof(WeatherRain) == 0x43C, "TES3::WeatherRain failed size validation");
}
