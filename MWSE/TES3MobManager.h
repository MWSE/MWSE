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

		ProcessManager() = delete;
		~ProcessManager() = delete;

		//
		// Related this-call functions.
		//

		bool detectAttack(MobileActor* actor);
		bool detectPresence(MobileActor* actor, bool ignoreCreatures = true);
		bool detectByActor(MobileActor* detector, MobileActor* target);
		bool detectSneak(MobileActor* detector, MobileActor* target, bool unknown = true);
		void findActorsInProximity(Vector3 * position, float range, IteratedList<MobileActor*>* outputList);
		void checkAlarmRadius(MobileActor * actor, IteratedList<AIPlanner*> * container);
		void checkPlayerDistance();

		bool checkNearbyEnemiesAllowRest();

		float getAIDistanceScale() const;
		void setAIDistanceScale(float scalar);

		//
		// Custom functions.
		//

		sol::table getAllMobileActors(sol::this_state ts);
		sol::table getAllPlanners(sol::this_state ts);
	};
	static_assert(sizeof(ProcessManager) == 0x830, "TES3::ProcessManager failed size validation");

	struct ProjectileManager {
		IteratedList<MobileProjectile*> activeProjectiles; // 0x0
		NI::Pointer<NI::Node> worldProjectileRoot; // 0x14
		CriticalSection criticalSection; // 0x18

		ProjectileManager() = delete;
		~ProjectileManager() = delete;

		//
		// Custom functions.
		//

		void resolveCollisions(float deltaTime);
		void removeProjectilesFiredByActor(MobileActor* mobileActor, bool includeSpellProjectiles);
	};
	static_assert(sizeof(ProjectileManager) == 0x3C, "TES3::ProjectileManager failed size validation");

	struct MobManager {
		NI::CollisionGroup * mobCollisionGroup;
		Vector3 gravity; // 0x4, Initialized to {0, 0, -627.2}.
		Vector3 terminalVelocity; // Initialized to {0, 0, -4005.5}.
		float dotProductOfMaxClimbableSlope; // Initialized to cos(46 degrees).
		float maxClimbableSlopeDegrees; // Initialized to 46 degrees.
		ProcessManager* processManager; // 0x24
		ProjectileManager* projectileManager; // 0x28
		bool mobCollisionActive;
		IteratedList<Reference*> listMovingProps;
		CriticalSection criticalSection_Props;
		CriticalSection criticalSection_Mobs;

		MobManager() = delete;
		~MobManager() = delete;

		//
		// Related this-call functions.
		//

		// Adds an actor to simulation, while also setting up their collider and AI planner.
		// The reference must not be disabled before calling this function, as it blocks execution.
		void addMob(Reference * reference);

		// Removes an actor from simulation, while also cleaning up their collider and AI planner.
		// Simulation will not be exited if the reference is disabled before calling this function.
		void removeMob(Reference * reference);

		void checkPlayerDistance();

		void addPlayerAsCollider();

		//
		// Custom functions.
		//

		bool hasMobileCollision(const MobileObject* mobile);
		void enableMobileCollision(MobileObject* mobile);
		void disableMobileCollision(MobileObject* mobile);
		void resetConstantVelocities();
		void clampAllActors();

		Vector3* getGravity();
		void setGravity(sol::stack_object);
		Vector3* getTerminalVelocity();
		void setTerminalVelocity(sol::stack_object);
		float getMaxClimbableSlope();
		void setMaxClimbableSlope(float value);
	};
	static_assert(sizeof(MobManager) == 0x8C, "TES3::MobManager failed size validation");
}
