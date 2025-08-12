#include "LuaSkillRaisedEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	SkillRaisedEvent::SkillRaisedEvent(int skillId, float newLevel, const char* source) :
		GenericEvent("skillRaised"),
		m_Skill(skillId),
		m_NewLevel(newLevel),
		m_Source(source)
	{

	}

	sol::table SkillRaisedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["skill"] = m_Skill;
		eventData["level"] = m_NewLevel;
		eventData["source"] = m_Source;

		return eventData;
	}

	sol::object SkillRaisedEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();

		options["filter"] = m_Skill;

		return options;
	}
}
