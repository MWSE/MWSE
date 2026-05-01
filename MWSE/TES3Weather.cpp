#include "TES3Weather.h"

#include "TES3DataHandler.h"
#include "TES3Sound.h"
#include "TES3Util.h"
#include "TES3WaterController.h"
#include "TES3WeatherAsh.h"
#include "TES3WeatherBlight.h"
#include "TES3WeatherBlizzard.h"
#include "TES3WeatherClear.h"
#include "TES3WeatherCloudy.h"
#include "TES3WeatherController.h"
#include "TES3WeatherCustom.h"
#include "TES3WeatherFoggy.h"
#include "TES3WeatherOvercast.h"
#include "TES3WeatherRain.h"
#include "TES3WeatherSnow.h"
#include "TES3WeatherThunder.h"
#include "TES3WorldController.h"

#include "LuaManager.h"

#include "LuaObjectInvalidatedEvent.h"

namespace TES3 {
	//
	// Weather
	//

	Weather::Weather() {
		vTable = (Weather_vTable*)0x746BD4;
		index = WEATHER_ID_INVALID;
		transitionDelta = 0.1f;
		glareView = 1.0f;
		controller = nullptr;
		ambientSunriseCol = { 0.0f, 0.0f, 0.0f };
		ambientDayCol = { 0.0f, 0.0f, 0.0f };
		ambientSunsetCol = { 0.0f, 0.0f, 0.0f };
		ambientNightCol = { 0.0f, 0.0f, 0.0f };
		fogSunriseCol = { 0.0f, 0.0f, 0.0f };
		fogDayCol = { 0.0f, 0.0f, 0.0f };
		fogSunsetCol = { 0.0f, 0.0f, 0.0f };
		fogNightCol = { 0.0f, 0.0f, 0.0f };
		skySunriseCol = { 0.0f, 0.0f, 0.0f };
		skyDayCol = { 0.0f, 0.0f, 0.0f };
		skySunsetCol = { 0.0f, 0.0f, 0.0f };
		skyNightCol = { 0.0f, 0.0f, 0.0f };
		sunSunriseCol = { 0.0f, 0.0f, 0.0f };
		sunDayCol = { 0.0f, 0.0f, 0.0f };
		sunSunsetCol = { 0.0f, 0.0f, 0.0f };
		sunNightCol = { 0.0f, 0.0f, 0.0f };
		sundiscSunsetCol = { 0.0f, 0.0f, 0.0f };
		cloudsMaxPercent = 1.0f;
		landFogDayDepth = 1.0f;
		landFogNightDepth = 1.0f;
		cloudsSpeed = 0.0f;
		windSpeed = 0.0f;
		ZeroMemory(texturePathCloud, sizeof(texturePathCloud));
		ambientPlaying = false;
		underwaterSoundState = false;
		ZeroMemory(soundIDAmbientLoop, sizeof(soundIDAmbientLoop));
		soundAmbientLoop = nullptr;

		unknown_0x208 = 0;
		unknown_0xE0 = 0;
		unknown_0xE4 = { 0, 0, 0 };
	}

	Weather::Weather(WeatherController* wc) : Weather() {
		controller = wc;
	}

	Weather::~Weather() {

	}

	void Weather::simulate(float transitionScalar, float deltaTime) {
		vTable->simulate(this, transitionScalar, deltaTime);
	}

	void Weather::unload() {
		vTable->unload(this);
	}

	void Weather::transition() {
		vTable->transition(this);
	}

	void Weather::vtbl_transition() {

	}

	void Weather::vtbl_unload() {

	}

	std::string Weather::toJson() const {
		std::ostringstream ss;
		ss << "\"tes3weather:" << index << "\"";
		return std::move(ss.str());
	}

	const char* Weather::getName() const {
		switch (index) {
		case WeatherType::Ash: return "Ashstorm";
		case WeatherType::Blight: return "Blight";
		case WeatherType::Blizzard: return "Blizzard";
		case WeatherType::Clear: return "Clear";
		case WeatherType::Cloudy: return "Cloudy";
		case WeatherType::Foggy: return "Foggy";
		case WeatherType::Overcast: return "Overcast";
		case WeatherType::Rain: return "Rain";
		case WeatherType::Snow: return "Snow";
		case WeatherType::Thunder: return "Thunderstorm";
		}

		return static_cast<const WeatherCustom*>(this)->name.c_str();
	}

	const char* Weather::getCloudTexturePath() const {
		return texturePathCloud;
	}

	bool Weather::setCloudTexturePath(const char* path) {
		return strcpy_s(texturePathCloud, sizeof(texturePathCloud), path) == 0;
	}

	const char* Weather::getAmbientLoopSoundID() const {
		return soundIDAmbientLoop;
	}

	bool Weather::setAmbientLoopSoundID(const char* id) {
		if (id == nullptr) {
			soundIDAmbientLoop[0] = 0;
		}
		else if (strcpy_s(soundIDAmbientLoop, sizeof(soundIDAmbientLoop), id) != 0) {
			return false;
		}

		if (soundAmbientLoop) {
			// Stop previous sound.
			if (soundAmbientLoop->isPlaying()) {
				soundAmbientLoop->stop();
				ambientPlaying = false;
			}

			// Clearing the sound pointer will cause the weather code to resolve the new sound id and play it.
			soundAmbientLoop = nullptr;
		}
		return true;
	}

	bool Weather::isCustomWeather() const {
		return index != WEATHER_ID_INVALID && index >= VANILLA_MAX_WEATHER_COUNT;
	}

	bool Weather::supportsParticleLerp() const {
		return index == WeatherType::Rain
			|| index == WeatherType::Thunder
			|| index == WeatherType::Snow
			|| (isCustomWeather() && static_cast<const WeatherCustom*>(this)->supportsParticleLerping);
	}

	float Weather::calculateNextWindSpeed(float windSpeed, const Vector3& previousVelocity) {
		const auto cappedScaledWindSpeed = std::min(windSpeed * 8.0f, 70.0f);
		auto nextWindSpeed = previousVelocity == Vector3::ZEROES ? cappedScaledWindSpeed : previousVelocity.length();
		if (nextWindSpeed == 0.0f) {
			nextWindSpeed = cappedScaledWindSpeed;
		}

		const auto randomValue = mwse::tes3::rand() % 0x7FFF;
		const auto randomizedWindSpeed = ((randomValue * 0.000030518509f) - 0.5f) * cappedScaledWindSpeed + nextWindSpeed;
		if (randomizedWindSpeed > cappedScaledWindSpeed * 0.5f && randomizedWindSpeed < cappedScaledWindSpeed * 2.0f) {
			nextWindSpeed = randomizedWindSpeed;
		}
		return nextWindSpeed;
	}

	void Weather::updateCloudWind() {
		if (!controller) {
			return;
		}

		const auto isCurrentWeather = controller->currentWeather == this;
		auto& velocity = isCurrentWeather ? controller->windVelocityCurrWeather : controller->windVelocityNextWeather;
		const auto nextWindSpeed = calculateNextWindSpeed(windSpeed, velocity);

		Matrix33 cloudRotation;
		cloudRotation.toRotation(0.0f, 0.0f, 0.0f, 1.0f);
		if (isCurrentWeather && controller->sgTriCloudsCurrent) {
			controller->sgTriCloudsCurrent->setLocalRotationMatrix(&cloudRotation);
		}
		else if (controller->sgTriCloudsNext) {
			controller->sgTriCloudsNext->setLocalRotationMatrix(&cloudRotation);
		}

		velocity = { 0.0f, nextWindSpeed, 0.0f };
	}

	void Weather::updateAmbientSound(float transitionScalar) {
		const auto volume = controller ? controller->getWeatherScaledVolume(transitionScalar) : 0;

		if (soundAmbientLoop) {
			soundAmbientLoop->loadBuffer(false);
		}

		if (!soundAmbientLoop && controller && controller->dataHandler) {
			soundAmbientLoop = controller->dataHandler->nonDynamicData->findSound(soundIDAmbientLoop);
			if (soundAmbientLoop) {
				soundAmbientLoop->loadBuffer(false);
				soundAmbientLoop->setVolumeRaw(volume);
				updateUnderwaterFrequency();
			}
		}

		if (transitionScalar >= 0.05f) {
			if (soundAmbientLoop && !soundAmbientLoop->isPlaying()) {
				ambientPlaying = true;
				soundAmbientLoop->playRaw(SoundPlayFlags::Loop, volume, 1.0f, true);
				updateUnderwaterFrequency();
			}
		}
		else {
			ambientPlaying = false;
			if (soundAmbientLoop && soundAmbientLoop->isPlaying()) {
				soundAmbientLoop->stop();
			}
		}
	}

	void Weather::updateUnderwaterFrequency() {
		if (!soundAmbientLoop || !soundAmbientLoop->soundBuffer) {
			return;
		}

		const auto dataHandler = TES3::DataHandler::get();
		if (controller && controller->underwaterPitchbendState && !underwaterSoundState && dataHandler && dataHandler->waterController) {
			soundAmbientLoop->soundBuffer->setFrequency(dataHandler->waterController->nearWaterUnderwaterFrequency);
		}
		else if (controller && !controller->underwaterPitchbendState && underwaterSoundState) {
			soundAmbientLoop->soundBuffer->setFrequency(1.0f);
		}

		if (controller) {
			underwaterSoundState = controller->underwaterPitchbendState;
		}
	}

	static std::unordered_map<const Weather*, sol::object> weatherObjectCache;
	static std::mutex weatherObjectCacheMutex;

	sol::object Weather::getOrCreateLuaObject(lua_State* L) const {
		if (this == nullptr) {
			return sol::nil;
		}

		weatherObjectCacheMutex.lock();

		auto cacheHit = weatherObjectCache.find(this);
		if (cacheHit != weatherObjectCache.end()) {
			auto result = cacheHit->second;
			weatherObjectCacheMutex.unlock();
			return result;
		}

		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		sol::object ref = sol::nil;
		switch (index) {
		case TES3::WeatherType::Ash:
			ref = sol::make_object(L, static_cast<const TES3::WeatherAsh*>(this));
			break;
		case TES3::WeatherType::Blight:
			ref = sol::make_object(L, static_cast<const TES3::WeatherBlight*>(this));
			break;
		case TES3::WeatherType::Blizzard:
			ref = sol::make_object(L, static_cast<const TES3::WeatherBlizzard*>(this));
			break;
		case TES3::WeatherType::Clear:
			ref = sol::make_object(L, static_cast<const TES3::WeatherClear*>(this));
			break;
		case TES3::WeatherType::Cloudy:
			ref = sol::make_object(L, static_cast<const TES3::WeatherCloudy*>(this));
			break;
		case TES3::WeatherType::Foggy:
			ref = sol::make_object(L, static_cast<const TES3::WeatherFoggy*>(this));
			break;
		case TES3::WeatherType::Overcast:
			ref = sol::make_object(L, static_cast<const TES3::WeatherOvercast*>(this));
			break;
		case TES3::WeatherType::Rain:
			ref = sol::make_object(L, static_cast<const TES3::WeatherRain*>(this));
			break;
		case TES3::WeatherType::Snow:
			ref = sol::make_object(L, static_cast<const TES3::WeatherSnow*>(this));
			break;
		case TES3::WeatherType::Thunder:
			ref = sol::make_object(L, static_cast<const TES3::WeatherThunder*>(this));
			break;
		default:
			ref = sol::make_object(L, static_cast<const TES3::WeatherCustom*>(this));
		}

		if (ref != sol::nil) {
			weatherObjectCache[this] = ref;
		}

		weatherObjectCacheMutex.unlock();

		return ref;;
	}

	void Weather::clearCachedLuaObject(const Weather* object) {
		if (!weatherObjectCache.empty()) {
			weatherObjectCacheMutex.lock();

			// Clear any events that make use of this object.
			auto it = weatherObjectCache.find(object);
			if (it != weatherObjectCache.end()) {
				// Let people know that this object is invalidated.
				mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::ObjectInvalidatedEvent(it->second));

				// Clear any events that make use of this object.
				mwse::lua::event::clearObjectFilter(it->second);

				// Remove it from the cache.
				weatherObjectCache.erase(it);
			}

			weatherObjectCacheMutex.unlock();
		}
	}

	void Weather::clearCachedLuaObjects() {
		weatherObjectCacheMutex.lock();
		weatherObjectCache.clear();
		weatherObjectCacheMutex.unlock();
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3_WEATHER(TES3::Weather)
