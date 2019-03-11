#include "LuaAttackEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ActorAnimationData.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			AttackEvent::AttackEvent(TES3::ActorAnimationData* animData) :
				ObjectFilteredEvent("attack", animData->mobileActor->reference),
				m_AnimationData(animData)
			{

			}

			sol::table AttackEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["mobile"] = makeLuaObject(m_AnimationData->mobileActor);
				eventData["reference"] = makeLuaObject(m_AnimationData->mobileActor->reference);

				TES3::MobileActor* target = m_AnimationData->mobileActor->actionData.hitTarget;
				if (target) {
					eventData["targetMobile"] = makeLuaObject(target);
					eventData["targetReference"] = makeLuaObject(target->reference);
				}

				return eventData;
			}
		}
	}
}
