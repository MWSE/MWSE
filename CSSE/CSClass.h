#pragma once

#include "CSBaseObject.h"

namespace se::cs {
	struct Class : BaseObject {
		char id[32]; // 0x10
		char name[32]; // 0x30
		int attributes[2]; // 0x50
		int specialization; // 0x58
		int skills[10]; // 0x5C
		int playable; // 0x84
		unsigned int services; // 0x88
		char* description; // 0x8C
		unsigned int descriptionOffset; // 0x90

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Class) == 0x94, "Class failed size validation");
}
