#include "CSArmor.h"

namespace se::cs {
	TES3::ArmorWeightClass::ArmorWeightClass Armor::getWeightClass() const {
		const auto Armor_getWeightClass = reinterpret_cast<TES3::ArmorWeightClass::ArmorWeightClass(__thiscall*)(const Armor*)>(0x4032AB);
		return Armor_getWeightClass(this);
	}

	const char* Armor::getWeightClassName() const {
		switch (getWeightClass()) {
		case TES3::ArmorWeightClass::Light:
			return "Light";
		case TES3::ArmorWeightClass::Medium:
			return "Medium";
		case TES3::ArmorWeightClass::Heavy:
			return "Heavy";
		default:
			return "Unknown";
		}
	}
}
