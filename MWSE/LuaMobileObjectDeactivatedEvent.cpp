#include "LuaMobileObjectDeactivatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileObject.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	MobileObjectDeactivatedEvent::MobileObjectDeactivatedEvent(TES3::MobileObject* mobile) :
		ObjectFilteredEvent("mobileDeactivated", mobile->reference),
		m_Mobile(mobile)
	{

	}

	sol::table MobileObjectDeactivatedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["mobile"] = m_Mobile;
		eventData["reference"] = m_Mobile->reference;

		return eventData;
	}
}
