#include "TES3WeatherController.h"

#include "TES3DataHandler.h"
#include "TES3GlobalVariable.h"
#include "TES3Region.h"
#include "TES3WeatherAsh.h"
#include "TES3WeatherBlight.h"
#include "TES3WeatherBlizzard.h"
#include "TES3WeatherClear.h"
#include "TES3WeatherCloudy.h"
#include "TES3WeatherFoggy.h"
#include "TES3WeatherOvercast.h"
#include "TES3WeatherRain.h"
#include "TES3WeatherSnow.h"
#include "TES3WeatherThunder.h"
#include "TES3WorldController.h"

#include "NIProperty.h"
#include "NIRenderer.h"

#include "LuaManager.h"
#include "LuaCalcSunDamageScalarEvent.h"
#include "LuaWeatherChangedImmediateEvent.h"
#include "LuaWeatherTransitionFinishedEvent.h"
#include "LuaWeatherTransitionStartedEvent.h"

namespace TES3 {

	const auto TES3_WeatherController_getCurrentWeatherIndex = reinterpret_cast<int(__thiscall*)(const WeatherController*)>(0x4424E0);
	int WeatherController::getCurrentWeatherIndex() const {
		return TES3_WeatherController_getCurrentWeatherIndex(this);
	}

	const auto TES3_WeatherController_calcSunDamageScalar = reinterpret_cast<float(__thiscall*)(WeatherController*)>(0x0440630);
	float WeatherController::calcSunDamageScalar() {
		float damage = TES3_WeatherController_calcSunDamageScalar(this);

		// Trigger calcSunDamageScalar event.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CalcSunDamageScalarEvent(damage));
		if (eventData.valid()) {
			damage = eventData.get_or<float>("damage", damage);
		}

		return damage;
	}

	const auto TES3_WeatherController_switchWeather = reinterpret_cast<void(__thiscall*)(WeatherController*, int, float)>(0x441C40);
	void WeatherController::switchWeather(int weatherId, float startingTransition) {
		TES3_WeatherController_switchWeather(this, weatherId, startingTransition);
	}

	const auto TES3_WeatherController_enableSky = reinterpret_cast<void(__thiscall*)(WeatherController*)>(0x440820);
	void WeatherController::enableSky() {
		TES3_WeatherController_enableSky(this);
	}

	const auto TES3_WeatherController_disableSky = reinterpret_cast<void(__thiscall*)(WeatherController*)>(0x440870);
	void WeatherController::disableSky() {
		TES3_WeatherController_disableSky(this);
	}

	std::reference_wrapper<Weather* [MAX_WEATHER_COUNT]> WeatherController::getWeathers() {
		return std::ref(arrayWeathers);
	}

	Weather* WeatherController::getWeather(int weatherId) const {
		return arrayWeathers[weatherId];
	}

	WeatherClear* WeatherController::getWeatherClear() const {
		return static_cast<WeatherClear*>(arrayWeathers[WeatherType::Clear]);
	}

	WeatherCloudy* WeatherController::getWeatherCloudy() const {
		return static_cast<WeatherCloudy*>(arrayWeathers[WeatherType::Cloudy]);
	}

	WeatherFoggy* WeatherController::getWeatherFoggy() const {
		return static_cast<WeatherFoggy*>(arrayWeathers[WeatherType::Foggy]);
	}

	WeatherOvercast* WeatherController::getWeatherOvercast() const {
		return static_cast<WeatherOvercast*>(arrayWeathers[WeatherType::Overcast]);
	}

	WeatherRain* WeatherController::getWeatherRain() const {
		return static_cast<WeatherRain*>(arrayWeathers[WeatherType::Rain]);
	}

	WeatherThunder* WeatherController::getWeatherThunder() const {
		return static_cast<WeatherThunder*>(arrayWeathers[WeatherType::Thunder]);
	}

	WeatherAsh* WeatherController::getWeatherAsh() const {
		return static_cast<WeatherAsh*>(arrayWeathers[WeatherType::Ash]);
	}

	WeatherBlight* WeatherController::getWeatherBlight() const {
		return static_cast<WeatherBlight*>(arrayWeathers[WeatherType::Blight]);
	}

	WeatherSnow* WeatherController::getWeatherSnow() const {
		return static_cast<WeatherSnow*>(arrayWeathers[WeatherType::Snow]);
	}

	WeatherBlizzard* WeatherController::getWeatherBlizzard() const {
		return static_cast<WeatherBlizzard*>(arrayWeathers[WeatherType::Blizzard]);
	}

	const auto TES3_WeatherController_setBackgroundToFog = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Object*)>(0x43EB20);
	const auto TES3_WeatherController_setFogColour = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Property*)>(0x43EB80);
	const auto TES3_WeatherController_updateAmbient = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43EF80);
	const auto TES3_WeatherController_updateColours = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43E000);
	const auto TES3_WeatherController_updateClouds = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43EC20);
	const auto TES3_WeatherController_updateCloudVertexCols = reinterpret_cast<void(__thiscall*)(WeatherController*)>(0x43EDE0);
	const auto TES3_WeatherController_updateSunCols = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43F5F0);
	const auto TES3_WeatherController_updateSun = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43FF80);
	const auto TES3_WeatherController_updateTick = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Property*, float, bool, float)>(0x440C80);
	void WeatherController::updateVisuals() {
		// Allows weather visuals to update when simulation is paused.
		auto gameHour = WorldController::get()->gvarGameHour->value;
		auto renderer = WorldController::get()->renderer;
		auto fogProperty = static_cast<NI::Property*>(DataHandler::get()->sgFogProperty);

		TES3_WeatherController_updateTick(this, fogProperty, 0.0, true, gameHour);
		TES3_WeatherController_updateClouds(this, 0.0);
		TES3_WeatherController_updateColours(this, gameHour);
		TES3_WeatherController_updateCloudVertexCols(this);
		TES3_WeatherController_updateAmbient(this, gameHour);
		TES3_WeatherController_updateSunCols(this, gameHour);
		TES3_WeatherController_updateSun(this, gameHour);

		// setBackgroundToFog decrements the refCount
		renderer->refCount++;
		TES3_WeatherController_setBackgroundToFog(this, renderer);

		// setFogColour decrements the refCount
		fogProperty->refCount++;
		TES3_WeatherController_setFogColour(this, fogProperty);
	}

	static std::atomic<bool> weatherEventGuard = false;

	void WeatherController::switchImmediate(int weather) {
		if (lastActiveRegion) {
			lastActiveRegion->currentWeatherIndex = weather;
		}
		switchWeather(weather, 1.0f);

		// Fire off the event, after function completes.
		// Prevent recursive triggering of weather change events.
		if (!weatherEventGuard && mwse::lua::event::WeatherChangedImmediateEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();

			weatherEventGuard = true;
			stateHandle.triggerEvent(new mwse::lua::event::WeatherChangedImmediateEvent());
			weatherEventGuard = false;
		}
	}

	void WeatherController::switchTransition(int weather) {
		switchWeather(weather, 0.001f);
		if (lastActiveRegion) {
			lastActiveRegion->currentWeatherIndex = weather;
		}

		// Fire off the event after the transition starts.
		// Prevent recursive triggering of weather change events.
		if (!weatherEventGuard && mwse::lua::event::WeatherTransitionStartedEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();

			weatherEventGuard = true;
			stateHandle.triggerEvent(new mwse::lua::event::WeatherTransitionStartedEvent());
			weatherEventGuard = false;
		}
	}
}