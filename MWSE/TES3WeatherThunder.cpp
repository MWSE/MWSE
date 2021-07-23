#include "TES3WeatherThunder.h"

namespace TES3 {
	bool WeatherThunder::setRainLoopSoundID(const char* id) {
		return strcpy_s(soundIDAmbientLoop, sizeof(soundIDAmbientLoop), id) == 0;
	}

	bool WeatherThunder::setThunder1SoundID(const char* id) {
		return strcpy_s(soundIdThunder1, sizeof(soundIdThunder1), id) == 0;
	}

	bool WeatherThunder::setThunder2SoundID(const char* id) {
		return strcpy_s(soundIdThunder2, sizeof(soundIdThunder2), id) == 0;
	}

	bool WeatherThunder::setThunder3SoundID(const char* id) {
		return strcpy_s(soundIdThunder3, sizeof(soundIdThunder3), id) == 0;
	}

	bool WeatherThunder::setThunder4SoundID(const char* id) {
		return strcpy_s(soundIdThunder4, sizeof(soundIdThunder4), id) == 0;
	}
}
