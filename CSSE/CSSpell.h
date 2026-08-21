#pragma once

#include "CSObject.h"
#include "CSEffect.h"

#include "TES3SpellFlags.h"

namespace se::cs {
	struct Spell : Object {
		char* objectID; // 0x28
		char* name; // 0x2C
		TES3::SpellCastType::value_type castType; // 0x2E
		unsigned short magickaCost; // 0x32
		Effect effects[8]; // 0x34
		unsigned int spellFlags; // 0xF4
		int useCount; // 0xF8

		bool getSpellFlag(TES3::SpellFlag::Flag flag) const;
		bool getPlayerStart() const;

		bool search(std::string_view needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Spell) == 0xFC, "Spell failed size validation");
}

