#pragma once

namespace TES3 {
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
}
