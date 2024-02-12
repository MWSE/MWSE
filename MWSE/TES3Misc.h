#pragma once

#include "TES3Defines.h"

#include "TES3Item.h"
#include "TES3IteratedList.h"

namespace TES3 {
	struct Misc : Item {
		IteratedList<TES3::BaseObject*> stolenList; // 0x30
		char * name; // 0x44
		Script * script; // 0x48
		char * model; // 0x4C
		char * icon; // 0x50
		float weight; // 0x54
		long value; // 0x58
		unsigned int flags; // 0x5C

		static constexpr auto OBJECT_TYPE = ObjectType::Misc;

		Misc();
		~Misc();

		//
		// Other related this-call functions.
		//

		int getGoldStackCount() const;

		//
		// Custom functions.
		//

		void setIconPath(const char* path);

		SoulGemData * getSoulGemData() const;
		bool isSoulGem() const;
		int getSoulGemCapacity() const;

		bool isGold() const;

		bool getIsKey() const;
		void setIsKey(bool value);

	};
	static_assert(sizeof(Misc) == 0x60, "TES3::Misc failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Misc)
