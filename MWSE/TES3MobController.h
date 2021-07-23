#pragma once

#include "TES3Defines.h"

#include "TES3CriticalSection.h"
#include "TES3IteratedList.h"

#include "NINode.h"
#include "NICollisionGroup.h"

namespace TES3 {
	struct ProcessManager {
		MobilePlayer * mobilePlayer; // 0x0
		IteratedList<AIPlanner*> aiPlanners; // 0x4
		bool unknown_0x18;
		bool unknown_0x19; // Maybe force new actions?
		CriticalSection criticalSection; // 0x1C
		unsigned char unknown[0x18]; // 0x40
		unsigned int plannerCount; // 0x58
		AIPlanner * planners[500]; // 0x5C
		float aiDistance; // 0x82C

		//
		// Related this-call functions.
		//

		bool detectPresence(MobileActor * actor, bool unknown = true);
		bool detectSneak(MobileActor * detector, MobileActor * target, bool unknown = true);
		void findActorsInProximity(Vector3 * position, float range, IteratedList<MobileActor*>* outputList);
		void checkAlarmRadius(MobileActor * actor, IteratedList<AIPlanner*> * container);
		void checkPlayerDistance();

		bool canRest();

		float getAIDistanceScale() const;
		void setAIDistanceScale(float scalar);

	};
	static_assert(sizeof(ProcessManager) == 0x830, "TES3::MobController_0x24 failed size validation");

	struct ProjectileController {
		IteratedList<MobileProjectile*> activeProjectiles; // 0x0
		NI::Pointer<NI::Node> worldProjectileRoot; // 0x14
		CriticalSection criticalSection; // 0x18

		//
		// Custom functions.
		//

		void resolveCollisions(float deltaTime);
	};
	static_assert(sizeof(ProjectileController) == 0x3C, "TES3::ProjectileController failed size validation");

	struct MobController {
		NI::CollisionGroup * mobCollisionGroup;
		int unknown_0x4;
		int unknown_0x8;
		float gravity; // 0xC
		int unknown_0x10;
		int unknown_0x14;
		float unknown_0x18; // Initialized to -4005.5
		float unknown_0x1C; // Initialized to 46 degrees.
		float unknown_0x20; // Initialized to cos(46 degrees)
		ProcessManager * processManager; // 0x24
		ProjectileController* projectileController; // 0x28
		bool unknown_0x2C;
		IteratedList<Reference*> listPropReferences;
		CriticalSection criticalSection_Props;
		CriticalSection criticalSection_Mobs;

		//
		// Related this-call functions.
		//

		void addMob(Reference * reference);
		void removeMob(Reference * reference);

		void checkPlayerDistance();

		void addPlayerAsCollider();

		//
		// Custom functions.
		//

		bool hasMobileCollision(const MobileActor* mobile);
		void enableMobileCollision(MobileActor* mobile);
		void disableMobileCollision(MobileActor* mobile);
	};
	static_assert(sizeof(MobController) == 0x8C, "TES3::MobController failed size validation");
}
