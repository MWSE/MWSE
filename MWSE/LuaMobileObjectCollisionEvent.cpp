#include "LuaMobileObjectCollisionEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileObject.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			MobileObjectCollisionEvent::MobileObjectCollisionEvent(TES3::MobileObject* mobileObject, TES3::Reference* targetReference) :
				ObjectFilteredEvent("collision", mobileObject->reference),
				m_MobileObject(mobileObject),
				m_TargetReference(targetReference)
			{

			}

			sol::table MobileObjectCollisionEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["mobile"] = m_MobileObject;
				eventData["reference"] = m_MobileObject->reference;
				eventData["target"] = m_TargetReference;

				return eventData;
			}

			bool MobileObjectCollisionEvent::m_EventEnabled = false;
		}
	}
}
