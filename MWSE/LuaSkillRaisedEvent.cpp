#include "LuaSkillRaisedEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			SkillRaisedEvent::SkillRaisedEvent(int skillId, float newLevel, const char* source) :
				GenericEvent("skillRaised"),
				m_Skill(skillId),
				m_NewLevel(newLevel),
				m_Source(source)
			{

			}

			sol::table SkillRaisedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["skill"] = m_Skill;
				eventData["level"] = m_NewLevel;
				eventData["source"] = m_Source;

				return eventData;
			}

			sol::object SkillRaisedEvent::getEventOptions() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table options = state.create_table();

				options["filter"] = m_Skill;

				return options;
			}

			bool SkillRaisedEvent::m_EventEnabled = false;
		}
	}
}
