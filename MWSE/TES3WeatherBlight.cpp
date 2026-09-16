#include "TES3WeatherBlight.h"

#include "TES3MobilePlayer.h"
#include "TES3SpellList.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

#include "RngUtil.h"

namespace TES3 {
	void WeatherBlight::simulate(float transitionScalar, float deltaTime) {
		(void)deltaTime;

		updateCloudWind();
		if (!controller) {
			updateAmbientSound(transitionScalar);
			return;
		}

		controller->updateStormCloud(controller->sgBlightCloud, transitionScalar, stormOrigin, stormThreshold);

		if (transitionScalar >= diseaseTransitionThreshold && !blightDiseases.empty()) {
			const auto diseaseRoll = mwse::rng::getRandomFloat(0.0f, 1.0f);
			if (diseaseRoll < (transitionScalar * diseaseChance)) {
				const auto blightDiseaseCount = blightDiseases.size();
				auto diseaseIndex = static_cast<size_t>(mwse::rng::getRandomFloat(0.0f, 1.0f) * blightDiseaseCount);
				if (diseaseIndex >= blightDiseaseCount) {
					diseaseIndex = blightDiseaseCount - 1;
				}

				const auto disease = blightDiseases[diseaseIndex];
				const auto world = TES3::WorldController::get();
				const auto macp = world ? world->getMobilePlayer() : nullptr;
				const auto spellList = macp ? macp->getSpellList() : nullptr;
				if (spellList && disease) {
					spellList->add(disease);
				}
			}
		}

		updateAmbientSound(transitionScalar);
	}
}
