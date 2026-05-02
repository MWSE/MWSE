#include "TES3WeatherAsh.h"

#include "TES3WeatherController.h"

namespace TES3 {
	void WeatherAsh::simulate(float transitionScalar, float deltaTime) {
		(void)deltaTime;
		updateCloudWind();
		if (controller) {
			controller->updateStormCloud(controller->sgAshCloud, transitionScalar, stormOrigin, stormThreshold);
		}
		updateAmbientSound(transitionScalar);
	}
}
