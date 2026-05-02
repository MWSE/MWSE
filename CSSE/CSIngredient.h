#pragma once

#include "CSPhysicalObject.h"

namespace se::cs {
	struct Ingredient : PhysicalObject {
		char name[32]; // 0x48
		char model[32]; // 0x68
		Script* script; // 0x88
		char texture[32]; // 0x8C
		float weight; // 0xAC
		int value; // 0xB0
		int effects[4]; // 0xB4
		int effectSkillIds[4]; // 0xC4
		int effectAttributeIds[4]; // 0xD4

		void getEffectName(char* buffer, size_t bufferSize, int index) const;

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Ingredient) == 0xE4, "Ingredient failed size validation");
}
