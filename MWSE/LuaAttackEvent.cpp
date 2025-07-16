#include "LuaAttackEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ActorAnimationController.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	AttackEvent::AttackEvent(TES3::ActorAnimationController* animController) :
		ObjectFilteredEvent("attack", animController->mobileActor->reference),
		m_AnimationController(animController)
	{

	}

	sol::table AttackEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["mobile"] = m_AnimationController->mobileActor;
		eventData["reference"] = m_AnimationController->mobileActor->reference;

		TES3::MobileActor* target = m_AnimationController->mobileActor->actionData.hitTarget;
		if (target) {
			eventData["targetMobile"] = target;
			eventData["targetReference"] = target->reference;
		}

		return eventData;
	}
}
