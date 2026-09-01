#pragma once

#include "CSPhysicalObject.h"
#include "CSWearablePart.h"

#include "TES3ArmorFlags.h"

namespace se::cs {
	struct Armor : PhysicalObject {
		char* name; // 0x48
		char* model; // 0x4C
		Script* script; // 0x50
		char* icon; // 0x54
		WearablePart parts[7]; // 0x58
		TES3::ArmorSlot::ArmorSlot slot; // 0xAC
		float weight; // 0xB0
		int value; // 0xB4
		int maxCondition; // 0xB8
		int enchantPoints; // 0xBC
		int armorRating; // 0xC0
		Enchantment* enchantment; // 0xC4

		TES3::ArmorWeightClass::ArmorWeightClass getWeightClass() const;
		const char* getWeightClassName() const;
	};
	static_assert(sizeof(Armor) == 0xC8, "Armor failed size validation");
}
