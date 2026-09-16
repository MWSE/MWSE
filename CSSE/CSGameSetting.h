#pragma once

#include "CSDefines.h"
#include "CSBaseObject.h"

#include "TES3GMST.h"

namespace se::cs {
	struct GameSettingInitializer {
		enum class ValueType : unsigned int {
			Integer,
			Float,
			String,

			INVALID
		};
		char* name; // 0x0
		char* defaultStringValue; // 0x4
		int defaultIntValue; // 0x8
		float defaultFloatValue; // 0xC
		ValueType valueType; // 0x10

		int getIndex() const;
		ValueType getType() const;
		GameSetting* getSetting() const;

		static std::span<GameSettingInitializer> get();
	};
	static_assert(sizeof(GameSettingInitializer) == 0x14, "GameSettingInitializer failed size validation");

	struct GameSetting : BaseObject {
		union {
			int asInt;
			float asFloat;
			const char* asString;
		} value; // 0x10
		int index; // 0x14

		GameSettingInitializer* getInitializer() const;

		bool search(std::string_view needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(GameSetting) == 0x18, "GameSetting failed size validation");
}
