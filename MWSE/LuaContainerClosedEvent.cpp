#include "LuaContainerClosedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse::lua::event {
	ContainerClosedEvent::ContainerClosedEvent(TES3::Reference* reference) :
		ObjectFilteredEvent("containerClosed", reference->baseObject),
		m_Reference(reference)
	{

	}

	sol::table ContainerClosedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["reference"] = m_Reference;

		return eventData;
	}
}
