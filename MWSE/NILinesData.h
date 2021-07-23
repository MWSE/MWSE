#pragma once

#include "NIGeometryData.h"

namespace NI {
	struct LinesData : GeometryData {
		bool* flags; // 0x34

		//
		// Custom functions.
		//

		// Fixed loadBinary call.
		void loadBinary(Stream* stream);

	};
	static_assert(sizeof(LinesData) == 0x38, "NI::LinesData failed size validation");
}
