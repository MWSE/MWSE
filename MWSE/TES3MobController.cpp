#include "TES3MobController.h"

#include "LuaManager.h"

#include "LuaMobileActorActivatedEvent.h"
#include "LuaMobileActorDeactivatedEvent.h"
#include "LuaDetectSneakEvent.h"

#include "TES3MobileActor.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3MobileSpellProjectile.h"
#include "TES3WorldController.h"

namespace TES3 {
	const auto TES3_ProcessManager_detectPresence = reinterpret_cast<bool(__thiscall*)(ProcessManager*, MobileActor*, bool)>(0x570A60);
	bool ProcessManager::detectPresence(MobileActor * actor, bool unknown) {
		return TES3_ProcessManager_detectPresence(this, actor, unknown);
	}

	const auto TES3_ProcessManager_detectSneak = reinterpret_cast<bool(__thiscall*)(ProcessManager*, MobileActor*, MobileActor*, bool)>(0x570600);
	bool ProcessManager::detectSneak(MobileActor* detector, MobileActor* target, bool unknown) {
		// Call the original function to get the default sneak detection.
		bool isDetected = TES3_ProcessManager_detectSneak(this, detector, target, unknown);

		// Launch our event, and overwrite the speed with what was given back to us.
		if (mwse::lua::event::DetectSneakEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::DetectSneakEvent(detector, target, isDetected));
			if (eventData.valid()) {
				isDetected = eventData["isDetected"];
			}
		}

		return isDetected;
	}

	const auto TES3_ProcessManager_findActorsInProximity = reinterpret_cast<void(__thiscall*)(ProcessManager*, Vector3*, float, IteratedList<MobileActor*>*)>(0x5702B0);
	void ProcessManager::findActorsInProximity(Vector3 * position, float range, IteratedList<MobileActor*>* outputList) {
		TES3_ProcessManager_findActorsInProximity(this, position, range, outputList);
	}

	const auto TES3_ProcessManager_checkAlarmRadius = reinterpret_cast<void(__thiscall*)(ProcessManager*, MobileActor*, IteratedList<AIPlanner*>*)>(0x5704B0);
	void ProcessManager::checkAlarmRadius(MobileActor * actor, IteratedList<AIPlanner*> * container) {
		TES3_ProcessManager_checkAlarmRadius(this, actor, container);
	}

	const auto TES3_ProcessManager_checkPlayerDist = reinterpret_cast<void(__thiscall*)(ProcessManager*)>(0x56F730);
	void ProcessManager::checkPlayerDistance() {
		TES3_ProcessManager_checkPlayerDist(this);
	}

	const auto TES3_ProcessManager_canRest = reinterpret_cast<bool(__thiscall*)(ProcessManager*)>(0x56F810);
	bool ProcessManager::canRest() {
		return TES3_ProcessManager_canRest(this);
	}

	float ProcessManager::getAIDistanceScale() const {
		return (aiDistance - 1000.0f) / 6000.0f;
	}

	void ProcessManager::setAIDistanceScale(float scalar) {
		aiDistance = 1000.0f + 6000.0f * scalar;
	}

	const auto TES3_ProjectileController_resolveCollisions = reinterpret_cast<void(__thiscall*)(ProjectileController*, float)>(0x5753A0);
	void ProjectileController::resolveCollisions(float deltaTime) {
		// Explode flagged spell projectiles.
		criticalSection.enter("MWSE:ProjectileController::resolveCollisions");
		for (auto projectile : activeProjectiles) {
			if (projectile->patchFlagExplode && (projectile->actorFlags & MobileActorFlag::ActiveInSimulation)) {
				static_cast<MobileSpellProjectile*>(projectile)->explode();
			}
		}
		criticalSection.leave();

		TES3_ProjectileController_resolveCollisions(this, deltaTime);
	}

	const auto TES3_MobController_addMob = reinterpret_cast<void(__thiscall*)(MobController*, Reference*)>(0x5636A0);
	void MobController::addMob(Reference * reference) {
		TES3_MobController_addMob(this, reference);

		auto mobile = reference->getAttachedMobileObject();
		if (mwse::lua::event::MobileActorActivatedEvent::getEventEnabled() && mobile) {
			// Update simulation distance with an initial value before the event fires.
			auto macp = WorldController::get()->getMobilePlayer();
			mobile->simulationDistance = reference->position.distance(&macp->reference->position);

			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileActorActivatedEvent(mobile));
		}
	}

	const auto TES3_MobController_removeMob = reinterpret_cast<void(__thiscall*)(MobController*, Reference*)>(0x5637F0);
	void MobController::removeMob(Reference * reference) {
		TES3_MobController_removeMob(this, reference);

		if (mwse::lua::event::MobileActorDeactivatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileActorDeactivatedEvent(reference));
		}
	}

	void MobController::checkPlayerDistance() {
		processManager->checkPlayerDistance();
	}

	const auto TES3_MobController_addPlayerAsCollider = reinterpret_cast<void(__thiscall*)(MobController*)>(0x563640);
	void MobController::addPlayerAsCollider() {
		TES3_MobController_addPlayerAsCollider(this);
	}

	bool MobController::hasMobileCollision(const MobileActor* mobile) {
		bool result = false;
		if (mobile && (mobile->actorFlags & TES3::MobileActorFlag::ActiveInSimulation)) {
			auto node = mobile->reference->sceneNode;
			criticalSection_Mobs.enter();
			result = mobCollisionGroup->containsCollider(node);
			criticalSection_Mobs.leave();
		}
		return result;
	}

	void MobController::enableMobileCollision(MobileActor* mobile) {
		if (mobile && (mobile->actorFlags & TES3::MobileActorFlag::ActiveInSimulation)) {
			auto node = mobile->reference->sceneNode;
			criticalSection_Mobs.enter();
			if (!mobCollisionGroup->containsCollider(node)) {
				mobCollisionGroup->addCollider(node);
			}
			criticalSection_Mobs.leave();
		}
	}

	void MobController::disableMobileCollision(MobileActor* mobile) {
		if (mobile && (mobile->actorFlags & TES3::MobileActorFlag::ActiveInSimulation)) {
			auto node = mobile->reference->sceneNode;
			criticalSection_Mobs.enter();
			mobCollisionGroup->removeCollider(node);
			criticalSection_Mobs.leave();
		}
	}
}
