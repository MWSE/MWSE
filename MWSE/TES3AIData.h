#pragma once

#include "TES3Defines.h"
#include "TES3Vectors.h"

namespace TES3 {
	struct AIPlanner {
		AIPackageConfig * currentPackageConfig; // 0x0
		MobileActor * mobileActor; // 0x4
		int counter_0x8;
		AIPackage * packages[32]; // 0xC
		int nextOpenPackageIndex; // 0x8C
		int currentPackageIndex; // 0x90
		float unknown_0x94;

		//
		// Other related this-call functions.
		//

		AIPackage * getActivePackage();

		bool allowRestingNear();

		void assignMobileActor(MobileActor* mobile);

		//
		// Custom functions.
		//

		std::reference_wrapper<AIPackage* [32]> getPackages();

	};
	static_assert(sizeof(AIPlanner) == 0x98, "TES3::AIPlanner failed size validation");
}
