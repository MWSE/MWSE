#include "LuaSkillExerciseEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	SkillExerciseEvent::SkillExerciseEvent(int skillId, float progress) :
		GenericEvent("exerciseSkill"),
		m_Skill(skillId),
		m_Progress(progress)
	{

	}

	sol::table SkillExerciseEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["skill"] = m_Skill;
		eventData["progress"] = m_Progress;

		return eventData;
	}

	sol::object SkillExerciseEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_Skill;

		return options;
	}
}
