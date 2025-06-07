#pragma once

#include "TES3IteratedList.h"
#include "TES3Weather.h"

namespace TES3 {
	struct WeatherBlight : Weather {
		Vector2 stormOrigin; // 0x318
		IteratedList<Spell*> blightDiseases; // 0x320
		float diseaseChance; // 0x334
		float diseaseTransitionThreshold; // 0x338
		float stormThreshold; // 0x33C

		WeatherBlight() = delete;
		~WeatherBlight() = delete;
	};
	static_assert(sizeof(WeatherBlight) == 0x340, "TES3::WeatherBlight failed size validation");
}
