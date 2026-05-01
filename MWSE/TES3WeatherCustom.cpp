#include "TES3WeatherCustom.h"

#include "TES3Sound.h"
#include "TES3WeatherController.h"

namespace TES3 {
	Weather_vTable WeatherCustom::VirtualTable;

	WeatherCustom::WeatherCustom() : Weather() {
		vTable = &VirtualTable;
		isStormy = false;
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

		if (simulateFunction.valid()) {
			simulateFunction(this, transitionScalar, deltaTime);
		}
	}

	void WeatherCustom::vtbl_transition() {
		const auto transitionScalar = controller ? std::clamp(controller->getTransitionScalarForWeather(this), 0.0f, 1.0f) : 1.0f;
		if (soundAmbientLoop && soundAmbientLoop->isPlaying()) {
			soundAmbientLoop->setVolumeRaw(controller ? controller->getWeatherScaledVolume(transitionScalar) : 0);
			updateUnderwaterFrequency();
		}

		if (transitionFunction.valid()) {
			transitionFunction(this);
		}
	}

	void WeatherCustom::vtbl_unload() {
		ambientPlaying = false;
		if (soundAmbientLoop) {
			soundAmbientLoop->stop();
			soundAmbientLoop->release();
		}

		if (unloadFunction.valid()) {
			unloadFunction(this);
		}
	}
}
