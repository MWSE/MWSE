#pragma once

#include "CSPhysicalObject.h"

namespace se::cs {
	struct LeveledList : PhysicalObject {
		struct Node {
			Object* object;
			short levelRequirement;
		};
	};
	static_assert(sizeof(LeveledList) == sizeof(PhysicalObject), "LeveledList failed size validation");
	static_assert(sizeof(LeveledList::Node) == 0x8, "LeveledList::Node failed size validation");
}
