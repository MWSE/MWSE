#pragma once

#include "NIPoint3.h"

namespace NI {
	struct Plane {
		Point3 normal; // 0x0
		float constant; // 0xC
	};
	static_assert(sizeof(Plane) == 0x10, "NI::Plane failed size validation");
}
