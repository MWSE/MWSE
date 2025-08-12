#include "TES3MobileCreature.h"

#include "LuaManager.h"

#include "LuaCalcMovementSpeedEvent.h"

#include "TES3ActorAnimationController.h"

namespace TES3 {
	const auto TES3_MobileCreature_calcWalkSpeed = reinterpret_cast<float(__thiscall*)(MobileCreature*)>(0x55AAF0);
	float MobileCreature::calculateWalkSpeed() {
		// Call the original function to get the default walk value.
		float speed = TES3_MobileCreature_calcWalkSpeed(this);

		// Launch our event, and overwrite the speed with what was given back to us.
		if (mwse::lua::event::CalculateMovementSpeed::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CalculateMovementSpeed(mwse::lua::event::CalculateMovementSpeed::Walk, this, speed));
			if (eventData.valid()) {
				speed = eventData["speed"];
			}
		}

		return speed;
	}

	float MobileCreature::calculateMovementSpeedFromAnimationData() {
		return animationController.asActor->calculateMovementSpeed();
	}

	std::reference_wrapper<Statistic[3]> MobileCreature::getSkillStatistics() {
		return std::ref(skills);
	}

	Statistic* MobileCreature::getCombatSkill() {
		return &skills[CreatureSkillID::Combat];
	}

	Statistic* MobileCreature::getMagicSkill() {
		return &skills[CreatureSkillID::Magic];
	}

	Statistic* MobileCreature::getStealthSkill() {
		return &skills[CreatureSkillID::Stealth];
	}

}
