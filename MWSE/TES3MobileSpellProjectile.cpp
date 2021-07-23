#include "TES3MobileSpellProjectile.h"

#include "TES3DataHandler.h"
#include "TES3MagicEffectController.h"
#include "TES3MagicInstanceController.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace TES3 {
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
