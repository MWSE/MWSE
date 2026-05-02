#pragma once

#include "CSObject.h"
#include "CSEffect.h"

namespace se::cs {
	struct Enchantment : Object {
		const char* objectID; // 0x28
		short castType; // 0x2C
		short chargeCost; // 0x2E
		short maxCharge; // 0x30
		Effect effects[8]; // 0x34
		unsigned int enchantFlags; // 0xF4
		int useCount; // 0xF8

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Enchantment) == 0xFC, "Enchantment failed size validation");
}
