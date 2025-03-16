#include "TES3WeatherRain.h"

#include "TES3Sound.h"

namespace TES3 {
	bool WeatherRain::setRainLoopSoundID(const char* id) {
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
}
