#include "TES3WeatherBlizzard.h"

#include "TES3WeatherController.h"

#include "MathUtil.h"
#include "NIMatrix33.h"

namespace TES3 {
	void WeatherBlizzard::simulate(float transitionScalar, float deltaTime) {
		(void)deltaTime;

		updateCloudWind();
		if (!controller) {
			updateAmbientSound(transitionScalar);
			return;
		}

		NI::Matrix33 rotation;
		rotation.toRotationZ(se::math::M_PI * 1.5f);
		if (controller->sgBlizzard) {
			controller->sgBlizzard->setLocalRotationMatrix(&rotation);
		}

		controller->setBlizzardCulled(transitionScalar < stormThreshold || controller->underwaterPitchbendState);
		controller->updateNodeMaterialAlpha(controller->sgBlizzard, std::clamp((transitionScalar - stormThreshold) / (1.0f - stormThreshold), 0.0f, 1.0f));

		updateAmbientSound(transitionScalar);
	}
}
