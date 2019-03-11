#include "LuaDetermineActionEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3CombatSession.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			DetermineActionEvent::DetermineActionEvent(TES3::CombatSession* session) :
				ObjectFilteredEvent("determineAction", session->parentActor->reference),
				m_Session(session)
			{

			}

			sol::table DetermineActionEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["session"] = m_Session;

				return eventData;
			}
		}
	}
}
