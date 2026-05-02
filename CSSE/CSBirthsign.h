#pragma once

#include "CSBaseObject.h"
#include "CSSpellList.h"

namespace se::cs {
	struct Birthsign : BaseObject {
		char id[32]; // 0x10
		char* name; // 0x30
		char* texturePath; // 0x34
		char* description; // 0x38
		SpellList spellList; // 0x3C

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Birthsign) == 0x54, "Birthsign failed size validation");
}
