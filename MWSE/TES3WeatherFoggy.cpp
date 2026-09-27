#include "TES3WeatherFoggy.h"

namespace TES3 {
	void WeatherFoggy::simulate(float transitionScalar, float deltaTime) {
		(void)deltaTime;

		updateCloudWind();
		updateAmbientSound(transitionScalar);
	}
}
