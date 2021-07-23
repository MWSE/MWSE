#pragma once

#include "TES3Defines.h"

namespace TES3 {
	struct GoldData {
		const char* id; // 0x0
		int value; // 0x4
		Misc* object; // 0x8

		//
		// Relevant static addresses.
		//

		static constexpr auto DATA_ARRAY = reinterpret_cast<GoldData*>(0x791C58);
		static constexpr auto DATA_ARRAY_COUNT = 5U;
	};
}
