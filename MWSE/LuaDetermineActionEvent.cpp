#include "LuaDetermineActionEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3CombatSession.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	DetermineActionEvent::DetermineActionEvent(TES3::CombatSession* session) :
		ObjectFilteredEvent("determineAction", session->parentActor->reference),
		m_Session(session)
	{

	}

	sol::table DetermineActionEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["session"] = m_Session;

		return eventData;
	}
}
