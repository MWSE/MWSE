#include "LuaCalcSoulValueEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Creature.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CalculateSoulValueEvent::CalculateSoulValueEvent(TES3::Actor* actor, sol::optional<int> value) :
		ObjectFilteredEvent("calcSoulValue", actor),
		m_Actor(actor),
		m_Value(value)
	{

	}

	sol::table CalculateSoulValueEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["actor"] = m_Actor;
		eventData["value"] = m_Value;

		return eventData;
	}
}
