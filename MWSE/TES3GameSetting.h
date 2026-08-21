#pragma once

#include "TES3Defines.h"
#include "TES3GMST.h"
#include "TES3Object.h"

namespace TES3 {
	// In-application structure used to store default values for GMSTs.
	struct GameSettingInfo {
		char* name; // 0x0
		char* defaultStringValue; // 0x4
		int defaultIntValue; // 0x8
		float defaultFloatValue; // 0xC
		int type; // 0x10

		GameSettingInfo() = delete;
		~GameSettingInfo() = delete;

		//
		// Custom functions.
		//

		static GameSettingInfo * get(int id);

	};

	// The non-static object used at runtime.
	struct GameSetting : BaseObject {
		union {
			long asLong;
			float asFloat;
			char * asString;
		} value; // 0x10
		long index; // 0x14 // Array index of this GMST

		GameSetting();
		~GameSetting();

		//
		// Virtual table overrides.
		//

		char * getObjectID() const;

		//
		// Custom functions.
		//

		GameSettingInfo* getInfo() const;
		char getType() const;
		const char* getName() const;
		const char* getDefaultStringValue() const;
		int getDefaultIntValue() const;
		float getDefaultFloatValue() const;

		std::string toJson() const;

		sol::object getDefaultValue_lua(sol::this_state ts) const;
		sol::object getValue_lua(sol::this_state ts) const;
		void setValue_lua(sol::object value, sol::this_state ts);

	};
	static_assert(sizeof(GameSetting) == 0x18, "TES3::GameSetting failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::GameSetting)
