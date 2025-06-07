#pragma once

#include "CSDefines.h"

#include "CSBaseObject.h"

namespace se::cs {
	struct Effect {
		enum class Range : unsigned char {
			Self,
			Touch,
			Target,

			Invalid = 0xFF,
		};
		short effectID; // 0x0
		signed char skillID; // 0x2
		signed char attributeID; // 0x3
		Range rangeType; // 0x4
		int radius; // 0x8
		int duration; // 0xC
		int magnitudeMin; // 0x10
		int magnitudeMax; // 0x14

		MagicEffect* getEffectData() const;
		std::optional<std::string> toString() const;

		bool search(const std::string_view& needle, const BaseObject::SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Effect) == 0x18, "Effect failed size validation");
}
