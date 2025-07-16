#include "LuaConsoleReferenceChangedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse::lua::event {
	ConsoleReferenceChangedEvent::ConsoleReferenceChangedEvent(TES3::Reference* reference) :
		ObjectFilteredEvent("consoleReferenceChanged", reference),
		m_Reference(reference)
	{

	}

	sol::table ConsoleReferenceChangedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["reference"] = m_Reference;

		return eventData;
	}
}
