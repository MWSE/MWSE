#include "TES3WeatherThunder.h"

#include "TES3DataHandler.h"
#include "TES3Sound.h"
#include "TES3WeatherController.h"
#include "TES3WaterController.h"

#include "RngUtil.h"

namespace TES3 {
	void WeatherThunder::simulate(float transitionScalar, float deltaTime) {
		const auto weatherController = controller;
		if (!weatherController) {
			return;
		}

		updateCloudWind();
		updateAmbientSound(transitionScalar);

		const auto rainBlend = getPrecipitationBlend(1, transitionScalar);

		weatherController->setRainCulled(transitionScalar < rainThreshold);

		const auto rainVolume = weatherController->getWeatherScaledVolume(rainBlend);
		updateLoopSound(soundRainLoop, soundIDRainLoop, rainPlaying, rainVolume, rainBlend > 0.0f);

		updatePrecipitationParticles(1, transitionScalar, deltaTime, rainRadius, rainHeightMin, rainHeightMax, rainEntranceSpeed);

		loadThunderSounds();
		const auto thunderChance = transitionScalar * thunderFrequency * deltaTime * 0.2f;
		const auto thunderRoll = mwse::rng::getRandomFloat(0.0f, 1.0f);

		if (thunderRoll <= thunderChance && transitionScalar >= thunderThreshold) {
			if (thunderSoundCount > 0) {
				const auto selectedIndex = mwse::rng::getRandomLong(0, thunderSoundCount - 1);
				const auto thunderSound = getLoadedThunderSound(selectedIndex);
				playSound(thunderSound);
				weatherController->modThunderFlashIntensity(static_cast<float>(thunderSoundCount - selectedIndex) / static_cast<float>(thunderSoundCount));
			}
			else {
				weatherController->setThunderFlashIntensity(1.0f);
			}
		}
		else if (weatherController->activeThunderFlashIntensity > 0.0f) {
			weatherController->modThunderFlashIntensity(deltaTime * flashDecrement * -1.0f);
		}
	}

	void WeatherThunder::transition() {
		const auto weatherController = controller;
		if (!weatherController) {
			return;
		}

		const float newVolume = getRelevance();
		const auto rainBlend = getPrecipitationBlend(1, newVolume);

		updateLoopSound(soundAmbientLoop, soundIDAmbientLoop, ambientPlaying, weatherController->getWeatherScaledVolume(newVolume), newVolume >= 0.05f);
		updateLoopSound(soundRainLoop, soundIDRainLoop, rainPlaying, weatherController->getWeatherScaledVolume(rainBlend), rainBlend > 0.0f);

		loadThunderSounds();
		for (int i = 0; i < thunderSoundCount; ++i) {
			updatePlayingSoundVolume(getLoadedThunderSound(i), weatherController->getWeatherBaseVolume());
		}

		underwaterSoundState = weatherController->underwaterPitchbendState;
	}

	void WeatherThunder::unload() {
		ambientPlaying = false;
		if (soundAmbientLoop) {
			soundAmbientLoop->stop();
			soundAmbientLoop->release();
			soundAmbientLoop = nullptr;
		}

		rainPlaying = false;
		if (soundRainLoop) {
			soundRainLoop->stop();
			soundRainLoop->release();
			soundRainLoop = nullptr;
		}

		for (auto& sound : thunderSounds) {
			if (sound) {
				sound->stop();
				sound->release();
				sound = nullptr;
			}
		}

		thunderSoundCount = 0;
	}

	void WeatherThunder::loadThunderSounds() {
		const auto dataHandler = controller ? controller->dataHandler : nullptr;
		const auto nonDynamicData = dataHandler ? dataHandler->nonDynamicData : nullptr;
		if (!nonDynamicData) {
			thunderSoundCount = 0;
			return;
		}

		thunderSoundCount = 0;
		for (size_t i = 0; i < THUNDER_SOUND_COUNT; ++i) {
			auto& sound = thunderSounds[i];
			if (!sound) {
				sound = nonDynamicData->findSound(thunderSoundIds[i]);
			}
			if (sound) {
				++thunderSoundCount;
			}
		}
	}

	const char* WeatherThunder::getThunderSoundId(size_t index) const {
		if (index >= THUNDER_SOUND_COUNT) {
			return nullptr;
		}

		return thunderSoundIds[index];
	}

	bool WeatherThunder::setThunderSoundId(size_t index, const char* path) {
		if (index >= THUNDER_SOUND_COUNT) {
			return false;
		}

		if (path == nullptr) {
			thunderSoundIds[index][0] = 0;
		}
		else if (strcpy_s(thunderSoundIds[index], sizeof(thunderSoundIds[index]), path) != 0) {
			return false;
		}

		if (thunderSounds[index]) {
			if (thunderSounds[index]->isPlaying()) {
				thunderSounds[index]->stop();
			}

			thunderSounds[index] = nullptr;
		}

		return true;
	}

	Sound* WeatherThunder::getLoadedThunderSound(size_t index) const {
		auto loadedIndex = 0u;
		for (size_t i = 0; i < THUNDER_SOUND_COUNT; ++i) {
			const auto sound = thunderSounds[i];
			if (!sound) {
				continue;
			}

			if (loadedIndex == index) {
				return sound;
			}

			++loadedIndex;
		}

		return nullptr;
	}

	Sound* WeatherThunder::getThunderSound(size_t index) const {
		if (index >= THUNDER_SOUND_COUNT) {
			return nullptr;
		}

		return thunderSounds[index];
	}

	void WeatherThunder::setThunderSound(size_t index, Sound* sound) {
		if (index < THUNDER_SOUND_COUNT) {
			thunderSounds[index] = sound;
		}
	}

	bool WeatherThunder::setRainLoopSoundID(const char* id) {
		if (id == nullptr) {
			soundIDRainLoop[0] = 0;
		}
		else if (strcpy_s(soundIDRainLoop, sizeof(soundIDRainLoop), id) != 0) {
			return false;
		}

		if (soundRainLoop) {
			// Stop previous sound.
			if (soundRainLoop->isPlaying()) {
				soundRainLoop->stop();
				rainPlaying = false;
			}

			// Clearing the sound pointer will cause the weather code to resolve the new sound id and play it.
			soundRainLoop = nullptr;
		}
		return true;
	}
}
