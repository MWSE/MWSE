#include "LuaItemDroppedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse::lua::event {
	ItemDroppedEvent::ItemDroppedEvent(TES3::Reference* reference) :
		ObjectFilteredEvent("itemDropped", reference->baseObject),
		m_Reference(reference)
	{

	}

	sol::table ItemDroppedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["reference"] = m_Reference;

		return eventData;
	}
}
