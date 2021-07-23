#pragma once

#include "TES3Defines.h"

#include "TES3Item.h"
#include "TES3IteratedList.h"

namespace TES3 {
	struct Book : Item {
		IteratedList<TES3::BaseObject*> stolenList; // 0x30
		char * name; // 0x44
		Script * script; // 0x48
		char * model; // 0x4C
		char * icon; // 0x50
		float weight; // 0x54
		long value; // 0x58
		int bookType;
		int skillToRaise;
		int enchantCapacity;
		Enchantment * enchantment;
		int unknown_0x6C;

		//
		// Other related this-call functions.
		//

		const char* getBookText();

		void setIconPath(const char* path);

	};
	static_assert(sizeof(Book) == 0x70, "TES3::Book failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Book)
