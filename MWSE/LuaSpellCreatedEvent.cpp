#include "LuaSpellCreatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Spell.h"

namespace mwse {
	namespace lua {
		namespace event {
			SpellCreatedEvent::SpellCreatedEvent(TES3::Spell* spell, const char* source) :
				GenericEvent("spellCreated"),
				m_Spell(spell),
				m_Source(source)
			{

			}

			sol::table SpellCreatedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["spell"] = m_Spell;
				eventData["source"] = m_Source;

				return eventData;
			}

			sol::object SpellCreatedEvent::getEventOptions() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table options = state.create_table();

				options["filter"] = m_Source;

				return options;
			}

			bool SpellCreatedEvent::m_EventEnabled = false;
		}
	}
}
