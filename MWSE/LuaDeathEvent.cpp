#include "LuaDeathEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	DeathEvent::DeathEvent(TES3::MobileActor* mobileActor) :
		ObjectFilteredEvent("death", mobileActor->reference),
		m_MobileActor(mobileActor)
	{

	}

	sol::table DeathEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["mobile"] = m_MobileActor;
		eventData["reference"] = m_MobileActor->reference;

		return eventData;
	}
}
