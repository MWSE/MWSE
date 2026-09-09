#include "TES3Enchantment.h"

#include "BitUtil.h"

namespace TES3 {
	const auto TES3_Enchantment_ctor = reinterpret_cast< void( __thiscall * )( Enchantment * ) >( 0x4AADA0 );
	Enchantment::Enchantment() :
		objectID{},
		castType{},
		chargeCost{},
		maxCharge{},
		pad_32{},
		effects{},
		flags{}
	{
		TES3_Enchantment_ctor( this );
	}

	const auto TES3_Enchantment_dtor = reinterpret_cast< void( __thiscall * )( Enchantment * ) >( 0x4AAEA0 );
	Enchantment::~Enchantment()
	{
		TES3_Enchantment_dtor( this );
	}

	bool Enchantment::getSpellFlag(EnchantmentFlag::Flag flag) const {
		return BITMASK_TEST(flags, flag);
	}

	void Enchantment::setSpellFlag(EnchantmentFlag::Flag flag, bool value) {
		BITMASK_SET(flags, flag, value);
	}

	bool Enchantment::getAutoCalc() const {
		return getSpellFlag(EnchantmentFlag::AutoCalc);
	}

	void Enchantment::setAutoCalc(bool value) {
		setSpellFlag(EnchantmentFlag::AutoCalc, value);
	}

	size_t Enchantment::getActiveEffectCount() const {
		size_t count = 0;
		for (size_t i = 0; i < 8; ++i) {
			if (effects[i].effectID != TES3::EffectID::None) {
				count++;
			}
		}
		return count;
	}

	int Enchantment::getFirstIndexOfEffect(int effectId) const {
		for (size_t i = 0; i < 8; ++i) {
			if (effects[i].effectID == effectId) {
				return i;
			}
		}
		return -1;
	}

	bool Enchantment::hasEffect(int effectId) const {
		return getFirstIndexOfEffect(effectId) != -1;
	}

	std::reference_wrapper<Effect[8]> Enchantment::getEffects() {
		return std::ref(effects);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Enchantment)
