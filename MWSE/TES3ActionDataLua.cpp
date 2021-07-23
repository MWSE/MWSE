#include "TES3ActionDataLua.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3ActionData.h"
#include "TES3MobileActor.h"
#include "TES3Item.h"

namespace mwse {
	namespace lua {
		void bindTES3ActionData() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ActionData>("tes3actionData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["aiBehaviorState"] = &TES3::ActionData::aiBehaviourState;
			usertypeDefinition["animationAttackState"] = &TES3::ActionData::animStateAttack;
			usertypeDefinition["attackSwing"] = &TES3::ActionData::attackSwing;
			usertypeDefinition["currentAnimationGroup"] = &TES3::ActionData::currentAnimGroup;
			usertypeDefinition["hitTarget"] = &TES3::ActionData::hitTarget;
			usertypeDefinition["lastBarterHoursPassed"] = &TES3::ActionData::lastBarterHoursPassed;
			usertypeDefinition["nockedProjectile"] = &TES3::ActionData::nockedProjectile;
			usertypeDefinition["physicalAttackType"] = &TES3::ActionData::physicalAttackType;
			usertypeDefinition["physicalDamage"] = &TES3::ActionData::physicalDamage;
			usertypeDefinition["stolenFrom"] = &TES3::ActionData::stolenFromFactionOrNPC;
			usertypeDefinition["target"] = &TES3::ActionData::target;
			usertypeDefinition["walkDestination"] = &TES3::ActionData::walkDestination;

			// Deprecated properties.
			usertypeDefinition["attackDirection"] = &TES3::ActionData::physicalAttackType;
		}
	}
}
