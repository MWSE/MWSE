#pragma once

#include "TES3Weather.h"

namespace TES3 {
	struct WeatherAsh : Weather {
		Vector2 stormOrigin; // 0x318
		float stormThreshold; // 0x320

		WeatherAsh() = delete;
		~WeatherAsh() = delete;

		void simulate(float transitionScalar, float deltaTime);
	};
	static_assert(sizeof(WeatherAsh) == 0x324, "TES3::WeatherAsh failed size validation");
}
