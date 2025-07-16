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
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["skill"] = m_Skill;
		eventData["progress"] = m_Progress;

		return eventData;
	}

	sol::object SkillExerciseEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();

		options["filter"] = m_Skill;

		return options;
	}
}
