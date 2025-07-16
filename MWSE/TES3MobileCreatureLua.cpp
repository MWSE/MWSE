#include "TES3MobileCreatureLua.h"

#include "LuaManager.h"
#include "TES3MobileActorLua.h"

#include "TES3Alchemy.h"
#include "TES3Enchantment.h"
#include "TES3MobileCreature.h"
#include "TES3Creature.h"
#include "TES3Spell.h"
#include "TES3Statistic.h"

namespace mwse::lua {
	void bindTES3MobileCreature() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::MobileCreature>("tes3mobileCreature");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::MobileActor, TES3::MobileObject>();
		setUserdataForTES3MobileActor(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["object"] = sol::readonly_property(&TES3::MobileCreature::creatureInstance);
		usertypeDefinition["skills"] = sol::property(&TES3::MobileCreature::getSkillStatistics);

		// Allow read access to movement speeds.
		usertypeDefinition["moveSpeed"] = sol::readonly_property(&TES3::MobileCreature::calculateMovementSpeedFromAnimationData);
		usertypeDefinition["walkSpeed"] = sol::readonly_property(&TES3::MobileCreature::calculateWalkSpeed);
		usertypeDefinition["runSpeed"] = sol::readonly_property(&TES3::MobileCreature::calculateRunSpeed);
		usertypeDefinition["swimSpeed"] = sol::readonly_property(&TES3::MobileCreature::calculateSwimSpeed);
		usertypeDefinition["swimRunSpeed"] = sol::readonly_property(&TES3::MobileCreature::calculateSwimRunSpeed);
		usertypeDefinition["flySpeed"] = sol::readonly_property(&TES3::MobileCreature::calculateFlySpeed);

		// Friendly access to skills.
		usertypeDefinition["combat"] = sol::readonly_property(&TES3::MobileCreature::getCombatSkill);
		usertypeDefinition["magic"] = sol::readonly_property(&TES3::MobileCreature::getMagicSkill);
		usertypeDefinition["stealth"] = sol::readonly_property(&TES3::MobileCreature::getStealthSkill);
	}
}
