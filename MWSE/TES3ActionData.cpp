#include "TES3ActionData.h"

namespace TES3 {
	void ActionData::cleanupMobileActor(MobileActor* mobileActor) {
		if (mobileActor == nullptr) {
			return;
		}

		if (target == mobileActor) {
			target = nullptr;
		}
		if (hitTarget == mobileActor) {
			hitTarget = nullptr;
		}
	}

}
