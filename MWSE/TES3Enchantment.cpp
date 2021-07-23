#include "TES3Enchantment.h"

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

	size_t Enchantment::getActiveEffectCount() {
		size_t count = 0;
		for (size_t i = 0; i < 8; i++) {
			if (effects[i].effectID != TES3::EffectID::None) {
				count++;
			}
		}
		return count;
	}

	int Enchantment::getFirstIndexOfEffect(int effectId) {
		for (size_t i = 0; i < 8; i++) {
			if (effects[i].effectID == effectId) {
				return i;
			}
		}
		return -1;
	}

	std::reference_wrapper<Effect[8]> Enchantment::getEffects() {
		return std::ref(effects);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Enchantment)
