#pragma once

#include "TES3MobileProjectile.h"

namespace TES3 {
	struct MobileSpellProjectile_vTable : MobileProjectile_vTable {
		void* setMagicInstanceSerial; // 0xA4
		void* setRotationSpeed; // 0xA8
		void* launchProjectile; // 0xAC
	};
	static_assert(sizeof(MobileSpellProjectile_vTable) == 0xB0, "TES3::MobileSpellProjectile_vTable failed size validation");

	struct MobileSpellProjectile : MobileProjectile {
		unsigned int spellInstanceSerial; // 0xAC
		float rotationSpeed; // 0xB0

		MobileSpellProjectile() = delete;
		~MobileSpellProjectile() = delete;

		//
		// Custom functions.
		//

		bool onActorCollision(int collisionIndex);
		bool onStaticCollision(int collisionIndex, bool isAvoidNode);
		bool onTerrainCollision(int collisionIndex);
		bool onWaterCollision(int collisionIndex);

		MagicSourceInstance* getInstance();
		void explode();
		void explodeDeferred();
	};
	static_assert(sizeof(MobileSpellProjectile) == 0xB4, "TES3::MobileSpellProjectile failed size validation");
}
