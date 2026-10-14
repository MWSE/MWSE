#include "TES3WeatherOvercast.h"

namespace TES3 {
	void WeatherOvercast::simulate(float transitionScalar, float deltaTime) {
		(void)deltaTime;

		updateCloudWind();
		updateAmbientSound(transitionScalar);
	}
}
