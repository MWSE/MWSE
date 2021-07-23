#pragma once

#include "TES3MobileProjectile.h"

namespace TES3 {
	struct MobileSpellProjectile : MobileProjectile {
		unsigned int spellInstanceSerial; // 0xAC
		float rotationSpeed; // 0xB0

		//
		// Custom functions.
		//

		MagicSourceInstance* getInstance();
		void explode();
		void explodeDeferred();
	};
	static_assert(sizeof(MobileSpellProjectile) == 0xB4, "TES3::MobileSpellProjectile failed size validation");
}
