#include "TES3WeatherCustom.h"

#include "TES3DataHandler.h"
#include "TES3Sound.h"
#include "TES3WorldController.h"

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
		if (simulateFunction.valid()) {
			simulateFunction(this, transitionScalar, deltaTime);
		}
	}

	void WeatherCustom::vtbl_transition() {
		Weather::vtbl_transition();

		if (transitionFunction.valid()) {
			transitionFunction(this);
		}
	}

	void WeatherCustom::vtbl_unload() {
		Weather::vtbl_unload();

		if (unloadFunction.valid()) {
			unloadFunction(this);
		}
	}
}