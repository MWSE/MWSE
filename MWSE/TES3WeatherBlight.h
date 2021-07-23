#pragma once

#include "TES3IteratedList.h"
#include "TES3Weather.h"

namespace TES3 {
	struct WeatherBlight : Weather {
		int unknown_0x318;
		int unknown_0x31C;
		IteratedList<Spell*> blightDiseases; // 0x320
		float diseaseChance; // 0x334
		int unknown_0x338;
		float stormThreshold; // 0x33C
	};
	static_assert(sizeof(WeatherBlight) == 0x340, "TES3::WeatherBlight failed size validation");
}
