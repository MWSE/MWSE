#pragma once

#include "LuaUtil.h"

#include "TES3Weather.h"

namespace mwse::lua {
	template <typename T>
	void setUserdataForTES3Weather(sol::usertype<T>& usertypeDefinition) {
		usertypeDefinition["__tojson"] = &TES3::Weather::toJson;

		// Basic property binding.
		usertypeDefinition["ambientDayColor"] = &TES3::Weather::ambientDayCol;
		usertypeDefinition["ambientLoopSound"] = &TES3::Weather::soundAmbientLoop;
		usertypeDefinition["ambientNightColor"] = &TES3::Weather::ambientNightCol;
		usertypeDefinition["ambientPlaying"] = &TES3::Weather::ambientPlaying;
		usertypeDefinition["ambientSunriseColor"] = &TES3::Weather::ambientSunriseCol;
		usertypeDefinition["ambientSunsetColor"] = &TES3::Weather::ambientSunsetCol;
		usertypeDefinition["cloudsMaxPercent"] = &TES3::Weather::cloudsMaxPercent;
		usertypeDefinition["cloudsSpeed"] = &TES3::Weather::cloudsSpeed;
		usertypeDefinition["controller"] = sol::readonly_property(&TES3::Weather::controller);
		usertypeDefinition["fogDayColor"] = &TES3::Weather::fogDayCol;
		usertypeDefinition["fogNightColor"] = &TES3::Weather::fogNightCol;
		usertypeDefinition["fogSunriseColor"] = &TES3::Weather::fogSunriseCol;
		usertypeDefinition["fogSunsetColor"] = &TES3::Weather::fogSunsetCol;
		usertypeDefinition["glareView"] = &TES3::Weather::glareView;
		usertypeDefinition["index"] = sol::readonly_property(&TES3::Weather::index);
		usertypeDefinition["landFogDayDepth"] = &TES3::Weather::landFogDayDepth;
		usertypeDefinition["landFogNightDepth"] = &TES3::Weather::landFogNightDepth;
		usertypeDefinition["name"] = sol::readonly_property(&TES3::Weather::getName);
		usertypeDefinition["skyDayColor"] = &TES3::Weather::skyDayCol;
		usertypeDefinition["skyNightColor"] = &TES3::Weather::skyNightCol;
		usertypeDefinition["skySunriseColor"] = &TES3::Weather::skySunriseCol;
		usertypeDefinition["skySunsetColor"] = &TES3::Weather::skySunsetCol;
		usertypeDefinition["sunDayColor"] = &TES3::Weather::sunDayCol;
		usertypeDefinition["sundiscSunsetColor"] = &TES3::Weather::sundiscSunsetCol;
		usertypeDefinition["sunNightColor"] = &TES3::Weather::sunNightCol;
		usertypeDefinition["sunSunriseColor"] = &TES3::Weather::sunSunriseCol;
		usertypeDefinition["sunSunsetColor"] = &TES3::Weather::sunSunsetCol;
		usertypeDefinition["transitionDelta"] = &TES3::Weather::transitionDelta;
		usertypeDefinition["underwaterSoundState"] = &TES3::Weather::underwaterSoundState;
		usertypeDefinition["windSpeed"] = &TES3::Weather::windSpeed;

		// Binding for IDs and paths.
		usertypeDefinition["ambientLoopSoundId"] = sol::property(&TES3::Weather::getAmbientLoopSoundID, &TES3::Weather::setAmbientLoopSoundID);
		usertypeDefinition["cloudTexture"] = sol::property(&TES3::Weather::getCloudTexturePath, &TES3::Weather::setCloudTexturePath);
	}

	void bindTES3Weather();
}
