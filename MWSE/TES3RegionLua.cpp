#include "TES3RegionLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3LeveledList.h"
#include "TES3Region.h"
#include "TES3Sound.h"
#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

namespace mwse::lua {
	void bindTES3Region() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::RegionSound
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::RegionSound>("tes3regionSound");
			usertypeDefinition["new"] = sol::no_constructor;

			// Access to other objects that need to be packaged.
			usertypeDefinition["sound"] = &TES3::RegionSound::sound;

			// Restrict chance [0-100].
			usertypeDefinition["chance"] = sol::property(&TES3::RegionSound::chance, &TES3::RegionSound::setChance);
		}

		// Binding for TES3::Region.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Region>("tes3region");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
			setUserdataForTES3BaseObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["name"] = sol::property(&TES3::Region::getName, &TES3::Region::setName);
			usertypeDefinition["sleepCreature"] = sol::readonly_property(&TES3::Region::sleepCreature);
			usertypeDefinition["sounds"] = sol::readonly_property(&TES3::Region::sounds);

			// Indirect bindings to unions and arrays.
			usertypeDefinition["weatherChances"] = sol::readonly_property(&TES3::Region::getWeatherChances);

			// User-friendly access to weather chances.
			usertypeDefinition["weatherChanceAsh"] = sol::property(&TES3::Region::getWeatherChanceAsh, &TES3::Region::setWeatherChanceAsh);
			usertypeDefinition["weatherChanceBlight"] = sol::property(&TES3::Region::getWeatherChanceBlight, &TES3::Region::setWeatherChanceBlight);
			usertypeDefinition["weatherChanceBlizzard"] = sol::property(&TES3::Region::getWeatherChanceBlizzard, &TES3::Region::setWeatherChanceBlizzard);
			usertypeDefinition["weatherChanceClear"] = sol::property(&TES3::Region::getWeatherChanceClear, &TES3::Region::setWeatherChanceClear);
			usertypeDefinition["weatherChanceCloudy"] = sol::property(&TES3::Region::getWeatherChanceCloudy, &TES3::Region::setWeatherChanceCloudy);
			usertypeDefinition["weatherChanceFoggy"] = sol::property(&TES3::Region::getWeatherChanceFoggy, &TES3::Region::setWeatherChanceFoggy);
			usertypeDefinition["weatherChanceOvercast"] = sol::property(&TES3::Region::getWeatherChanceOvercast, &TES3::Region::setWeatherChanceOvercast);
			usertypeDefinition["weatherChanceRain"] = sol::property(&TES3::Region::getWeatherChanceRain, &TES3::Region::setWeatherChanceRain);
			usertypeDefinition["weatherChanceSnow"] = sol::property(&TES3::Region::getWeatherChanceSnow, &TES3::Region::setWeatherChanceSnow);
			usertypeDefinition["weatherChanceThunder"] = sol::property(&TES3::Region::getWeatherChanceThunder, &TES3::Region::setWeatherChanceThunder);

			// Expose the current weather, and allow it to be changed via setting.
			usertypeDefinition["weather"] = sol::property(&TES3::Region::getCurrentWeather, &TES3::Region::setCurrentWeather_lua);

			// Basic function binding.
			usertypeDefinition["changeWeather"] = &TES3::Region::setCurrentWeather_lua;
			usertypeDefinition["randomizeWeather"] = &TES3::Region::randomizeWeather;
		}
	}
}
