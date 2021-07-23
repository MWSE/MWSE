#include "TES3WeatherRain.h"

namespace TES3 {
	bool WeatherRain::setRainLoopSoundID(const char* id) {
		return strcpy_s(soundIDRainLoop, sizeof(soundIDRainLoop), id) == 0;
	}
}
