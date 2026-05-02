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

#include "RngUtil.h"

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

		if (isCustomWeather()) {
			return static_cast<const WeatherCustom*>(this)->name.c_str();
		}

		return nullptr;
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

	bool Weather::getSupportsPrecipitationType(int type) const {
		switch (type) {
		case 1: return getSupportsRain();
		case 5: return getSupportsSnow();
		}

		return false;
	}

	float Weather::getPrecipitationThreshold(int type) const {
		switch (type) {
		case 1: return getRainThreshold();
		case 5: return getSnowThreshold();
		}

		return IMPOSSIBLE_THRESHOLD;
	}

	float Weather::getPrecipitationBlend(int type, float transitionScalar) const {
		if (!controller) {
			return 0.0f;
		}

		const auto currentWeather = controller->currentWeather;
		const auto nextWeather = controller->nextWeather;
		const auto currentSupportsPrecipitation = currentWeather ? currentWeather->getSupportsPrecipitationType(type) : false;
		const auto nextSupportsPrecipitation = nextWeather ? nextWeather->getSupportsPrecipitationType(type) : false;

		if (controller->transitionScalar <= 0.0f || controller->transitionScalar >= 1.0f || (currentSupportsPrecipitation && nextSupportsPrecipitation)) {
			return std::clamp(transitionScalar, 0.0f, 1.0f);
		}

		const auto precipitationThreshold = getPrecipitationThreshold(type);
		if (precipitationThreshold == IMPOSSIBLE_THRESHOLD) {
			return 0.0f;
		}
		return std::clamp((transitionScalar - precipitationThreshold) / (1.0f - precipitationThreshold), 0.0f, 1.0f);
	}

	int Weather::getPrecipitationParticleTarget(int type, float transitionScalar) const {
		if (!controller) {
			return 0;
		}

		const auto currentWeather = controller->currentWeather;
		const auto nextWeather = controller->nextWeather;
		const auto currentSupportsPrecipitation = currentWeather ? currentWeather->getSupportsPrecipitationType(type) : false;
		const auto nextSupportsPrecipitation = nextWeather ? nextWeather->getSupportsPrecipitationType(type) : false;
		const auto precipitationMax = getPrecipitationMax(type);

		if (controller->transitionScalar <= 0.0f
			|| controller->transitionScalar >= 1.0f
			|| (currentSupportsPrecipitation && nextSupportsPrecipitation)) {
			return static_cast<int>(precipitationMax);
		}

		const auto precipitationThreshold = getPrecipitationThreshold(type);
		if (precipitationThreshold == IMPOSSIBLE_THRESHOLD) {
			return 0;
		}

		if (currentSupportsPrecipitation && currentWeather && currentWeather->getRelevance() >= precipitationThreshold) {
			const auto target = static_cast<int>(std::floor((1.0f - controller->transitionScalar) * precipitationMax));
			return std::max(target, 0);
		}

		if (nextSupportsPrecipitation && nextWeather && nextWeather->getRelevance() >= precipitationThreshold) {
			const auto target = static_cast<int>(std::floor(controller->transitionScalar * precipitationMax));
			return std::max(target, 0);
		}

		return 0;
	}

	float Weather::getRainThreshold() const {
		switch (index) {
		case WeatherType::Rain:
			return static_cast<const WeatherRain*>(this)->rainThreshold;
		case WeatherType::Thunder:
			return static_cast<const WeatherThunder*>(this)->rainThreshold;
		default:
			if (isCustomWeather()) {
				return static_cast<const WeatherCustom*>(this)->rainThreshold.value_or(IMPOSSIBLE_THRESHOLD);
			}
			return IMPOSSIBLE_THRESHOLD;
		}
	}

	float Weather::getStormThreshold() const {
		switch (index) {
		case WeatherType::Ash:
			return static_cast<const WeatherAsh*>(this)->stormThreshold;
		case WeatherType::Blight:
			return static_cast<const WeatherBlight*>(this)->stormThreshold;
		case WeatherType::Blizzard:
			return static_cast<const WeatherBlizzard*>(this)->stormThreshold;
		default:
			if (isCustomWeather()) {
				return static_cast<const WeatherCustom*>(this)->stormThreshold.value_or(IMPOSSIBLE_THRESHOLD);
			}
			return IMPOSSIBLE_THRESHOLD;
		}
	}

	float Weather::getSnowThreshold() const {
		switch (index) {
		case WeatherType::Snow:
			return static_cast<const WeatherSnow*>(this)->snowThreshold;
		default:
			if (isCustomWeather()) {
				return static_cast<const WeatherCustom*>(this)->snowThreshold.value_or(IMPOSSIBLE_THRESHOLD);
			}
			return IMPOSSIBLE_THRESHOLD;
		}
	}

	float Weather::getRaindropsMax() const {
		switch (index) {
		case WeatherType::Rain:
			return static_cast<const WeatherRain*>(this)->raindropsMax;
		case WeatherType::Thunder:
			return static_cast<const WeatherThunder*>(this)->raindropsMax;
		default:
			if (isCustomWeather()) {
				return static_cast<const WeatherCustom*>(this)->raindropsMax.value_or(0.0f);
			}
			return 0.0f;
		}
	}

	float Weather::getSnowflakesMax() const {
		switch (index) {
		case WeatherType::Snow:
			return static_cast<const WeatherSnow*>(this)->snowflakesMax;
		default:
			if (isCustomWeather()) {
				return static_cast<const WeatherCustom*>(this)->snowflakesMax.value_or(0.0f);
			}
			return 0.0f;
		}
	}

	float Weather::getPrecipitationMax() const {
		return std::max(getRaindropsMax(), getSnowflakesMax());
	}

	float Weather::getPrecipitationMax(int type) const {
		switch (type) {
		case 1: return getRaindropsMax();
		case 5: return getSnowflakesMax();
		}

		return 0.0f;
	}

	float Weather::getRelevance() const {
		if (!controller) {
			return 0.0f;
		}

		if (controller->currentWeather == this) {
			if (!controller->nextWeather || controller->nextWeather->index == WEATHER_ID_INVALID) {
				return 1.0f;
			}

			if (controller->nextWeather == this) {
				return 1.0f;
			}

			return std::clamp(1.0f - controller->transitionScalar, 0.0f, 1.0f);
		}

		if (controller->nextWeather == this) {
			return std::clamp(controller->transitionScalar, 0.0f, 1.0f);
		}

		return 0.0f;
	}

	bool Weather::getSupportsRain() const {
		switch (index) {
		case WeatherType::Rain:
		case WeatherType::Thunder:
			return true;
		default:
			if (isCustomWeather()) {
				return static_cast<const WeatherCustom*>(this)->supportsRain;
			}
			return false;
		}
	}

	bool Weather::getSupportsAshCloud() const {
		switch (index) {
		case WeatherType::Ash:
			return true;
		default:
			if (isCustomWeather()) {
				return static_cast<const WeatherCustom*>(this)->supportsAshCloud;
			}
			return false;
		}
	}

	bool Weather::getSupportsBlightCloud() const {
		switch (index) {
		case WeatherType::Blight:
			return true;
		default:
			if (isCustomWeather()) {
				return static_cast<const WeatherCustom*>(this)->supportsBlightCloud;
			}
			return false;
		}
	}

	bool Weather::getSupportsSnow() const {
		switch (index) {
		case WeatherType::Snow:
			return true;
		default:
			if (isCustomWeather()) {
				return static_cast<const WeatherCustom*>(this)->supportsSnow;
			}
			return false;
		}
	}

	bool Weather::getSupportsBlizzard() const {
		switch (index) {
		case WeatherType::Blizzard:
			return true;
		default:
			if (isCustomWeather()) {
				return static_cast<const WeatherCustom*>(this)->supportsBlizzard;
			}
			return false;
		}
	}

	float Weather::getWindJitter() const {
		switch (index) {
		case WeatherType::Rain: return 0.5f;
		case WeatherType::Thunder: return 0.5f;
		}

		if (isCustomWeather()) {
			return static_cast<const WeatherCustom*>(this)->windJitterScalar;
		}

		return 1.0f;
	}

	float Weather::calculateNextWindSpeed(float windSpeed, float windJitterScalar, const Vector3& previousVelocity) {
		const auto cappedScaledWindSpeed = std::min(windSpeed * 8.0f, 70.0f);
		auto nextWindSpeed = previousVelocity == Vector3::ZEROES ? cappedScaledWindSpeed : previousVelocity.length();
		if (nextWindSpeed == 0.0f) {
			nextWindSpeed = cappedScaledWindSpeed;
		}

		const auto randomizedWindSpeed = mwse::rng::getRandomFloat(-0.5f, 0.5f) * windJitterScalar * cappedScaledWindSpeed + nextWindSpeed;
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
		const auto nextWindSpeed = calculateNextWindSpeed(windSpeed, getWindJitter(), velocity);

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
		updateLoopSound(soundAmbientLoop, soundIDAmbientLoop, ambientPlaying, controller ? controller->getWeatherScaledVolume(transitionScalar) : 0, transitionScalar >= 0.05f);
	}

	void Weather::updateLoopSound(Sound*& sound, const char* soundId, bool& playing, unsigned char volume, bool shouldPlay) const {
		if (!sound && controller && controller->dataHandler && controller->dataHandler->nonDynamicData) {
			sound = controller->dataHandler->nonDynamicData->findSound(soundId);
		}

		if (!sound) {
			playing = false;
			return;
		}

		sound->loadBuffer(false);
		if (!sound->isPlaying()) {
			sound->setVolumeRaw(volume);
			updateUnderwaterFrequency(sound);
		}

		if (shouldPlay) {
			if (sound->isPlaying()) {
				sound->adjustPlayingSoundVolume(volume);
			}
			else {
				sound->playRaw(SoundPlayFlags::Loop, volume, 1.0f, true);
			}
			updateUnderwaterFrequency(sound);
			playing = true;
		}
		else {
			playing = false;
			if (sound->isPlaying()) {
				sound->stop();
			}
		}
	}

	void Weather::updatePlayingSoundVolume(Sound* sound, unsigned char volume) const {
		if (!sound || !sound->isPlaying()) {
			return;
		}

		sound->adjustPlayingSoundVolume(volume);
		updateUnderwaterFrequency(sound);
	}

	void Weather::updatePrecipitationParticles(int type, float transitionScalar, float deltaTime, float rainRadius, float rainHeightMin, float rainHeightMax, float rainEntranceSpeed) const {
		if (!controller || rainEntranceSpeed <= 0.0f) {
			return;
		}

		const auto rainParticleTarget = getPrecipitationParticleTarget(type, transitionScalar);
		if (rainParticleTarget <= 0) {
			return;
		}
		const auto randomSpawnRoll = mwse::rng::getRandomFloat(0.0f, 1.0f);
		const auto activeRainParticles = controller->getActiveParticleCount(type);
		const auto spawnChance = rainParticleTarget > 0 ? double(rainParticleTarget - activeRainParticles) / double(rainParticleTarget) : 0.0;
		if (randomSpawnRoll > spawnChance) {
			return;
		}

		const auto availableParticles = rainParticleTarget - activeRainParticles;
		const auto spawnBudget = static_cast<double>(availableParticles) * deltaTime / rainEntranceSpeed;
		if (spawnBudget <= 0.0) {
			return;
		}

		auto spawnCount = spawnBudget;
		while (spawnCount > 0.0 && controller->spawnParticle(type, rainRadius, rainHeightMin, rainHeightMax)) {
			--spawnCount;
			if (rainParticleTarget > 0 && controller->getActiveParticleCount(type) >= rainParticleTarget) {
				break;
			}
		}
	}

	void Weather::updateUnderwaterFrequency(Sound* sound) const {
		if (!sound || !sound->soundBuffer) {
			return;
		}

		const auto dataHandler = TES3::DataHandler::get();
		if (controller && controller->underwaterPitchbendState && !underwaterSoundState && dataHandler && dataHandler->waterController) {
			sound->setFrequency(dataHandler->waterController->nearWaterUnderwaterFrequency);
		}
		else if (controller && !controller->underwaterPitchbendState && underwaterSoundState) {
			sound->setFrequency(1.0f);
		}
	}

	void Weather::updateCloudTexture(NI::TriShape* shape) const {
		const auto buffer = mwse::tes3::getThreadSafeStringBuffer();
		if (mwse::tes3::resolveAssetPath(texturePathCloud, buffer) == 0) {
			mwse::tes3::logAndShowError("Weather %s texture not found.", texturePathCloud);
			sprintf(buffer, "%s\\tx_mooncircle_full_M.tga", "Textures");
		}

		auto cloudTexture = NI::SourceTexture::createFromPath(buffer);
		const auto cloudTextureProperty = shape->getTexturingProperty();
		if (!cloudTextureProperty) {
			return;
		}

		if (cloudTextureProperty->getBaseMap() == nullptr) {
			const auto baseMap = new NI::TexturingProperty::Map(cloudTexture);
			baseMap->filterMode = NI::TexturingProperty::FilterMode::BILERP;
			cloudTextureProperty->setBaseMap(baseMap);
		}

		cloudTextureProperty->getBaseMap()->texture = cloudTexture;
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
