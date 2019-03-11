#include "LuaSkillExerciseEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			SkillExerciseEvent::SkillExerciseEvent(int skillId, float progress) :
				GenericEvent("exerciseSkill"),
				m_Skill(skillId),
				m_Progress(progress)
			{

			}

			sol::table SkillExerciseEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["skill"] = m_Skill;
				eventData["progress"] = m_Progress;

				return eventData;
			}

			sol::object SkillExerciseEvent::getEventOptions() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table options = LuaManager::getInstance().createTable();

				options["filter"] = m_Skill;

				return options;
			}
		}
	}
}
