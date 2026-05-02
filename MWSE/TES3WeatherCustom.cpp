#include "TES3WeatherCustom.h"

#include "TES3Sound.h"
#include "TES3WeatherController.h"

#include "Log.h"
#include "LuaManager.h"
#include "LuaUtil.h"

namespace TES3 {
	Weather_vTable WeatherCustom::VirtualTable;

	WeatherCustom::WeatherCustom() : Weather() {
		vTable = &VirtualTable;

		windJitterScalar = 1.0f;

		supportsParticleLerping = false;
		supportsAshCloud = false;
		supportsBlightCloud = false;
		supportsBlizzard = false;
	}

	WeatherCustom::WeatherCustom(WeatherController* wc) : WeatherCustom() {
		controller = wc;
	}

	WeatherCustom::~WeatherCustom() {

	}

	void WeatherCustom::vtbl_delete(bool del) {
		if (del) {
			delete this;
		}
		else {
			this->~WeatherCustom();
		}
	}

	void WeatherCustom::vtbl_simulate(float transitionScalar, float deltaTime) {
		updateCloudWind();
		updateAmbientSound(transitionScalar);

		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		if (simulateFunction.valid()) {
			auto& state = stateHandle.getState();
			auto e = state.create_table();
			e["weather"] = this;
			e["deltaTime"] = deltaTime;
			sol::protected_function_result result = simulateFunction(e);
			if (!result.valid()) {
				sol::error error = result;
				mwse::log::getLog() << "Lua error encountered in custom weather simulate function:" << std::endl << error.what() << std::endl;
				mwse::lua::reportErrorInGame("custom weather simulate", error);
			}
		}
	}

	void WeatherCustom::vtbl_transition() {
		const auto relevance = getRelevance();
		if (soundAmbientLoop && soundAmbientLoop->isPlaying()) {
			soundAmbientLoop->setVolumeRaw(controller ? controller->getWeatherScaledVolume(relevance) : 0);
			updateUnderwaterFrequency();
		}

		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		if (transitionFunction.valid()) {
			auto direction = 1;
			if (controller && controller->currentWeather == controller->nextWeather) {
				direction = 0;
			}
			else if (controller && controller->currentWeather == this) {
				direction = -1;
			}

			Weather* otherWeather = this;
			if (controller && controller->nextWeather != this) {
				otherWeather = controller->nextWeather;
			}
			else if (controller && controller->currentWeather != this) {
				otherWeather = controller->currentWeather;
			}

			auto& state = stateHandle.getState();
			auto e = state.create_table();
			e["weather"] = this;
			e["direction"] = direction;
			e["otherWeather"] = otherWeather;
			sol::protected_function_result result = transitionFunction(e);
			if (!result.valid()) {
				sol::error error = result;
				mwse::log::getLog() << "Lua error encountered in custom weather transition function:" << std::endl << error.what() << std::endl;
				mwse::lua::reportErrorInGame("custom weather transition", error);
			}
		}

		// Only update the underwater sound state at the very end of simulation/transition.
		if (controller) {
			underwaterSoundState = controller->underwaterPitchbendState;
		}
	}

	void WeatherCustom::vtbl_unload() {
		ambientPlaying = false;
		if (soundAmbientLoop) {
			soundAmbientLoop->stop();
			soundAmbientLoop->release();
		}

		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		if (unloadFunction.valid()) {
			auto& state = stateHandle.getState();
			auto e = state.create_table();
			e["weather"] = this;
			sol::protected_function_result result = unloadFunction(e);
			if (!result.valid()) {
				sol::error error = result;
				mwse::log::getLog() << "Lua error encountered in custom weather unload function:" << std::endl << error.what() << std::endl;
				mwse::lua::reportErrorInGame("custom weather unload", error);
			}
		}
	}
}
