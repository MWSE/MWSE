#include "TES3ActionDataLua.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3ActionData.h"
#include "TES3MobileActor.h"
#include "TES3MobileProjectile.h"
#include "TES3Item.h"

namespace mwse::lua {
	TES3::MobileProjectile* getNockedProjectile(TES3::ActionData* self) {
		return self->nockedProjectile;
	}

	void setNockedProjectile(TES3::ActionData* self, sol::object value) {
		auto projectile = self->nockedProjectile;
		if (projectile && value == sol::nil) {
			projectile->vTable.mobileObject->destructor(projectile, true);
			self->nockedProjectile = nullptr;
		}
	}

	void bindTES3ActionData() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::ActionData>("tes3actionData");
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		usertypeDefinition["aiBehaviorState"] = &TES3::ActionData::aiBehaviorState;
		usertypeDefinition["aiPackageCount"] = &TES3::ActionData::countAIPackages;
		usertypeDefinition["animationAttackState"] = &TES3::ActionData::animStateAttack;
		usertypeDefinition["animGroupBlocking"] = &TES3::ActionData::animGroupBlocking;
		usertypeDefinition["animGroupCurrentAction"] = &TES3::ActionData::animGroupCurrentAction;
		usertypeDefinition["animGroupStunEffect"] = &TES3::ActionData::animGroupStunEffect;
		usertypeDefinition["animSectionCurrentAction"] = &TES3::ActionData::animSectionCurrentAction;
		usertypeDefinition["attackSwing"] = &TES3::ActionData::attackSwing;
		usertypeDefinition["attackWasBlocked"] = &TES3::ActionData::attackWasBlocked;
		usertypeDefinition["blockingState"] = &TES3::ActionData::blockingState;
		usertypeDefinition["currentAnimationGroup"] = &TES3::ActionData::animGroupCurrentAction;
		usertypeDefinition["dispositionCombatChange"] = &TES3::ActionData::dispositionCombatChange;
		usertypeDefinition["fightCombatChange"] = &TES3::ActionData::fightAttrCombatChange;
		usertypeDefinition["hitTarget"] = &TES3::ActionData::hitTarget;
		usertypeDefinition["lastBarterHoursPassed"] = &TES3::ActionData::lastBarterHoursPassed;
		usertypeDefinition["lastPositionBeforeCombat"] = &TES3::ActionData::lastPositionBeforeCombat;
		usertypeDefinition["lastWitnessedCrimeTimestamp"] = &TES3::ActionData::lastWitnessedCrimeTimestamp;
		usertypeDefinition["nockedProjectile"] = sol::property(getNockedProjectile, setNockedProjectile);
		usertypeDefinition["physicalAttackType"] = &TES3::ActionData::physicalAttackType;
		usertypeDefinition["physicalDamage"] = &TES3::ActionData::physicalDamage;
		usertypeDefinition["quantizedHitAngleX"] = &TES3::ActionData::quantizedHitAngleX;
		usertypeDefinition["quantizedHitAngleY"] = &TES3::ActionData::quantizedHitAngleY;
		usertypeDefinition["quantizedHitAngleZ"] = &TES3::ActionData::quantizedHitAngleZ;
		usertypeDefinition["rescaledFacingBeforeCombat"] = &TES3::ActionData::rescaledFacingBeforeCombat;
		usertypeDefinition["stolenFrom"] = &TES3::ActionData::stolenFromFactionOrNPC;
		usertypeDefinition["swingTimer"] = &TES3::ActionData::swingTimer;
		usertypeDefinition["target"] = &TES3::ActionData::target;
		usertypeDefinition["walkDestination"] = &TES3::ActionData::walkDestination;

		// Deprecated properties.
		usertypeDefinition["attackDirection"] = &TES3::ActionData::physicalAttackType;
	}
}
