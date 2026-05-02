#include "TES3WeatherController.h"

#include "TES3AudioController.h"
#include "TES3Cell.h"
#include "TES3DataHandler.h"
#include "TES3GlobalVariable.h"
#include "TES3Region.h"
#include "TES3Spell.h"
#include "TES3WeatherAsh.h"
#include "TES3WeatherBlight.h"
#include "TES3WeatherBlizzard.h"
#include "TES3WeatherClear.h"
#include "TES3WeatherCloudy.h"
#include "TES3WeatherCustom.h"
#include "TES3WeatherFoggy.h"
#include "TES3WeatherOvercast.h"
#include "TES3WeatherRain.h"
#include "TES3WeatherSnow.h"
#include "TES3WeatherThunder.h"
#include "TES3WaterController.h"
#include "TES3WorldController.h"

#include "NIProperty.h"
#include "NIRenderer.h"
#include "NISourceTexture.h"
#include "NITriShape.h"

#include "LuaManager.h"
#include "LuaCalcSunDamageScalarEvent.h"
#include "LuaWeatherChangedImmediateEvent.h"
#include "LuaWeatherTransitionFinishedEvent.h"
#include "LuaWeatherTransitionStartedEvent.h"

#include "MathUtil.h"
#include "MemoryUtil.h"
#include "TES3Util.h"
#include "TES3UIManager.h"

namespace TES3 {
	typedef mwse::ExternalGlobal<bool, 0x785FAC> g_fillSnowParticleVolume;

	static void __fastcall Patch_WeatherController_SwitchWeather_WithEvent(WeatherController* wc, DWORD _EDX_, int weatherId, float startingTransition);

	const auto TES3_WeatherController_ctor = reinterpret_cast<WeatherController*(__thiscall*)(WeatherController*, int)>(0x439F60);
	WeatherController* WeatherController::ctor(int initialWeatherId) {
		// Call vanilla function.
		TES3_WeatherController_ctor(this, initialWeatherId);

#if defined(MWSE_CUSTOM_WEATHERS) && MWSE_CUSTOM_WEATHERS == TRUE
		// Copy created weathers to the new location.
		ZeroMemory(&arrayWeathers, sizeof(arrayWeathers));
		memcpy_s(&arrayWeathers, sizeof(arrayWeathers), &vanillaWeathers, sizeof(vanillaWeathers));
		ZeroMemory(&vanillaWeathers, sizeof(vanillaWeathers));
#endif

		return this;
	}

	const auto TES3_WeatherController_dtor = reinterpret_cast<WeatherController * (__thiscall*)(WeatherController*)>(0x43B1C0);
	void WeatherController::dtor() {
		// Call vanilla function.
		TES3_WeatherController_dtor(this);

#if defined(MWSE_CUSTOM_WEATHERS) && MWSE_CUSTOM_WEATHERS == TRUE
		// Clean up where we moved the weathers.
		for (auto& weather : arrayWeathers) {
			if (weather == nullptr) {
				continue;
			}

			delete weather;
			weather = nullptr;
		}
#endif
	}

	const auto TES3_WeatherController_calcSunDamageScalar = reinterpret_cast<float(__thiscall*)(WeatherController*)>(0x0440630);
	float WeatherController::calcSunDamageScalar() {
		float damage = TES3_WeatherController_calcSunDamageScalar(this);

		// Trigger calcSunDamageScalar event.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		const auto stateHandle = luaManager.getThreadSafeStateHandle();
		sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CalcSunDamageScalarEvent(damage));
		if (eventData.valid()) {
			damage = eventData.get_or<float>("damage", damage);
		}

		return damage;
	}

	const auto TES3_WeatherController_switchWeather = reinterpret_cast<void(__thiscall*)(WeatherController*, int, float)>(0x441C40);
	void WeatherController::switchWeather(int weatherId, float startingTransition) {
		windVelocityCurrWeather = { 0.0f, 0.0f, 0.0f };
		windVelocityNextWeather = { 0.0f, 0.0f, 0.0f };
		sgAshCloud->setAppCulled(true);
		sgBlightCloud->setAppCulled(true);
		sgBlizzard->setAppCulled(true);

		transitionScalar = startingTransition;
		if (startingTransition < 1.0f && currentWeather) {
			if (startingTransition == 0.0f) {
				transition(weatherId);
			}
			else {
				setNextWeather(weatherId);
			}
		}
		else {
			setCurrentWeather(weatherId);
			clearNextWeather();
		}

		g_fillSnowParticleVolume::set(true);
	}

	const auto TES3_WeatherController_enableSky = reinterpret_cast<void(__thiscall*)(WeatherController*)>(0x440820);
	void WeatherController::enableSky() {
		TES3_WeatherController_enableSky(this);
	}

	const auto TES3_WeatherController_disableSky = reinterpret_cast<void(__thiscall*)(WeatherController*)>(0x440870);
	void WeatherController::disableSky() {
		TES3_WeatherController_disableSky(this);
	}

	void WeatherController::transition(int weatherId) {
		if (transitionScalar != 0.0f) {
			return;
		}

		windVelocityCurrWeather = Vector3::ZEROES;
		windVelocityNextWeather = Vector3::ZEROES;

		if (currentWeather == nullptr) {
			switchWeather(weatherId, 1.0f);
		}

		transitionScalar = 0.01f;
		setNextWeather(weatherId);
	}

	void WeatherController::onInactivateWeather(DataHandler* dataHandler, float gameHour) {
		inactivateWeather = false;
		sgAshCloud->setAppCulled(true);
		sgBlightCloud->setAppCulled(true);
		sgBlizzard->setAppCulled(true);

		// Update blight spells.
		const auto weatherBlight = getWeatherBlight();
		for (const auto spell : *dataHandler->nonDynamicData->spellsList) {
			if (spell->isBlightDisease() && !spell->hasEffect(EffectID::Corprus)) {
				weatherBlight->blightDiseases.push_back(spell);
			}
		}

		if (!lastActiveRegion) {
			// Determine the weather appropriate for the cell.
			const auto playerCell = dataHandler->currentCell;
			if (playerCell->getIsOrBehavesAsExterior()) {
				const auto playerCellRegion = playerCell->getRegion();
				if (playerCellRegion) {
					setCurrentWeather(playerCellRegion->currentWeatherIndex);
					lastActiveRegion = playerCellRegion;
				}
				else {
					setCurrentWeather(WeatherType::Clear);
				}
			}
			else {
				clearCurrentWeather();
			}
		}

		if (!unknown_0x10) {
			unknown_0x10 = true;
			hoursRemaining = 0.0f;
			if (gameHour >= 0.0f) {
				do {
					hoursRemaining += hoursBetweenWeatherChanges;
				} while (hoursRemaining <= gameHour);
			}
			while (hoursRemaining >= 24.0f) {
				hoursRemaining -= 24.0f;
				daysRemaining++;
			}
		}
	}

	bool WeatherController::isStormy() const {
		const auto isTransitionFullyActive = transitionScalar <= 0.0f || transitionScalar >= 1.0f;
		const auto currentThreshold = 1.0f - transitionScalar;

		if (currentWeather) {
			switch (currentWeather->index) {
			case WeatherType::Ash:
				if (isTransitionFullyActive || currentThreshold > static_cast<const WeatherAsh*>(currentWeather)->stormThreshold) {
					return true;
				}
				break;
			case WeatherType::Blight:
				if (isTransitionFullyActive || currentThreshold > static_cast<const WeatherBlight*>(currentWeather)->stormThreshold) {
					return true;
				}
				break;
			case WeatherType::Blizzard:
				if (isTransitionFullyActive || currentThreshold > static_cast<const WeatherBlizzard*>(currentWeather)->stormThreshold) {
					return true;
				}
				break;
			default:
				if (currentWeather->isCustomWeather()) {
					const auto customWeather = static_cast<const WeatherCustom*>(currentWeather);
					if (customWeather->stormThreshold.has_value()
						&& (isTransitionFullyActive || currentThreshold > customWeather->stormThreshold.value())) {
						return true;
					}
				}
				break;
			}
		}

		if (nextWeather) {
			switch (nextWeather->index) {
			case WeatherType::Ash:
				return static_cast<const WeatherAsh*>(nextWeather)->stormThreshold < transitionScalar;
			case WeatherType::Blight:
				return static_cast<const WeatherBlight*>(nextWeather)->stormThreshold < transitionScalar;
			case WeatherType::Blizzard:
				return static_cast<const WeatherBlizzard*>(nextWeather)->stormThreshold < transitionScalar;
			default:
				if (nextWeather->isCustomWeather()) {
					const auto customWeather = static_cast<const WeatherCustom*>(nextWeather);
					return customWeather->stormThreshold.has_value()
						&& customWeather->stormThreshold.value() < transitionScalar;
				}
				break;
			}
		}

		return false;
	}

	bool WeatherController::updateParticles(int mode) const {
		auto currentThreshold = Weather::IMPOSSIBLE_THRESHOLD;
		auto nextThreshold = Weather::IMPOSSIBLE_THRESHOLD;

		if (mode == 1) {
			currentThreshold = currentWeather ? currentWeather->getRainThreshold() : Weather::IMPOSSIBLE_THRESHOLD;
			nextThreshold = nextWeather ? nextWeather->getRainThreshold() : Weather::IMPOSSIBLE_THRESHOLD;
		}
		else if (mode == 5) {
			currentThreshold = currentWeather ? currentWeather->getSnowThreshold() : Weather::IMPOSSIBLE_THRESHOLD;
			nextThreshold = nextWeather ? nextWeather->getSnowThreshold() : Weather::IMPOSSIBLE_THRESHOLD;
		}
		else {
			return false;
		}

		// Transitions between two weathers that support this mode always return true.
		if (currentThreshold > 0.0f && nextThreshold > 0.0f) {
			return true;
		}

		if (currentWeather && currentWeather->getRelevance() >= currentThreshold) {
			return true;
		}

		if (nextWeather && nextWeather->getRelevance() >= nextThreshold) {
			return true;
		}

		return false;
	}

	float WeatherController::lerpE0() const {
		if (currentWeather && currentWeather->supportsParticleLerp() && nextWeather && nextWeather->supportsParticleLerp()) {
			return mwse::math::lerp(currentWeather->unknown_0xE0, nextWeather->unknown_0xE0, transitionScalar);
		}
		else if (currentWeather && currentWeather->supportsParticleLerp()) {
			return currentWeather->unknown_0xE0;
		}
		else {
			return 0.0f;
		}
	}

	Vector3* WeatherController::lerpE4(Vector3* out_result) const {
		if (currentWeather && currentWeather->supportsParticleLerp() && nextWeather && nextWeather->supportsParticleLerp()) {
			*out_result = currentWeather->unknown_0xE4.lerp(nextWeather->unknown_0xE4, transitionScalar);
			return out_result;
		}
		else if (currentWeather && currentWeather->supportsParticleLerp()) {
			*out_result = currentWeather->unknown_0xE4;
			return out_result;
		}
		else {
			*out_result = { 1.0f, 0.0f, 0.0f };
			return out_result;
		}
	}

	void WeatherController::setCurrentWeather(int weatherId) {
		if (weatherId < WeatherType::MINIMUM || weatherId > WeatherType::MAXIMUM) {
			return;
		}

		clearCurrentWeather();

		// If the desired weather is invalid, abort.
		// This is only likely to happen if loading a save where a custom weather has been removed.
		const auto weather = getWeather(weatherId);
		if (weather == nullptr) {
			return;
		}

		currentWeather = weather;

		// Validate cloud texture location.
		const auto buffer = mwse::tes3::getThreadSafeStringBuffer();
		if (mwse::tes3::resolveAssetPath(currentWeather->texturePathCloud, buffer) == 0) {
			mwse::tes3::logAndShowError("Weather %s texture not found.", currentWeather->texturePathCloud);
			sprintf(buffer, "%s\\tx_mooncircle_full_M.tga", "Textures");
		}

		// Set cloud texture.
		auto cloudTexture = NI::SourceTexture::createFromPath(buffer);
		const auto cloudTextureProperty = sgTriCloudsCurrent->getTexturingProperty();
		if (cloudTextureProperty->getBaseMap() == nullptr) {
			const auto baseMap = new NI::TexturingProperty::Map(cloudTexture);
			baseMap->filterMode = NI::TexturingProperty::FilterMode::BILERP;
			cloudTextureProperty->setBaseMap(baseMap);
		}
		cloudTextureProperty->getBaseMap()->texture = cloudTexture;
	}

	void WeatherController::clearCurrentWeather() {
		if (currentWeather == nullptr) {
			return;
		}
		currentWeather->unload();
		currentWeather = nullptr;
	}

	void WeatherController::setNextWeather(int weatherId) {
		if (weatherId < WeatherType::MINIMUM || weatherId > WeatherType::MAXIMUM) {
			return;
		}

		clearNextWeather();

		// If the desired weather is invalid, abort.
		// This is only likely to happen if loading a save where a custom weather has been removed.
		const auto weather = getWeather(weatherId);
		if (weather == nullptr) {
			return;
		}

		nextWeather = weather;

		// Validate cloud texture location.
		const auto buffer = mwse::tes3::getThreadSafeStringBuffer();
		if (mwse::tes3::resolveAssetPath(nextWeather->texturePathCloud, buffer) == 0) {
			mwse::tes3::logAndShowError("Weather %s texture not found.", nextWeather->texturePathCloud);
			sprintf(buffer, "%s\\tx_mooncircle_full_M.tga", "Textures");
		}

		// Set cloud texture.
		auto cloudTexture = NI::SourceTexture::createFromPath(buffer);
		const auto cloudTextureProperty = sgTriCloudsNext->getTexturingProperty();
		if (cloudTextureProperty->getBaseMap() == nullptr) {
			const auto baseMap = new NI::TexturingProperty::Map(cloudTexture);
			baseMap->filterMode = NI::TexturingProperty::FilterMode::BILERP;
			cloudTextureProperty->setBaseMap(baseMap);
		}
		cloudTextureProperty->getBaseMap()->texture = cloudTexture;
	}

	void WeatherController::clearNextWeather() {
		if (nextWeather == nullptr) {
			return;
		}
		nextWeather->unload();
		nextWeather = nullptr;
	}

	int WeatherController::getCurrentWeatherIndex() const {
		return currentWeather ? currentWeather->index : WEATHER_ID_INVALID;
	}

	int WeatherController::getNextWeatherIndex() const {
		return nextWeather ? nextWeather->index : WEATHER_ID_INVALID;
	}

	std::reference_wrapper<Weather* [MAX_WEATHER_COUNT]> WeatherController::getWeathers() {
		return std::ref(arrayWeathers);
	}

	Weather* WeatherController::getWeather(int weatherId) const {
		if (weatherId < WeatherType::MINIMUM || weatherId > WeatherType::MAXIMUM) {
			return nullptr;
		}
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

	unsigned char WeatherController::getWeatherBaseVolume() const {
		const auto worldController = WorldController::get();
		const auto audio = worldController ? worldController->audioController : nullptr;
		if (!audio) {
			return 0;
		}

		return static_cast<unsigned char>(std::clamp<int>(audio->volumeMaster * audio->volumeEffects / 250, 0, 250));
	}

	unsigned char WeatherController::getWeatherScaledVolume(float transitionScalar) const {
		return static_cast<unsigned char>(std::clamp<int>(static_cast<int>(getWeatherBaseVolume() * transitionScalar), 0, 250));
	}

	const auto TES3_WeatherController_setBackgroundToFog = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Object*)>(0x43EB20);
	void WeatherController::setBackgroundToFog(NI::Object* background) {
		TES3_WeatherController_setBackgroundToFog(this, background);
	}

	const auto TES3_WeatherController_setFogColour = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Property*)>(0x43EB80);
	void WeatherController::setFogColour(NI::Property* fogProperty) {
		TES3_WeatherController_setFogColour(this, fogProperty);
	}

	const auto TES3_WeatherController_updateAmbient = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43EF80);
	void WeatherController::updateAmbient(float gameHour) {
		TES3_WeatherController_updateAmbient(this, gameHour);
	}

	const auto TES3_WeatherController_updateColours = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43E000);
	void WeatherController::updateColours(float gameHour) {
		TES3_WeatherController_updateColours(this, gameHour);
	}

	const auto TES3_WeatherController_updateClouds = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43EC20);
	void WeatherController::updateClouds(float deltaTime) {
		TES3_WeatherController_updateClouds(this, deltaTime);
	}

	const auto TES3_WeatherController_updateCloudVertexCols = reinterpret_cast<void(__thiscall*)(WeatherController*)>(0x43EDE0);
	void WeatherController::updateCloudVertexCols() {
		TES3_WeatherController_updateCloudVertexCols(this);
	}

	const auto TES3_WeatherController_lerpFogDepth = reinterpret_cast<float(__thiscall*)(WeatherController*, float)>(0x443FB0);
	float WeatherController::lerpFogDepth(float gameHour) {
		return TES3_WeatherController_lerpFogDepth(this, gameHour);
	}

	const auto TES3_WeatherController_getFogDensityMult = reinterpret_cast<float(__thiscall*)(WeatherController*, float)>(0x444250);
	float WeatherController::getFogDensityMult(float gameHour) {
		return TES3_WeatherController_getFogDensityMult(this, gameHour);
	}

	const auto TES3_WeatherController_updateSunCols = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43F5F0);
	void WeatherController::updateSunCols(float gameHour) {
		TES3_WeatherController_updateSunCols(this, gameHour);
	}

	const auto TES3_WeatherController_updateSun = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43FF80);
	void WeatherController::updateSun(float gameHour) {
		TES3_WeatherController_updateSun(this, gameHour);
	}

	void WeatherController::updateTick(NI::FogProperty* fogProperty, float deltaTime, bool skyVisible, float gameHour) {
		const auto worldController = WorldController::get();

		// Rotate the night sky.
		Matrix33 rotation;
		rotation.toRotation(
			std::fmod(worldController->gvarDaysPassed->value, 4.0f) * 0.25f * mwse::math::M_2_PI + gameHour * (1.0f / 24.0f) * mwse::math::M_PI_2,
			0.0f,
			0.0f,
			1.0f
		);
		sgSkyNight->setLocalRotationMatrix(&rotation);

		// Check to see if rain/snow particles should cull.
		if (!updateParticles(1)) {
			sgRainRoot->setAppCulled(true);
		}
		if (!updateParticles(5)) {
			sgSnowRoot->setAppCulled(true);
		}

		if (transitionScalar >= 1.0f) {
			windVelocityNextWeather = Vector3::ZEROES;
			windVelocityCurrWeather = Vector3::ZEROES;

			if (nextWeather) {
				if (currentWeather) {
					currentWeather->unload();
				}

				currentWeather = nextWeather;
				nextWeather = nullptr;
				transitionScalar = 0.0f;

				currentWeather->updateCloudTexture(sgTriCloudsCurrent);

				const auto nextCloudModelData = sgTriCloudsNext ? sgTriCloudsNext->modelData.get() : nullptr;
				const auto currentCloudModelData = sgTriCloudsCurrent ? sgTriCloudsCurrent->modelData.get() : nullptr;
				if (nextCloudModelData && currentCloudModelData && nextCloudModelData->textureCoords && currentCloudModelData->textureCoords) {
					for (auto i = 0u; i < nextCloudModelData->vertexCount; ++i) {
						currentCloudModelData->textureCoords[i] = nextCloudModelData->textureCoords[i];
					}

					sgTriCloudsCurrent->vTable.asAVObject->updateWorldVertices(sgTriCloudsCurrent);
					currentCloudModelData->markAsChanged();
				}
			}
			else {
				transitionScalar = 0.0f;
				currentWeather->updateCloudTexture(sgTriCloudsCurrent);
			}
		}

		if (transitionScalar > 0.0f && nextWeather) {
			transitionScalar += deltaTime * nextWeather->transitionDelta;
		}
		transitionScalar = std::min(transitionScalar, 1.0f);

		// Instantly switch weather if we are passing time.
		if (nextWeather && UI::findMenu("MenuTimePass")) {
			Patch_WeatherController_SwitchWeather_WithEvent(this, 0, nextWeather->index, 1.0f);
		}

		if (!skyVisible) {
			sgRainRoot->setAppCulled(true);
			sgSnowRoot->setAppCulled(true);

			if (dataHandler && dataHandler->waterController) {
				dataHandler->waterController->setRainFrequency(0.0f);
			}

			g_fillSnowParticleVolume::set(true);
			return;
		}

		// Dispatch simulate calls.
		if (currentWeather) {
			currentWeather->simulate(1.0f - transitionScalar, deltaTime);
			currentWeather->transition();
		}
		if (nextWeather) {
			nextWeather->simulate(transitionScalar, deltaTime);
			nextWeather->transition();
		}
		else if (currentWeather) {
			if (!currentWeather->getSupportsAshCloud()) {
				sgAshCloud->setAppCulled(true);
			}
			if (!currentWeather->getSupportsBlightCloud()) {
				sgBlightCloud->setAppCulled(true);
			}
			if (!currentWeather->getSupportsBlizzard()) {
				sgBlizzard->setAppCulled(true);
			}
		}

		auto fogDensityMult = lerpFogDepth(gameHour);
		if (underwaterPitchbendState) {
			fogDensityMult = getFogDensityMult(worldController->gvarGameHour->value);
		}
		fogProperty->density = std::max(fogDensityMult, 0.0f);
		fogProperty->update(0.0f);

		const auto deactivateParticlesByType = [&](int type, int& activeCount) {
			for (auto it = listActiveParticles.begin(); it != listActiveParticles.end(); ) {
				auto particle = *it;
				if (particle->getType() != type) {
					++it;
					continue;
				}

				it = listActiveParticles.erase(it);
				particle->object->setAppCulled(true);
				listInactiveParticles.push_back(particle);
				--activeCount;
			}
		};

		if (activeRainParticles > 0 && !updateParticles(1)) {
			deactivateParticlesByType(1, activeRainParticles);
		}
		if (activeSnowParticles > 0 && !updateParticles(5)) {
			deactivateParticlesByType(5, activeSnowParticles);
		}

		if (listActiveParticles.empty()) {
			sgRainRoot->setAppCulled(true);
			sgSnowRoot->setAppCulled(true);

			if (dataHandler && dataHandler->waterController) {
				dataHandler->waterController->setRainFrequency(0.0f);
			}

			g_fillSnowParticleVolume::set(true);
			return;
		}

		auto particlesEffectiveMax = 0.0f;
		if (currentWeather) {
			particlesEffectiveMax = currentWeather->getPrecipitationMax();
		}
		if (nextWeather) {
			particlesEffectiveMax = (1.0f - transitionScalar) * particlesEffectiveMax + nextWeather->getPrecipitationMax() * transitionScalar;
		}

		const auto playerCell = dataHandler ? dataHandler->currentCell : nullptr;
		if (sgSkyRoot->getAppCulled() || (playerCell && !playerCell->getIsOrBehavesAsExterior())) {
			if (dataHandler && dataHandler->waterController) {
				dataHandler->waterController->setRainFrequency(0.0f);
			}
		}
		else if (rainRipples && updateParticles(1)) {
			if (dataHandler && dataHandler->waterController && snowRipples && updateParticles(5)) {
				dataHandler->waterController->setRainFrequency(float(activeRainParticles + activeSnowParticles) / particlesEffectiveMax);
			}
			else if (dataHandler && dataHandler->waterController) {
				dataHandler->waterController->setRainFrequency(float(activeRainParticles) / particlesEffectiveMax);
			}
		}
		else if (dataHandler && dataHandler->waterController && snowRipples && updateParticles(5)) {
			dataHandler->waterController->setRainFrequency(float(activeSnowParticles) / particlesEffectiveMax);
		}

		const auto waterLevel = playerCell && playerCell->getHasWater() ? playerCell->getWaterLevel().value_or(-100.0f) : -100.0f;
		for (auto it = listActiveParticles.begin(); it != listActiveParticles.end(); ) {
			auto particle = *it;
			particle->update(deltaTime, waterLevel);

			if (!particle->unknown_0x34) {
				++it;
				continue;
			}

			it = listActiveParticles.erase(it);
			particle->object->setAppCulled(true);
			listInactiveParticles.push_back(particle);

			const auto particleType = particle->getType();
			if (particleType == 1) {
				--activeRainParticles;
			}
			else if (particleType == 5) {
				--activeSnowParticles;
			}
		}

		g_fillSnowParticleVolume::set(true);
	}

	void WeatherController::updateVisuals() {
		// Allows weather visuals to update when simulation is paused.
		auto gameHour = WorldController::get()->gvarGameHour->value;
		auto renderer = WorldController::get()->renderer;
		auto fogProperty = DataHandler::get()->sgFogProperty;

		updateTick(fogProperty, 0.0f, true, gameHour);
		updateClouds(0.0f);
		updateColours(gameHour);
		updateCloudVertexCols();
		updateAmbient(gameHour);
		updateSunCols(gameHour);
		updateSun(gameHour);

		// setBackgroundToFog decrements the refCount
		renderer->refCount++;
		setBackgroundToFog(renderer);

		// setFogColour decrements the refCount
		fogProperty->refCount++;
		setFogColour(fogProperty);
	}

	void WeatherController::switchImmediate(int weather) {
		using mwse::lua::event::WeatherChangedImmediateEvent;

		if (lastActiveRegion) {
			lastActiveRegion->currentWeatherIndex = weather;
		}
		switchWeather(weather, 1.0f);

		// Fire off the event, after function completes.
		// Prevent recursive triggering of weather change events.
		if (!WeatherChangedImmediateEvent::ms_EventGuard && WeatherChangedImmediateEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();

			WeatherChangedImmediateEvent::ms_EventGuard = true;
			stateHandle.triggerEvent(new WeatherChangedImmediateEvent());
			WeatherChangedImmediateEvent::ms_EventGuard = false;
		}
	}

	void WeatherController::switchTransition(int weather) {
		using mwse::lua::event::WeatherChangedImmediateEvent;

		switchWeather(weather, 0.001f);
		if (lastActiveRegion) {
			lastActiveRegion->currentWeatherIndex = weather;
		}

		// Fire off the event after the transition starts.
		// Prevent recursive triggering of weather change events.
		if (!WeatherChangedImmediateEvent::ms_EventGuard && WeatherChangedImmediateEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();

			WeatherChangedImmediateEvent::ms_EventGuard = true;
			stateHandle.triggerEvent(new WeatherChangedImmediateEvent());
			WeatherChangedImmediateEvent::ms_EventGuard = false;
		}
	}

	static void __fastcall Patch_WeatherController_SwitchWeather_WithEvent(WeatherController* wc, DWORD _EDX_, int weatherId, float startingTransition) {
		wc->switchWeather(weatherId, startingTransition);

		if (mwse::lua::event::WeatherChangedImmediateEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();

			stateHandle.triggerEvent(new mwse::lua::event::WeatherChangedImmediateEvent());
		}
	}

	void WeatherController::installPatches() {
#if defined(MWSE_CUSTOM_WEATHERS) && MWSE_CUSTOM_WEATHERS == TRUE
		// Change size of constructor.
		mwse::writeValueEnforced<DWORD>(0x417EF6 + 0x1, 0x1F0, sizeof(WeatherController));

		// Setup custom weather vtable.
		ZeroMemory(&WeatherCustom::VirtualTable, sizeof(WeatherCustom::VirtualTable));
		auto WeatherCustom_delete = &WeatherCustom::vtbl_delete;
		auto WeatherCustom_simulate = &WeatherCustom::vtbl_simulate;
		auto WeatherCustom_transition = &WeatherCustom::vtbl_transition;
		auto WeatherCustom_unload = &WeatherCustom::vtbl_unload;
		mwse::overrideVirtualTableEnforced(DWORD(&WeatherCustom::VirtualTable), offsetof(Weather_vTable, deleting), 0x0, *reinterpret_cast<DWORD*>(&WeatherCustom_delete));
		mwse::overrideVirtualTableEnforced(DWORD(&WeatherCustom::VirtualTable), offsetof(Weather_vTable, simulate), 0x0, *reinterpret_cast<DWORD*>(&WeatherCustom_simulate));
		mwse::overrideVirtualTableEnforced(DWORD(&WeatherCustom::VirtualTable), offsetof(Weather_vTable, transition), 0x0, *reinterpret_cast<DWORD*>(&WeatherCustom_transition));
		mwse::overrideVirtualTableEnforced(DWORD(&WeatherCustom::VirtualTable), offsetof(Weather_vTable, unload), 0x0, *reinterpret_cast<DWORD*>(&WeatherCustom_unload));

		// Fixup constructor.
		const auto WeatherController_ctor = &WeatherController::ctor;
		mwse::genCallEnforced(0x417F17, DWORD(TES3_WeatherController_ctor), *reinterpret_cast<const DWORD*>(&WeatherController_ctor));

		// Fixup destructor.
		const auto WeatherController_dtor = &WeatherController::dtor;
		mwse::genCallEnforced(0x40E0CB, DWORD(TES3_WeatherController_dtor), *reinterpret_cast<const DWORD*>(&WeatherController_dtor));

		// Replace function: switchWeather
		const auto WeatherController_switchWeather = &WeatherController::switchWeather;
		mwse::genCallEnforced(0x410368, 0x441C40, reinterpret_cast<const DWORD>(Patch_WeatherController_SwitchWeather_WithEvent));
		mwse::genCallEnforced(0x441084, 0x441C40, reinterpret_cast<const DWORD>(Patch_WeatherController_SwitchWeather_WithEvent));
		mwse::genCallEnforced(0x441AA7, 0x441C40, *reinterpret_cast<const DWORD*>(&WeatherController_switchWeather));
		mwse::genCallEnforced(0x45CE2D, 0x441C40, reinterpret_cast<const DWORD>(Patch_WeatherController_SwitchWeather_WithEvent));
		mwse::genCallEnforced(0x45D211, 0x441C40, reinterpret_cast<const DWORD>(Patch_WeatherController_SwitchWeather_WithEvent));
		mwse::genCallEnforced(0x4BE166, 0x441C40, *reinterpret_cast<const DWORD*>(&WeatherController_switchWeather));
		mwse::genCallEnforced(0x4BE19A, 0x441C40, *reinterpret_cast<const DWORD*>(&WeatherController_switchWeather));

		// Replace function: transition
		const auto WeatherController_transition = &WeatherController::transition;
		mwse::genCallEnforced(0x41034D, 0x441A10, *reinterpret_cast<const DWORD*>(&WeatherController_transition));
		mwse::genCallEnforced(0x441D13, 0x441A10, *reinterpret_cast<const DWORD*>(&WeatherController_transition));

		// Replace function: onInactivateWeather
		const auto WeatherController_onInactivateWeather = &WeatherController::onInactivateWeather;
		mwse::genCallEnforced(0x410245, 0x4415E0, *reinterpret_cast<const DWORD*>(&WeatherController_onInactivateWeather));

		// Replace function: isStormy
		const auto WeatherController_isStormy = &WeatherController::isStormy;
		mwse::genCallEnforced(0x43B9E5, 0x452DC0, *reinterpret_cast<const DWORD*>(&WeatherController_isStormy));

		// Replace function: updateParticles
		const auto WeatherController_updateParticles = &WeatherController::updateParticles;
		mwse::genCallEnforced(0x43BA00, 0x452AE0, *reinterpret_cast<const DWORD*>(&WeatherController_updateParticles));
		mwse::genCallEnforced(0x43BA1B, 0x452AE0, *reinterpret_cast<const DWORD*>(&WeatherController_updateParticles));
		mwse::genCallEnforced(0x440D20, 0x452AE0, *reinterpret_cast<const DWORD*>(&WeatherController_updateParticles));
		mwse::genCallEnforced(0x440D37, 0x452AE0, *reinterpret_cast<const DWORD*>(&WeatherController_updateParticles));
		mwse::genCallEnforced(0x44117B, 0x452AE0, *reinterpret_cast<const DWORD*>(&WeatherController_updateParticles));
		mwse::genCallEnforced(0x4411E5, 0x452AE0, *reinterpret_cast<const DWORD*>(&WeatherController_updateParticles));
		mwse::genCallEnforced(0x44139C, 0x452AE0, *reinterpret_cast<const DWORD*>(&WeatherController_updateParticles));
		mwse::genCallEnforced(0x4413B3, 0x452AE0, *reinterpret_cast<const DWORD*>(&WeatherController_updateParticles));
		mwse::genCallEnforced(0x44141F, 0x452AE0, *reinterpret_cast<const DWORD*>(&WeatherController_updateParticles));

		// Replace function: updateTick
		const auto WeatherController_updateTick = &WeatherController::updateTick;
		mwse::genCallEnforced(0x4103CC, 0x440C80, *reinterpret_cast<const DWORD*>(&WeatherController_updateTick));
		mwse::genCallEnforced(0x41050E, 0x440C80, *reinterpret_cast<const DWORD*>(&WeatherController_updateTick));

		// Replace function: lerpE0
		const auto WeatherController_lerpE0 = &WeatherController::lerpE0;
		mwse::genCallEnforced(0x451FF1, 0x442460, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE0));
		mwse::genCallEnforced(0x452890, 0x442460, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE0));

		// Replace function: lerpE4
		const auto WeatherController_lerpE4 = &WeatherController::lerpE4;
		mwse::genCallEnforced(0x451FB8, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));
		mwse::genCallEnforced(0x451FCC, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));
		mwse::genCallEnforced(0x451FE3, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));
		mwse::genCallEnforced(0x45285A, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));
		mwse::genCallEnforced(0x45286E, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));
		mwse::genCallEnforced(0x452882, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));

		// Patch mwscript: ChangeWeather
		mwse::genNOPUnprotected(0x50C361, 0x50C370 - 0x50C361);
		bool (Region::*Region_setCurrentWeather)(int) = &Region::setCurrentWeather;
		mwse::genCallEnforced(0x50C373, 0x4812F0, *reinterpret_cast<const DWORD*>(&Region_setCurrentWeather));
#endif
	}

	//
	// WeatherController::Particle
	//

	int WeatherController::Particle::getType() const {
		return vtbl->getType(this);
	}

	void WeatherController::Particle::update(float dt, float waterLevel) {
		return vtbl->update(this, dt, waterLevel);
	}
}
