#pragma once

#include "LuaUtil.h"

#include "TES3Weather.h"

namespace mwse::lua {
	template <typename T>
	void setUserdataForTES3Weather(sol::usertype<T>& usertypeDefinition) {
		usertypeDefinition["__tojson"] = &TES3::Weather::toJson;

		// Basic property binding.
		usertypeDefinition["ambientDayColor"] = sol::readonly_property(&TES3::Weather::ambientDayCol);
		usertypeDefinition["ambientLoopSound"] = sol::readonly_property(&TES3::Weather::soundAmbientLoop);
		usertypeDefinition["ambientNightColor"] = sol::readonly_property(&TES3::Weather::ambientNightCol);
		usertypeDefinition["ambientPlaying"] = sol::readonly_property(&TES3::Weather::ambientPlaying);
		usertypeDefinition["ambientSunriseColor"] = sol::readonly_property(&TES3::Weather::ambientSunriseCol);
		usertypeDefinition["ambientSunsetColor"] = sol::readonly_property(&TES3::Weather::ambientSunsetCol);
		usertypeDefinition["cloudsMaxPercent"] = &TES3::Weather::cloudsMaxPercent;
		usertypeDefinition["cloudsSpeed"] = &TES3::Weather::cloudsSpeed;
		usertypeDefinition["controller"] = sol::readonly_property(&TES3::Weather::controller);
		usertypeDefinition["fogDayColor"] = sol::readonly_property(&TES3::Weather::fogDayCol);
		usertypeDefinition["fogNightColor"] = sol::readonly_property(&TES3::Weather::fogNightCol);
		usertypeDefinition["fogSunriseColor"] = sol::readonly_property(&TES3::Weather::fogSunriseCol);
		usertypeDefinition["fogSunsetColor"] = sol::readonly_property(&TES3::Weather::fogSunsetCol);
		usertypeDefinition["glareView"] = &TES3::Weather::glareView;
		usertypeDefinition["index"] = sol::readonly_property(&TES3::Weather::index);
		usertypeDefinition["landFogDayDepth"] = &TES3::Weather::landFogDayDepth;
		usertypeDefinition["landFogNightDepth"] = &TES3::Weather::landFogNightDepth;
		usertypeDefinition["name"] = sol::readonly_property(&TES3::Weather::getName);
		usertypeDefinition["skyDayColor"] = sol::readonly_property(&TES3::Weather::skyDayCol);
		usertypeDefinition["skyNightColor"] = sol::readonly_property(&TES3::Weather::skyNightCol);
		usertypeDefinition["skySunriseColor"] = sol::readonly_property(&TES3::Weather::skySunriseCol);
		usertypeDefinition["skySunsetColor"] = sol::readonly_property(&TES3::Weather::skySunsetCol);
		usertypeDefinition["sunDayColor"] = sol::readonly_property(&TES3::Weather::sunDayCol);
		usertypeDefinition["sundiscSunsetColor"] = sol::readonly_property(&TES3::Weather::sundiscSunsetCol);
		usertypeDefinition["sunNightColor"] = sol::readonly_property(&TES3::Weather::sunNightCol);
		usertypeDefinition["sunSunriseColor"] = sol::readonly_property(&TES3::Weather::sunSunriseCol);
		usertypeDefinition["sunSunsetColor"] = sol::readonly_property(&TES3::Weather::sunSunsetCol);
		usertypeDefinition["transitionDelta"] = &TES3::Weather::transitionDelta;
		usertypeDefinition["underwaterSoundState"] = sol::readonly_property(&TES3::Weather::underwaterSoundState);
		usertypeDefinition["windSpeed"] = &TES3::Weather::windSpeed;

		// Binding for IDs and paths.
		usertypeDefinition["ambientLoopSoundId"] = sol::property(&TES3::Weather::getAmbientLoopSoundID, &TES3::Weather::setAmbientLoopSoundID);
		usertypeDefinition["cloudTexture"] = sol::property(&TES3::Weather::getCloudTexturePath, &TES3::Weather::setCloudTexturePath);
	}

	void bindTES3Weather();
}
