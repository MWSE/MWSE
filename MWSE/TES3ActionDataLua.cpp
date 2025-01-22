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
		usertypeDefinition["animationAttackState"] = &TES3::ActionData::animStateAttack;
		usertypeDefinition["attackSwing"] = &TES3::ActionData::attackSwing;
		usertypeDefinition["attackWasBlocked"] = &TES3::ActionData::attackWasBlocked;
		usertypeDefinition["blockingState"] = &TES3::ActionData::blockingState;
		usertypeDefinition["currentAnimationGroup"] = &TES3::ActionData::animGroupCurrentAction;
		usertypeDefinition["hitTarget"] = &TES3::ActionData::hitTarget;
		usertypeDefinition["lastBarterHoursPassed"] = &TES3::ActionData::lastBarterHoursPassed;
		usertypeDefinition["lastPositionBeforeCombat"] = &TES3::ActionData::lastPositionBeforeCombat;
		usertypeDefinition["nockedProjectile"] = sol::property(getNockedProjectile, setNockedProjectile);
		usertypeDefinition["physicalAttackType"] = &TES3::ActionData::physicalAttackType;
		usertypeDefinition["physicalDamage"] = &TES3::ActionData::physicalDamage;
		usertypeDefinition["stolenFrom"] = &TES3::ActionData::stolenFromFactionOrNPC;
		usertypeDefinition["swingTimer"] = &TES3::ActionData::swingTimer;
		usertypeDefinition["target"] = &TES3::ActionData::target;
		usertypeDefinition["walkDestination"] = &TES3::ActionData::walkDestination;

		// Deprecated properties.
		usertypeDefinition["attackDirection"] = &TES3::ActionData::physicalAttackType;
	}
}
