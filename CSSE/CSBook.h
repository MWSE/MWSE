#pragma once

#include "CSPhysicalObject.h"

namespace se::cs {
	struct Book : PhysicalObject {
		const char* name; // 0x48
		Script* script; // 0x4C
		const char* model; // 0x50
		const char* icon; // 0x54
		float weight; // 0x58
		int value; // 0x5C
		bool isScroll; // 0x60
		int skillId; // 0x64
		int enchantPoints; // 0x68
		Object* enchantment; // 0x6C
		char* text; // 0x70

		const char* getTaughtSkillName() const;

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Book) == 0x74, "Book failed size validation");
}
