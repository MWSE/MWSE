#pragma once

#include "NIGeometry.h"

namespace NI {
	struct Lines : Geometry {

	};
	static_assert(sizeof(Lines) == 0xAC, "NI::Lines failed size validation");
}
