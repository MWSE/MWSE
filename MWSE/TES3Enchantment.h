#pragma once

#include "TES3Defines.h"

#include "TES3MagicEffect.h"
#include "TES3Object.h"

namespace TES3 {
	enum class EnchantmentCastType : unsigned short {
		Once,
		OnStrike,
		OnUse,
		Constant,
		Invalid
	};

	namespace EnchantmentFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			AutoCalc = 0x1,

			AllFlags = (AutoCalc),
			NoFlags = 0,
		};

		enum FlagBit {
			AutoCalcBit = 0,
		};
	}

	struct Enchantment : Object {
		char * objectID; // 0x28
		EnchantmentCastType castType; // 0x2C
		unsigned short chargeCost; // 0x2E
		unsigned short maxCharge; // 0x30
		short pad_32; // 0x32
		Effect effects[8]; // 0x34
		unsigned int flags; // 0xF4

		static constexpr auto OBJECT_TYPE = ObjectType::Enchantment;

		Enchantment();
		~Enchantment();

		//
		// Custom functions.
		//

		bool getSpellFlag(EnchantmentFlag::Flag flag) const;
		void setSpellFlag(EnchantmentFlag::Flag flag, bool value);

		bool getAutoCalc() const;
		void setAutoCalc(bool value);

		size_t getActiveEffectCount() const;
		int getFirstIndexOfEffect(int effectId) const;
		bool hasEffect(int effectId) const;

		std::reference_wrapper<Effect[8]> getEffects();

	};
	static_assert(sizeof(Enchantment) == 0xF8, "TES3::Enchantment failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Enchantment)
