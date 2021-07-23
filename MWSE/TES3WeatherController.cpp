#include "TES3WeatherController.h"

#include "TES3DataHandler.h"
#include "TES3GlobalVariable.h"
#include "TES3Region.h"
#include "TES3WorldController.h"

#include "NIProperty.h"
#include "NIRenderer.h"

#include "LuaManager.h"
#include "LuaCalcSunDamageScalarEvent.h"

namespace TES3 {
	const auto TES3_WeatherController_calcSunDamageScalar = reinterpret_cast<float(__thiscall*)(WeatherController*)>(0x0440630);
	const auto TES3_WeatherController_setBackgroundToFog = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Object*)>(0x43EB20);
	const auto TES3_WeatherController_setFogColour = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Property*)>(0x43EB80);
	const auto TES3_WeatherController_switch = reinterpret_cast<void(__thiscall*)(WeatherController*, int, float)>(0x441C40);
	const auto TES3_WeatherController_updateAmbient = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43EF80);
	const auto TES3_WeatherController_updateColours = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43E000);
	const auto TES3_WeatherController_updateClouds = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43EC20);
	const auto TES3_WeatherController_updateCloudVertexCols = reinterpret_cast<void(__thiscall*)(WeatherController*)>(0x43EDE0);
	const auto TES3_WeatherController_updateSunCols = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43F5F0);
	const auto TES3_WeatherController_updateSun = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43FF80);
	const auto TES3_WeatherController_updateTick = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Property*, float, bool, float)>(0x440C80);

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

	void WeatherController::switchWeather(int weatherId, float startingTransition) {
		TES3_WeatherController_switch(this, weatherId, startingTransition);
	}

	std::reference_wrapper<Weather*[10]> WeatherController::getWeathers() {
		return std::ref(arrayWeathers);
	}

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

	void WeatherController::switchImmediate(int weather) {
		if (lastActiveRegion) {
			lastActiveRegion->currentWeatherIndex = weather;
		}
		switchWeather(weather, 1.0f);
	}

	void WeatherController::switchTransition(int weather) {
		switchWeather(weather, 0.001f);
		if (lastActiveRegion) {
			lastActiveRegion->currentWeatherIndex = weather;
		}
	}
}