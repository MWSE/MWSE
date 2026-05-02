#pragma once

#include "CSPhysicalObject.h"

#include "CSEffect.h"

namespace se::cs {
	struct Alchemy : PhysicalObject {
		char* name; // 0x48
		Script* script; // 0x4C
		char* model; // 0x50
		char* icon; // 0x54
		float weight; // 0x58
		unsigned short value; // 0x5C
		Effect effects[8]; // 0x60
		unsigned short flags; // 0x120

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Alchemy) == 0x124, "TES3::Alchemy failed size validation");
}
