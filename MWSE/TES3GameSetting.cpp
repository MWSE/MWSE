#include "TES3GameSetting.h"

#include "TES3Util.h"

namespace TES3 {
	GameSettingInfo * GameSettingInfo::get(int id) {
		return &reinterpret_cast<TES3::GameSettingInfo*>(0x794800)[id];
	}

	GameSettingInfo * GameSetting::getInfo() const {
		return GameSettingInfo::get(index);
	}

	char * GameSetting::getObjectID() const {
		return getInfo()->name;
	}

	char GameSetting::getType() const {
		// The type is unreliable, instead use the first character of the name.
		return getInfo()->name[0];
	}

	const char* GameSetting::getName() const {
		return getInfo()->name;
	}

	const char* GameSetting::getDefaultStringValue() const {
		return getInfo()->defaultStringValue;
	}

	int GameSetting::getDefaultIntValue() const {
		return getInfo()->defaultIntValue;
	}

	float GameSetting::getDefaultFloatValue() const {
		return getInfo()->defaultFloatValue;
	}

	std::string GameSetting::toJson() const {
		std::ostringstream ss;
		ss << "\"tes3gameSetting:" << getName() << "\"";
		return std::move(ss.str());
	}

	sol::object GameSetting::getDefaultValue_lua(sol::this_state ts) const {
		switch (getType()) {
		case 'i': return sol::make_object(ts, (double)getDefaultIntValue());
		case 'f': return sol::make_object(ts, (double)getDefaultFloatValue());
		case 's': return sol::make_object(ts, getDefaultStringValue());
		}

		return sol::nil;
	}

	sol::object GameSetting::getValue_lua(sol::this_state ts) const {
		switch (getType()) {
		case 'i': return sol::make_object(ts, (double)value.asLong);
		case 'f': return sol::make_object(ts, (double)value.asFloat);
		case 's': return sol::make_object(ts, value.asString);
		}

		return sol::nil;
	}

	void GameSetting::setValue_lua(sol::object v, sol::this_state ts) {
		char type = getType();
		if (type == 's' && v.is<std::string>()) {
			mwse::tes3::setDataString(&value.asString, v.as<std::string>().c_str(), true);
		}
		else if (v.is<double>()) {
			if (type == 'i') {
				value.asLong = v.as<double>();
			}
			else if (type == 'f') {
				value.asFloat = v.as<double>();
			}
		}
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::GameSetting)
