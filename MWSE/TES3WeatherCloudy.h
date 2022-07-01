#pragma once

#include "TES3Weather.h"

namespace TES3 {
	struct WeatherCloudy : Weather {
		WeatherCloudy() = delete;
		~WeatherCloudy() = delete;
	};
	static_assert(sizeof(WeatherCloudy) == 0x318, "TES3::WeatherCloudy failed size validation");
}
