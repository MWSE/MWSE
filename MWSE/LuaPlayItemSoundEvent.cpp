#include "LuaPlayItemSoundEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	PlayItemSoundEvent::PlayItemSoundEvent(TES3::BaseObject* item, int state, TES3::Reference* reference) :
		ObjectFilteredEvent("playItemSound", item),
		m_Item(item),
		m_State(state),
		m_Reference(reference)
	{

	}

	sol::table PlayItemSoundEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["item"] = m_Item;
		eventData["state"] = m_State;
		eventData["reference"] = m_Reference;

		return eventData;
	}
}
