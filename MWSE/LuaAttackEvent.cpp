#include "LuaAttackEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ActorAnimationController.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			AttackEvent::AttackEvent(TES3::ActorAnimationController* animController) :
				ObjectFilteredEvent("attack", animController->mobileActor->reference),
				m_AnimationController(animController)
			{

			}

			sol::table AttackEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["mobile"] = m_AnimationController->mobileActor;
				eventData["reference"] = m_AnimationController->mobileActor->reference;

				TES3::MobileActor* target = m_AnimationController->mobileActor->actionData.hitTarget;
				if (target) {
					eventData["targetMobile"] = target;
					eventData["targetReference"] = target->reference;
				}

				return eventData;
			}

			bool AttackEvent::m_EventEnabled = false;
		}
	}
}
