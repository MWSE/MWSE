#include "LuaSpellCastedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

namespace mwse {
	namespace lua {
		namespace event {
			SpellCastedEvent::SpellCastedEvent(TES3::MagicSourceInstance* magicInstance, bool success, int expGainSchool) :
				GenericEvent("spellCasted"),
				m_MagicSourceInstance(magicInstance),
				m_ExpGainSchool(expGainSchool)
			{
				if (success) {
					m_EventName = "spellCasted";
				}
				else {
					m_EventName = "spellCastedFailure";
				}
			}

			sol::table SpellCastedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["caster"] = makeLuaObject(m_MagicSourceInstance->caster);
				eventData["target"] = makeLuaObject(m_MagicSourceInstance->target);
				eventData["source"] = makeLuaObject(m_MagicSourceInstance->sourceCombo.source.asSpell);
				eventData["sourceInstance"] = makeLuaObject(m_MagicSourceInstance);
				eventData["expGainSchool"] = m_ExpGainSchool;

				return eventData;
			}

			sol::object SpellCastedEvent::getEventOptions() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table options = LuaManager::getInstance().createTable();

				options["filter"] = makeLuaObject(m_MagicSourceInstance->sourceCombo.source.asGeneric);
				return options;
			}
		}
	}
}
