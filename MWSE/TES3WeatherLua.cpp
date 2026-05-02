#include "TES3WeatherLua.h"

#include "LuaManager.h"

#include "TES3Sound.h"
#include "TES3Spell.h"
#include "TES3Weather.h"
#include "TES3WeatherAsh.h"
#include "TES3WeatherBlight.h"
#include "TES3WeatherBlizzard.h"
#include "TES3WeatherClear.h"
#include "TES3WeatherCloudy.h"
#include "TES3WeatherController.h"
#include "TES3WeatherFoggy.h"
#include "TES3WeatherOvercast.h"
#include "TES3WeatherRain.h"
#include "TES3WeatherSnow.h"
#include "TES3WeatherThunder.h"

namespace mwse::lua {
	void bindTES3Weather() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::Weather
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Weather>("tes3weather");
			usertypeDefinition["new"] = sol::no_constructor;

			setUserdataForTES3Weather(usertypeDefinition);
		}


		// Binding for TES3::WeatherAsh
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WeatherAsh>("tes3weatherAsh");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Weather>();
			setUserdataForTES3Weather(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["stormOrigin"] = &TES3::WeatherAsh::stormOrigin;
			usertypeDefinition["threshold"] = &TES3::WeatherAsh::stormThreshold;
		}

		// Binding for TES3::WeatherBlight
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WeatherBlight>("tes3weatherBlight");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Weather>();
			setUserdataForTES3Weather(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["blightDiseaseChance"] = &TES3::WeatherBlight::diseaseChance;
			usertypeDefinition["blightDiseases"] = sol::readonly_property(&TES3::WeatherBlight::blightDiseases);
			usertypeDefinition["stormOrigin"] = &TES3::WeatherBlight::stormOrigin;
			usertypeDefinition["threshold"] = &TES3::WeatherBlight::stormThreshold;
		}

		// Binding for TES3::WeatherBlizzard
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WeatherBlizzard>("tes3weatherBlizzard");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Weather>();
			setUserdataForTES3Weather(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["threshold"] = &TES3::WeatherBlizzard::stormThreshold;
		}

		// Binding for TES3::WeatherClear
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WeatherClear>("tes3weatherClear");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Weather>();
			setUserdataForTES3Weather(usertypeDefinition);
		}

		// Binding for TES3::WeatherCloudy
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WeatherCloudy>("tes3weatherCloudy");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Weather>();
			setUserdataForTES3Weather(usertypeDefinition);
		}

		// Binding for TES3::WeatherFoggy
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WeatherFoggy>("tes3weatherFoggy");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Weather>();
			setUserdataForTES3Weather(usertypeDefinition);
		}

		// Binding for TES3::WeatherOvercast
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WeatherOvercast>("tes3weatherOvercast");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Weather>();
			setUserdataForTES3Weather(usertypeDefinition);
		}

		// Binding for TES3::WeatherRain
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WeatherRain>("tes3weatherRain");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Weather>();
			setUserdataForTES3Weather(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["maxParticles"] = &TES3::WeatherRain::raindropsMax;
			usertypeDefinition["particleEntranceSpeed"] = &TES3::WeatherRain::rainEntranceSpeed;
			usertypeDefinition["particleHeightMax"] = &TES3::WeatherRain::rainHeightMax;
			usertypeDefinition["particleHeightMin"] = &TES3::WeatherRain::rainHeightMin;
			usertypeDefinition["particleRadius"] = &TES3::WeatherRain::rainRadius;
			usertypeDefinition["rainActive"] = sol::readonly_property(&TES3::WeatherRain::rainPlaying);
			usertypeDefinition["rainLoopSound"] = &TES3::WeatherRain::soundRainLoop;
			usertypeDefinition["rainLoopSoundId"] = sol::property(&TES3::WeatherRain::soundIDRainLoop, &TES3::WeatherRain::setRainLoopSoundID);
			usertypeDefinition["threshold"] = &TES3::WeatherRain::rainThreshold;
		}

		// Binding for TES3::WeatherSnow
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WeatherSnow>("tes3weatherSnow");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Weather>();
			setUserdataForTES3Weather(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["maxParticles"] = &TES3::WeatherSnow::snowflakesMax;
			usertypeDefinition["particleEntranceSpeed"] = &TES3::WeatherSnow::snowEntranceSpeed;
			usertypeDefinition["particleHeightMax"] = &TES3::WeatherSnow::snowHeightMax;
			usertypeDefinition["particleHeightMin"] = &TES3::WeatherSnow::snowHeightMin;
			usertypeDefinition["particleRadius"] = &TES3::WeatherSnow::snowRadius;
			usertypeDefinition["threshold"] = &TES3::WeatherSnow::snowThreshold;
		}

		// Binding for TES3::WeatherThunder
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WeatherThunder>("tes3weatherThunder");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Weather>();
			setUserdataForTES3Weather(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["flashDecrement"] = &TES3::WeatherThunder::flashDecrement;
			usertypeDefinition["maxParticles"] = &TES3::WeatherThunder::raindropsMax;
			usertypeDefinition["particleEntranceSpeed"] = &TES3::WeatherThunder::rainEntranceSpeed;
			usertypeDefinition["particleHeightMax"] = &TES3::WeatherThunder::rainHeightMax;
			usertypeDefinition["particleHeightMin"] = &TES3::WeatherThunder::rainHeightMin;
			usertypeDefinition["particleRadius"] = &TES3::WeatherThunder::rainRadius;
			usertypeDefinition["rainActive"] = sol::readonly_property(&TES3::WeatherThunder::rainPlaying);
			usertypeDefinition["rainLoopSound"] = &TES3::WeatherThunder::soundRainLoop;
			usertypeDefinition["rainLoopSoundId"] = sol::property(&TES3::WeatherThunder::soundIDRainLoop, &TES3::WeatherThunder::setRainLoopSoundID);
			usertypeDefinition["threshold"] = &TES3::WeatherThunder::rainThreshold;
			usertypeDefinition["thunderFrequency"] = &TES3::WeatherThunder::thunderFrequency;
			usertypeDefinition["thunderSound1"] = &TES3::WeatherThunder::thunderSound1;
			usertypeDefinition["thunderSound1Id"] = sol::property(&TES3::WeatherThunder::soundIDThunder1, &TES3::WeatherThunder::setThunder1SoundID);
			usertypeDefinition["thunderSound2"] = &TES3::WeatherThunder::thunderSound2;
			usertypeDefinition["thunderSound2Id"] = sol::property(&TES3::WeatherThunder::soundIDThunder2, &TES3::WeatherThunder::setThunder2SoundID);
			usertypeDefinition["thunderSound3"] = &TES3::WeatherThunder::thunderSound3;
			usertypeDefinition["thunderSound3Id"] = sol::property(&TES3::WeatherThunder::soundIDThunder3, &TES3::WeatherThunder::setThunder3SoundID);
			usertypeDefinition["thunderSound4"] = &TES3::WeatherThunder::thunderSound4;
			usertypeDefinition["thunderSound4Id"] = sol::property(&TES3::WeatherThunder::soundIDThunder4, &TES3::WeatherThunder::setThunder4SoundID);
			usertypeDefinition["thunderSoundCount"] = &TES3::WeatherThunder::thunderSoundCount;
			usertypeDefinition["thunderThreshold"] = &TES3::WeatherThunder::thunderThreshold;
		}
	}
}
