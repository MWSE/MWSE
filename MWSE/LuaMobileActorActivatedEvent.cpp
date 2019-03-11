#include "LuaMobileActorActivatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			MobileActorActivatedEvent::MobileActorActivatedEvent(TES3::Reference * reference) :
				ObjectFilteredEvent("mobileActivated", reference),
				m_Reference(reference)
			{

			}

			sol::table MobileActorActivatedEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["reference"] = lua::makeLuaObject(m_Reference);

				return eventData;
			}
		}
	}
}
