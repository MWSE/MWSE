#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"
#include "TES3SpellList.h"

namespace TES3 {
	struct Birthsign : BaseObject {
		char id[32];						// 0x10
		char * name;						// 0x30
		char * texturePath;					// 0x34
		char * description;					// 0x38
		TES3::SpellList spellList;			// 0x3C
		unsigned int descriptionFileOffset; // 0x54

		static constexpr auto OBJECT_TYPE = ObjectType::Birthsign;

		Birthsign() = delete;
		~Birthsign() = delete;
		
		char * getName();
		char * getDescription();
		void freeDescription();
		std::string getAndFreeDescription();

	};
	static_assert(sizeof(Birthsign) == 0x58, "TES3::Birthsign failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Birthsign)
