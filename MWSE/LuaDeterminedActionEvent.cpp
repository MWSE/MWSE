#include "LuaDeterminedActionEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3CombatSession.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			DeterminedActionEvent::DeterminedActionEvent(TES3::CombatSession* session) :
				ObjectFilteredEvent("determinedAction", session->parentActor->reference),
				m_Session(session)
			{

			}

			sol::table DeterminedActionEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["session"] = m_Session;

				return eventData;
			}
		}
	}
}
