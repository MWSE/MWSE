#pragma once

#include "NIGeometry.h"
#include "NILinesData.h"

namespace NI {
	struct Lines : Geometry {
		Pointer<LinesData> getModelData() const;
	};
	static_assert(sizeof(Lines) == 0xAC, "NI::Lines failed size validation");
}
