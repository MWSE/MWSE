#pragma once

#include "TES3Weather.h"

namespace TES3 {
	struct WeatherFoggy : Weather {
		WeatherFoggy() = delete;
		~WeatherFoggy() = delete;

		void simulate(float transitionScalar, float deltaTime);
	};
	static_assert(sizeof(WeatherFoggy) == 0x318, "TES3::WeatherFoggy failed size validation");
}
