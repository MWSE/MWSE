#pragma once

#include "TES3Weather.h"

namespace TES3 {
	struct WeatherSnow : Weather {
		float snowRadius; // 0x318
		float snowHeightMin; // 0x31C
		float snowHeightMax; // 0x320
		float snowThreshold; // 0x324
		float snowEntranceSpeed; // 0x328
		float snowflakesMax; // 0x32C

		WeatherSnow() = delete;
		~WeatherSnow() = delete;

		void simulate(float transitionScalar, float deltaTime);
	};
	static_assert(sizeof(WeatherSnow) == 0x330, "TES3::WeatherSnow failed size validation");
}
