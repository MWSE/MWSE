#include "MWSEInterface.h"

#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

namespace mwse {
	TES3::Weather* getWeather(int weatherID) {
		const auto worldController = TES3::WorldController::get();
		const auto weatherController = worldController ? worldController->weatherController : nullptr;
		return weatherController ? weatherController->getWeather(weatherID) : nullptr;
	}

	MWSEAPIv1 api;

	int MWSEAPIv1::getAPIVersion() const {
		return 1;
	}

	int MWSEAPIv1::getWeatherCurrent() const {
		const auto worldController = TES3::WorldController::get();
		const auto weatherController = worldController ? worldController->weatherController : nullptr;
		const auto weather = weatherController ? weatherController->currentWeather : nullptr;
		return weather ? weather->index : -1;
	}

	int MWSEAPIv1::getWeatherNext() const {
		const auto worldController = TES3::WorldController::get();
		const auto weatherController = worldController ? worldController->weatherController : nullptr;
		const auto weather = weatherController ? weatherController->nextWeather : nullptr;
		return weather ? weather->index : -1;
	}

	float MWSEAPIv1::getWeatherLerp() const {
		const auto worldController = TES3::WorldController::get();
		const auto weatherController = worldController ? worldController->weatherController : nullptr;
		const auto weather = weatherController ? weatherController->currentWeather : nullptr;
		return weather ? weather->getRelevance() : 0.0f;
	}

	bool MWSEAPIv1::getWeatherExists(int weatherID) const {
		const auto worldController = TES3::WorldController::get();
		const auto weatherController = worldController ? worldController->weatherController : nullptr;
		const auto weather = weatherController ? weatherController->getWeather(weatherID) : nullptr;
		return weather != nullptr;
	}

	bool MWSEAPIv1::getWeatherHasFog(int weatherID) const {
		const auto worldController = TES3::WorldController::get();
		const auto weatherController = worldController ? worldController->weatherController : nullptr;
		const auto weather = weatherController ? weatherController->getWeather(weatherID) : nullptr;
		return weather ? weather->getHasFog() : false;
	}

	float MWSEAPIv1::getWeatherRippleFactor(int weatherID) const {
		const auto worldController = TES3::WorldController::get();
		const auto weatherController = worldController ? worldController->weatherController : nullptr;
		const auto weather = weatherController ? weatherController->getWeather(weatherID) : nullptr;
		return weather ? weather->getRippleFactor() : -1.5f;
	}

	MWSEAPI* getInterface(int version) {
		if (version == 1) {
			return &api;
		}

		return nullptr;
	}
}
