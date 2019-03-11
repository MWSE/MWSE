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
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["mobile"] = makeLuaObject(m_MobileObject);
				eventData["reference"] = makeLuaObject(m_MobileObject->reference);
				eventData["target"] = makeLuaObject(m_TargetReference);

				return eventData;
			}
		}
	}
}
