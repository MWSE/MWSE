#include "LuaActivateEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse::lua::event {
	ActivateEvent::ActivateEvent(TES3::Reference* activator, TES3::Reference* target) :
		ObjectFilteredEvent("activate", target),
		m_Activator(activator),
		m_Target(target)
	{

	}

	sol::table ActivateEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["activator"] = m_Activator;
		eventData["target"] = m_Target;

		return eventData;
	}
}
