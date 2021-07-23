#pragma once

#include "TES3Defines.h"

#include "TES3Item.h"
#include "TES3IteratedList.h"

namespace TES3 {
	struct Lockpick : Item {
		IteratedList<TES3::BaseObject*> stolenList; // 0x30
		char name[32]; // 0x44
		Script * script; // 0x64
		char model[32]; // 0x68
		char icon[32]; // 0x88
		float weight; // 0xA8
		int value; // 0xAC
		float quality; // 0xB0
		unsigned short maxCondition; // 0xB4
		short unknown_0xB6;

		//
		// Custom functions.
		//

		void setIconPath(const char* path);
		void setName(const char* newName);

	};
	static_assert(sizeof(Lockpick) == 0xB8, "TES3::Lockpick failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Lockpick)
