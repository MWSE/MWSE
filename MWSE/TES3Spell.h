#pragma once

#include "TES3Defines.h"
#include "TES3MagicEffect.h"

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

	struct Spell : Object {
		char * objectID; // 0x28
		char * name; // 0x2C
		SpellCastType::value_type castType; // 0x30
		char unknown_0x31; // Undefined.
		unsigned short magickaCost; // 0x32
		Effect effects[8]; // 0x34
		unsigned int spellFlags; //  0xF4

		static constexpr auto OBJECT_TYPE = ObjectType::Spell;

		Spell();
		~Spell();

		//
		// Other related this-call functions.
		//

		Effect* getLeastProficientEffect(const NPC* npc);
		Effect* getLeastProficientEffect(const MobileActor* mobile);
		Effect* getLeastProficientEffect_lua(sol::stack_object object);
		int getLeastProficientSchool(const NPC* npc);
		int getLeastProficientSchool(const MobileActor* mobile);
		int getLeastProficientSchool_lua(sol::stack_object object);
		float calculateCastChance(Reference* caster, bool checkMagicka = true, int* weakestSchoolId = nullptr);
		float calculateCastChance(MobileActor* caster, bool checkMagicka = true, int* weakestSchoolId = nullptr);
		float castChanceOnCast(MobileActor* caster, bool checkMagicka, int* weakestSchoolId);

		//
		// Custom functions.
		//

		bool getSpellFlag(SpellFlag::Flag flag) const;
		void setSpellFlag(SpellFlag::Flag flag, bool value);

		bool getAutoCalc() const;
		void setAutoCalc(bool value);
		bool getPlayerStart() const;
		void setPlayerStart(bool value);
		bool getAlwaysSucceeds() const;
		void setAlwaysSucceeds(bool value);

		bool isAbility() const;
		bool isBlightDisease() const;
		bool isCorprusDisease() const;
		bool isCommonDisease() const;
		bool isCurse() const;
		bool isDisease() const;
		bool isPower() const;
		bool isSpell() const;

		int getValue() const;

		size_t getActiveEffectCount() const;
		int getFirstIndexOfEffect(int effectId) const;
		bool hasEffect(int effectId) const;

		int calculateBasePuchaseCost() const;
		float calculateCastChance_lua(sol::table params);
		int getAutoCalcMagickaCost() const;

		bool isActiveCast() const;

		std::reference_wrapper<Effect[8]> getEffects();

	};
	static_assert(sizeof(Spell) == 0xF8, "TES3::Spell failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Spell)
