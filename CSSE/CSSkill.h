#pragma once

#include "TES3BaseObject.h"

namespace se::cs {
	struct Skill : BaseObject {
		int skillID; // 0x10
		int attributeID; // 0x14
		int specialization; // 0x18
		float useValues[4]; // 0x1C
		const char* description; // 0x2C
	};
	static_assert(sizeof(Skill) == 0x30, "Skill failed size validation");
}
