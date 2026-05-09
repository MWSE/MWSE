#pragma once
#include "TES3Weather.h"

#include "NIIteratedList.h"
#include "NIVector2.h"

namespace TES3 {
	struct WeatherBlight : Weather {
		NI::Vector2 stormOrigin; // 0x318
		NI::IteratedList<Spell*> blightDiseases; // 0x320
		float diseaseChance; // 0x334
		float diseaseTransitionThreshold; // 0x338
		float stormThreshold; // 0x33C

		WeatherBlight() = delete;
		~WeatherBlight() = delete;
	};
	static_assert(sizeof(WeatherBlight) == 0x340, "TES3::WeatherBlight failed size validation");
}
