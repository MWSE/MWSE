#pragma once

#include "CSBaseObject.h"

namespace se::cs {
	struct Faction : BaseObject {
		struct Rank {
			int requiredAttribute[2]; // 0x0
			int requiredSkill[2]; // 0x8
			int reputation; // 0x10
		};
		char objectID[32]; // 0x10
		char name[32]; // 0x30
		char rankNames[10][32]; // 0x50
		int attributes[2]; // 0x190
		Rank ranks[10]; // 0x198
		int skills[7]; // 0x260
		int unknown_0x27C;
		int unknown_0x280;
		int unknown_0x284;
		int unknown_0x288;
		int unknown_0x28C;
		int unknown_0x290;

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Faction) == 0x294, "Faction failed size validation");
}
