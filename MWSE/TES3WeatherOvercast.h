#pragma once

#include "TES3Weather.h"

namespace TES3 {
	struct WeatherOvercast : Weather {
		WeatherOvercast() = delete;
		~WeatherOvercast() = delete;

		void simulate(float transitionScalar, float deltaTime);
	};
	static_assert(sizeof(WeatherOvercast) == 0x318, "TES3::WeatherOvercast failed size validation");
}
