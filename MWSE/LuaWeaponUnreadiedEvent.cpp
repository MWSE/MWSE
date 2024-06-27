#include "LuaWeaponUnreadiedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse::lua::event {
	WeaponUnreadiedEvent::WeaponUnreadiedEvent(TES3::Reference* reference) :
		ObjectFilteredEvent("weaponUnreadied", reference),
		m_Reference(reference)
	{

	}

	sol::table WeaponUnreadiedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["reference"] = m_Reference;

		return eventData;
	}
}
