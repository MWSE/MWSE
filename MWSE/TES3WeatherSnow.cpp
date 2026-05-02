#include "TES3WeatherSnow.h"

#include "TES3WeatherController.h"

namespace TES3 {
	void WeatherSnow::simulate(float transitionScalar, float deltaTime) {
		updateCloudWind();
		if (!controller) {
			updateAmbientSound(transitionScalar);
			return;
		}

		controller->setSnowCulled(transitionScalar < snowThreshold || controller->underwaterPitchbendState);
		updatePrecipitationParticles(5, transitionScalar, deltaTime, snowRadius, snowHeightMin, snowHeightMax, snowEntranceSpeed);
		controller->updateNodeMaterialAlpha(controller->sgSnowRoot, getPrecipitationBlend(5, transitionScalar));

		unknown_0xE4 = { 1.0f, 0.0f, 0.0f };
		unknown_0xE0 = std::atan2(std::fabs(windSpeed) * 0.02f, 1.0f);

		updateAmbientSound(transitionScalar);
	}
}
