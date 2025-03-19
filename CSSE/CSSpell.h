#pragma once

#include "CSObject.h"
#include "CSEffect.h"

namespace se::cs {
	namespace SpellCastType {
		typedef unsigned char value_type;

		enum SpellCastType : value_type {
			Spell = 0,
			Ability = 1,
			Blight = 2,
			Disease = 3,
			Curse = 4,
			Power = 5,

			FirstCastType = Spell,
			LastCastType = Power
		};
	}

	namespace SpellFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			AutoCalc = 0x1,
			PCStartSpell = 0x2,
			AlwaysSucceeds = 0x4,

			AllSpellFlags = (AutoCalc | PCStartSpell | AlwaysSucceeds),
			NoSpellFlags = 0
		};

		enum FlagBit {
			AutoCalcBit = 0,
			PCStartSpellBit = 1,
			AlwaysSucceedsBit = 2
		};
	}

	namespace SpellOrigin {
		enum SpellOrigin {
			Module = 1,
			Spellmaker,
			FirstSpellOrigin = Module,
			LastSpellOrigin = Spellmaker
		};
	}

	struct Spell : Object {
		char* objectID; // 0x28
		char* name; // 0x2C
		SpellCastType::value_type castType; // 0x2E
		unsigned short magickaCost; // 0x32
		Effect effects[8]; // 0x34
		unsigned int spellFlags; // 0xF4
		int useCount; // 0xF8

		bool getSpellFlag(SpellFlag::Flag flag) const;
		bool getPlayerStart() const;

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Spell) == 0xFC, "Spell failed size validation");
}

