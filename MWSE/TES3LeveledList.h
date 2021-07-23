#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	namespace LeveledCreatureFlags {
		typedef unsigned int value_type;

		enum Flags : value_type {
			CalculateFromAllLevels = 0x1
		};
	}

	namespace LeveledItemFlags {
		typedef unsigned int value_type;

		enum Flags : value_type {
			CalculateForEachItem = 0x1,
			CalculateFromAllLevels = 0x2
		};
	}

	struct LeveledListNode {
		Object * object;
		short levelRequirement;
	};
	static_assert(sizeof(LeveledListNode) == 0x8, "TES3::LeveledListNode failed size validation");

	struct LeveledCreature : PhysicalObject {
		ActorAnimationController * animationController; // 0x30 // Why?
		IteratedList<LeveledListNode*> * itemList; // 0x34
		int itemCount; // 0x38
		unsigned int flags; // 0x3C
		signed char chanceForNothing; // 0x40

		//
		// Other related this-call functions.
		//

		Object * resolve();
		bool insert(BaseObject* entry, short level);
		bool remove(BaseObject* entry, short level = -1);

		//
		// Custom functions.
		//

		bool getLeveledFlag(LeveledCreatureFlags::Flags flag) const;
		void setLeveledFlag(LeveledCreatureFlags::Flags flag, bool value);
		bool getCalculateFromAllLevels() const;
		void setCalculateFromAllLevels(bool value);

	};
	static_assert(sizeof(LeveledCreature) == 0x44, "TES3::LeveledCreature failed size validation");

	struct LeveledItem : PhysicalObject {
		IteratedList<LeveledListNode*> * itemList; // 0x30
		int itemCount; // 0x34
		unsigned int flags; // 0x38
		signed char chanceForNothing; // 0x3C

		//
		// Other related this-call functions.
		//

		Object * resolve();
		bool insert(BaseObject* entry, short level);
		bool remove(BaseObject* entry, short level = -1);

		//
		// Custom functions.
		//

		bool getLeveledFlag(LeveledItemFlags::Flags flag) const;
		void setLeveledFlag(LeveledItemFlags::Flags flag, bool value);
		bool getCalculateForEachItem() const;
		void setCalculateForEachItem(bool value);
		bool getCalculateFromAllLevels() const;
		void setCalculateFromAllLevels(bool value);

	};
	static_assert(sizeof(LeveledItem) == 0x40, "TES3::LeveledItem failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::LeveledCreature)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::LeveledItem)
