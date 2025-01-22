#pragma once

#include "CSLeveledList.h"

#include "NIIteratedList.h"

namespace se::cs {
	struct LeveledItem : LeveledList {
		NI::IteratedList<Node*>* list; // 0x48
		size_t listSize; // 0x4C
		unsigned int leveledFlags; // 0x50
		BYTE chanceForNone; // 0x54

		bool getCalculateFromEachItem() const;
		bool getCalculateFromAllLevelsLessThanEqualToPCLevel() const;
	};
	static_assert(sizeof(LeveledItem) == 0x58, "LeveledItem failed size validation");
}
