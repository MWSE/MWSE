#include "TES3WeatherRain.h"

#include "TES3Sound.h"
#include "TES3WeatherController.h"

namespace TES3 {
	void WeatherRain::simulate(float transitionScalar, float deltaTime) {
		const auto weatherController = controller;
		if (!weatherController) {
			return;
		}

		updateCloudWind();
		updateAmbientSound(transitionScalar);

		const auto rainBlend = getPrecipitationBlend(1, transitionScalar);

		weatherController->setRainCulled(transitionScalar < rainThreshold);

		const auto baseVolume = weatherController->getWeatherScaledVolume(rainBlend);
		updateLoopSound(soundRainLoop, soundIDRainLoop, rainPlaying, baseVolume, rainBlend > 0.0f);

		updatePrecipitationParticles(1, transitionScalar, deltaTime, rainRadius, rainHeightMin, rainHeightMax, rainEntranceSpeed);
	}

	void WeatherRain::transition() {
		const auto weatherController = controller;
		if (!weatherController) {
			return;
		}

		const float newVolume = getRelevance();
		const auto rainBlend = getPrecipitationBlend(1, newVolume);

		updateLoopSound(soundAmbientLoop, soundIDAmbientLoop, ambientPlaying, weatherController->getWeatherScaledVolume(newVolume), newVolume >= 0.05f);
		updateLoopSound(soundRainLoop, soundIDRainLoop, rainPlaying, weatherController->getWeatherScaledVolume(rainBlend), rainBlend > 0.0f);

		underwaterSoundState = weatherController->underwaterPitchbendState;
	}

	void WeatherRain::unload() {
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
	}

	bool WeatherRain::setRainLoopSoundID(const char* id) {
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
