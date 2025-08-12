#include "LuaAttackHitEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ActorAnimationController.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	AttackHitEvent::AttackHitEvent(TES3::MobileActor* mobileActor) :
		ObjectFilteredEvent("attackHit", mobileActor->reference),
		m_MobileActor(mobileActor)
	{

	}

	sol::table AttackHitEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["mobile"] = m_MobileActor;
		eventData["reference"] = m_MobileActor->reference;

		TES3::MobileActor* target = m_MobileActor->actionData.hitTarget;
		if (target) {
			eventData["targetMobile"] = target;
			eventData["targetReference"] = target->reference;
		}

		return eventData;
	}
}
