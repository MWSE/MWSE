#include "TES3WeatherThunder.h"

#include "TES3Sound.h"

namespace TES3 {
	bool WeatherThunder::setRainLoopSoundID(const char* id) {
		if (id == nullptr) {
			soundIDRainLoop[0] = 0;
		}
		else if (strcpy_s(soundIDRainLoop, sizeof(soundIDRainLoop), id) != 0) {
			return false;
		}

		if (soundRainLoop) {
			// Stop previous sound.
			if (soundRainLoop->isPlaying()) {
				soundRainLoop->stop();
				rainPlaying = false;
			}

			// Clearing the sound pointer will cause the weather code to resolve the new sound id and play it.
			soundRainLoop = nullptr;
		}
		return true;
	}

	bool WeatherThunder::setThunder1SoundID(const char* id) {
		return strcpy_s(soundIDThunder1, sizeof(soundIDThunder1), id) == 0;
	}

	bool WeatherThunder::setThunder2SoundID(const char* id) {
		return strcpy_s(soundIDThunder2, sizeof(soundIDThunder2), id) == 0;
	}

	bool WeatherThunder::setThunder3SoundID(const char* id) {
		return strcpy_s(soundIDThunder3, sizeof(soundIDThunder3), id) == 0;
	}

	bool WeatherThunder::setThunder4SoundID(const char* id) {
		return strcpy_s(soundIDThunder4, sizeof(soundIDThunder4), id) == 0;
	}
}
