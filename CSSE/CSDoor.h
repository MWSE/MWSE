#pragma once

#include "CSAnimatedObject.h"

namespace se::cs {
	struct Door : AnimatedObject {
		char name[32]; // 0x4C
		char model[32]; // 0x6C
		Script* script; // 0x8C
		Sound* openSound; // 0x90
		Sound* closeSound; // 0x94
	};
	static_assert(sizeof(Door) == 0x98, "TES3::Door failed size validation");
}
