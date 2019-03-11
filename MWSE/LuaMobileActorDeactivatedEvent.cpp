#include "LuaMobileActorDeactivatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			MobileActorDeactivatedEvent::MobileActorDeactivatedEvent(TES3::Reference * reference) :
				ObjectFilteredEvent("mobileDeactivated", reference),
				m_Reference(reference)
			{

			}

			sol::table MobileActorDeactivatedEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["reference"] = lua::makeLuaObject(m_Reference);

				return eventData;
			}
		}
	}
}
