#include "TES3MobileSpellProjectile.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3DataHandler.h"
#include "TES3MagicEffectController.h"
#include "TES3MagicInstanceController.h"
#include "TES3MagicSourceInstance.h"
#include "TES3MobManager.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

#include "LuaMobileSpellProjectileActorCollisionEvent.h"
#include "LuaMobileSpellProjectileObjectCollisionEvent.h"
#include "LuaMobileSpellProjectileTerrainCollisionEvent.h"
#include "LuaMobileSpellProjectileWaterCollisionEvent.h"

namespace TES3 {
	const auto TES3_SpellProjectile_onActorCollide = reinterpret_cast<void(__thiscall*)(MobileSpellProjectile*, int)>(0x574C20);
	bool MobileSpellProjectile::onActorCollision(int collisionIndex) {
		// Grab the collision data now, it won't be available after calling the main function.
		const auto& hit = this->arrayCollisionResults[collisionIndex];
		TES3::Reference* hitReference = hit.colliderRef;
		NI::Point3 point = hit.point;
		NI::Point3 pos = hit.objectPosAtCollision;
		NI::Point3 vel = hit.velocity;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		TES3::MobileProjectile* projectileSwap = this;
		std::swap(projectileSwap, ProjectileManager::ms_CurrentlyCollidingProjectile);
		TES3_SpellProjectile_onActorCollide(this, collisionIndex);
		std::swap(projectileSwap, ProjectileManager::ms_CurrentlyCollidingProjectile);

		// Fire off our hit event.
		//! TODO: Make this into projectileHitMobile event with backup projectileHitActor for backwards compatibility.
		if (mwse::lua::event::MobileSpellProjectileActorCollisionEvent::getEventEnabled() && hitReference->baseObject->isActor()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileSpellProjectileActorCollisionEvent(this, hitReference, point, pos, vel));
		}

		return true;
	}

	const auto TES3_SpellProjectile_onStaticCollision = reinterpret_cast<bool(__thiscall*)(MobileSpellProjectile*, int, bool)>(0x574EB0);
	bool MobileSpellProjectile::onStaticCollision(int collisionIndex, bool isAvoidNode) {
		// Grab the collision data now, it won't be available after calling the main function.
		const auto& hit = this->arrayCollisionResults[collisionIndex];
		TES3::Reference* hitReference = hit.colliderRef;
		NI::Point3 point = hit.point;
		NI::Point3 pos = hit.objectPosAtCollision;
		NI::Point3 vel = hit.velocity;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		TES3::MobileProjectile* projectileSwap = this;
		std::swap(projectileSwap, ProjectileManager::ms_CurrentlyCollidingProjectile);
		bool result = TES3_SpellProjectile_onStaticCollision(this, collisionIndex, isAvoidNode);
		std::swap(projectileSwap, ProjectileManager::ms_CurrentlyCollidingProjectile);

		// Fire off our hit event.
		if (mwse::lua::event::MobileSpellProjectileObjectCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileSpellProjectileObjectCollisionEvent(this, hitReference, point, pos, vel));
		}

		return result;
	}

	const auto TES3_SpellProjectile_onTerrainCollision = reinterpret_cast<bool(__thiscall*)(MobileSpellProjectile*, int)>(0x574E40);
	bool MobileSpellProjectile::onTerrainCollision(int collisionIndex) {
		// Grab the collision data now, it won't be available after calling the main function.
		const auto& hit = this->arrayCollisionResults[collisionIndex];
		NI::Point3 point = hit.point;
		NI::Point3 pos = hit.objectPosAtCollision;
		NI::Point3 vel = hit.velocity;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		TES3::MobileProjectile* projectileSwap = this;
		std::swap(projectileSwap, ProjectileManager::ms_CurrentlyCollidingProjectile);
		bool result = TES3_SpellProjectile_onTerrainCollision(this, collisionIndex);
		std::swap(projectileSwap, ProjectileManager::ms_CurrentlyCollidingProjectile);

		// Fire off our hit event.
		if (mwse::lua::event::MobileSpellProjectileTerrainCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileSpellProjectileTerrainCollisionEvent(this, point, pos, vel));
		}

		return result;
	}

	const auto TES3_SpellProjectile_onWaterCollision = reinterpret_cast<bool(__thiscall*)(MobileSpellProjectile*, int)>(0x574DA0);
	bool MobileSpellProjectile::onWaterCollision(int collisionIndex) {
		// Grab the collision data now, it won't be available after calling the main function.
		const auto& hit = this->arrayCollisionResults[collisionIndex];
		TES3::Reference* hitReference = hit.colliderRef;
		NI::Point3 point = hit.point;
		NI::Point3 pos = hit.objectPosAtCollision;
		NI::Point3 vel = hit.velocity;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		TES3::MobileProjectile* projectileSwap = this;
		std::swap(projectileSwap, ProjectileManager::ms_CurrentlyCollidingProjectile);
		bool result = TES3_SpellProjectile_onWaterCollision(this, collisionIndex);
		std::swap(projectileSwap, ProjectileManager::ms_CurrentlyCollidingProjectile);

		// Fire off our hit event.
		if (mwse::lua::event::MobileSpellProjectileWaterCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileSpellProjectileWaterCollisionEvent(this, hitReference, point, pos, vel));
		}

		return result;
	}

	MagicSourceInstance* MobileSpellProjectile::getInstance() {
		return TES3::WorldController::get()->magicInstanceController->getInstanceFromSerial(spellInstanceSerial);
	}

	void MobileSpellProjectile::explode() {
		auto magicSourceInstance = getInstance();
		if (!magicSourceInstance) {
			return;
		}

		// Create temporary collision data for the projectile hit function. The projectile collider is itself.
		MobileObject::Collision collision;
		collision.valid = true;
		collision.time = 0;
		collision.point = reference->position;
		collision.objectPosAtCollision = reference->position;
		collision.colliderRef = reference;
		collision.collisionType = MobileObject::Collision::CollisionType::None;

		// Call projectile impact handling function.
		auto magicEffectController = DataHandler::get()->nonDynamicData->magicEffects;
		magicEffectController->spellProjectileHit(magicSourceInstance, &collision);

		// Expire projectile.
		enterLeaveSimulation(false);
		flagExpire = true;
		patchFlagExplode = false;
	}

	void MobileSpellProjectile::explodeDeferred() {
		// The explode logic is deferred to execute at the same point as projectile simulation to preserve consistency.
		patchFlagExplode = true;
	}
}
