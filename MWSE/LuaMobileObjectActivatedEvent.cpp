#include "LuaMobileObjectActivatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileObject.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	MobileObjectActivatedEvent::MobileObjectActivatedEvent(TES3::MobileObject* mobile) :
		ObjectFilteredEvent("mobileActivated", mobile->reference),
		m_Mobile(mobile)
	{

	}

	sol::table MobileObjectActivatedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["mobile"] = m_Mobile;
		eventData["reference"] = m_Mobile->reference;

		return eventData;
	}
}
