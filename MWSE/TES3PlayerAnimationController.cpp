#include "TES3PlayerAnimationController.h"

namespace TES3 {
	bool PlayerAnimationController::force1stPerson() {
		if (is3rdPerson) {
			togglePOV = true;
		}
		return togglePOV;
	}

	bool PlayerAnimationController::force3rdPerson() {
		if (!is3rdPerson) {
			togglePOV = true;
		}
		return togglePOV;
	}
}
