#include "LuaSkillRaisedEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			SkillRaisedEvent::SkillRaisedEvent(int skillId, float newLevel) :
				GenericEvent("skillRaised"),
				m_Skill(skillId),
				m_NewLevel(newLevel)
			{

			}

			sol::table SkillRaisedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["skill"] = m_Skill;
				eventData["level"] = m_NewLevel;

				return eventData;
			}

			sol::object SkillRaisedEvent::getEventOptions() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table options = LuaManager::getInstance().createTable();

				options["filter"] = m_Skill;

				return options;
			}
		}
	}
}
