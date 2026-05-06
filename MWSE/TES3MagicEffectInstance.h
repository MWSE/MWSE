#pragma once

#include "TES3Defines.h"

namespace TES3 {
	struct MagicEffectInstance {
		union CreatedDataVariant {
			EquipmentStack* equipmentOrSummon;
			int vampireHeadId;
		};

		Reference * target; // 0x0
		float resistedPercent; // 0x4
		int magnitude; // 0x8
		float timeActive; // 0xC
		float cumulativeMagnitude; // 0x10
		int state; // 0x14
		VFX* visual; // 0x18
		CreatedDataVariant createdData; // 0x1C
		EquipmentStack* createdData2; // 0x20
		EquipmentStack* lastUsedWeapon; // 0x24
		EquipmentStack* lastUsedArmor; // 0x28
		EquipmentStack* lastUsedShield; // 0x2C
		EquipmentStack* lastUsedLight; // 0x30
		EquipmentStack* lastUsedEnchItem; // 0x34

		MagicEffectInstance() = delete;
		// Destructible for template interoperability; instances remain engine-owned.
		~MagicEffectInstance() = default;

		EquipmentStack* getCreatedStack() const;
		float getEffectiveMagnitude() const;
		bool isBoundItem() const;
		bool isSummon() const;
	};
	static_assert(sizeof(MagicEffectInstance) == 0x38, "TES3::MagicEffectInstance failed size validation");
}
