#include "LuaStartGlobalScriptEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"
#include "TES3Script.h"

namespace mwse::lua::event {
	StartGlobalScriptEvent::StartGlobalScriptEvent(TES3::Script* script, TES3::Reference* reference) :
		ObjectFilteredEvent("startGlobalScript", script),
		m_Script(script),
		m_Reference(reference)
	{

	}

	sol::table StartGlobalScriptEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["script"] = m_Script;
		eventData["reference"] = m_Reference;

		return eventData;
	}
}
