#include "TES3WeatherCloudy.h"

namespace TES3 {
	void WeatherCloudy::simulate(float transitionScalar, float deltaTime) {
		(void)deltaTime;

		updateCloudWind();
		updateAmbientSound(transitionScalar);
	}
}
