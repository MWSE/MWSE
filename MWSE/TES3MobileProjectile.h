#pragma once

#include "TES3Defines.h"

#include "TES3MobileObject.h"
#include "NIVector3.h"

namespace TES3 {
	struct MobileProjectile_vTable : MobileObject_vTable {
		void* initStats; // 0x98
		void* initPhysics; // 0x9C
		void* serializeExtraData; // 0xA0
	};
	static_assert(sizeof(MobileProjectile_vTable) == 0xA4, "TES3::MobileProjectile_vTable failed size validation");

	struct MobileProjectile : MobileObject {
		MobileActor* firingActor; // 0x80
		Weapon* firingWeapon; // 0x84
		unsigned char flagExpire; // 0x88
		unsigned char patchFlagExplode; // 0x89
		char padding_0x8A[2];
		float attackSwing; // 0x8C
		float initialSpeed; // 0x90
		int unknown_0x94;
		NI::Vector3 velocity; // 0x98
		float damage; // 0xA4
		float animTime; // 0xA8

		MobileProjectile() = delete;
		~MobileProjectile() = delete;

		//
		// vTable accessor functions.
		//

		bool onActorCollision(int collisionIndex);
		bool onObjectCollision(int collisionIndex, bool flag);
		bool onTerrainCollision(int collisionIndex);
		bool onWaterCollision(int collisionIndex);

		//
		// Custom functions.
		//

		NI::Vector3 getProjectileVelocity() const;
		void setProjectileVelocity_lua(sol::stack_object value);

	};
	static_assert(sizeof(MobileProjectile) == 0xAC, "TES3::MobileProjectile failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3_MOBILEOBJECT(TES3::MobileProjectile)
