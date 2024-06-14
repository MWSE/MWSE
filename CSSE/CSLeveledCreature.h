#pragma once

#include "CSLeveledList.h"

#include "NIIteratedList.h"

namespace se::cs {
	struct LeveledCreature : LeveledList {
		int unknown_0x48;
		NI::IteratedList<Node*>* list; // 0x4C
		size_t listSize; // 0x50
		unsigned int leveledFlags; // 0x54
		BYTE chanceForNone; // 0x58

		bool getCalculateFromAllLevelsLessThanEqualToPCLevel() const;
	};
	static_assert(sizeof(LeveledCreature) == 0x5C, "LeveledCreature failed size validation");
}
