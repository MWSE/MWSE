#pragma once

#include "TES3Defines.h"

namespace TES3 {
	struct MagicEffectInstance {
		Reference * target; // 0x0
		float resistedPercent; // 0x4
		int magnitude; // 0x8
		float timeActive; // 0xC
		float cumulativeMagnitude; // 0x10
		int state; // 0x14
		signed char visual; // 0x18
		ItemStack * createdData; // 0x1C
		int unknown_0x20;
		Weapon * lastUsedWeapon; // 0x24
		Armor * lastUsedArmor; // 0x28
		Armor * lastUsedShield; // 0x2C
		Light * lastUsedLight; // 0x30
		Item * lastUsedEnchItem; // 0x34
	};
	static_assert(sizeof(MagicEffectInstance) == 0x38, "TES3::MagicEffectInstance failed size validation");
}
