#pragma once

#include "TES3Weather.h"

namespace TES3 {
	struct WeatherThunder : Weather {
		char soundIDThunder1[260]; // 0x318
		char soundIDThunder2[260]; // 0x41C
		char soundIDThunder3[260]; // 0x520
		char soundIDThunder4[260]; // 0x624
		char soundIDRainLoop[260]; // 0x728
		Sound* thunderSound1; // 0x82C
		Sound* thunderSound2; // 0x830
		Sound* thunderSound3; // 0x834
		Sound* thunderSound4; // 0x838
		int thunderSoundCount; // 0x83C
		Sound* soundRainLoop; // 0x840
		float thunderFrequency; // 0x844
		float thunderThreshold; // 0x848
		float rainRadius; // 0x84C
		float rainHeightMin; // 0x850
		float rainHeightMax; // 0x854
		float rainThreshold; // 0x858
		float rainEntranceSpeed; // 0x85C
		float raindropsMax; // 0x860
		float flashDecrement; // 0x864
		bool rainPlaying; // 0x868

		WeatherThunder() = delete;
		~WeatherThunder() = delete;

		//
		// Custom functions.
		//

		bool setRainLoopSoundID(const char* id);
		bool setThunder1SoundID(const char* id);
		bool setThunder2SoundID(const char* id);
		bool setThunder3SoundID(const char* id);
		bool setThunder4SoundID(const char* id);

	};
	static_assert(sizeof(WeatherThunder) == 0x86C, "TES3::WeatherThunder failed size validation");
}
