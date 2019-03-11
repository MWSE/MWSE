#include "LuaActivateEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			ActivateEvent::ActivateEvent(TES3::Reference* activator, TES3::Reference* target) :
				ObjectFilteredEvent("activate", target),
				m_Activator(activator),
				m_Target(target)
			{

			}

			sol::table ActivateEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["activator"] = makeLuaObject(m_Activator);
				eventData["target"] = makeLuaObject(m_Target);

				return eventData;
			}
		}
	}
}
