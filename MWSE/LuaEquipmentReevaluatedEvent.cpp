#include "LuaEquipmentReevaluatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	EquipmentReevaluatedEvent::EquipmentReevaluatedEvent(TES3::Actor* actor) :
		ObjectFilteredEvent("equipmentReevaluated", actor->getBaseActor()),
		m_Actor(actor)
	{

	}

	sol::table EquipmentReevaluatedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["actor"] = m_Actor;
		eventData["reference"] = m_Actor->getReference();
		eventData["mobile"] = m_Actor->getMobile();

		return eventData;
	}
}
