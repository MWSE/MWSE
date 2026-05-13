#pragma once

#include "NIGeometryData.h"

namespace NI {
	struct LinesData : GeometryData {
		bool* lineSegmentFlags; // 0x34

		// Fixed loadBinary implementation that correctly reads per-segment flags.
		void loadBinary(Stream* stream);
	};
	static_assert(sizeof(LinesData) == 0x38, "NI::LinesData failed size validation");
}
