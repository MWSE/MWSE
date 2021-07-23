#include "LuaSpellCastEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

namespace mwse {
	namespace lua {
		namespace event {
			SpellCastEvent::SpellCastEvent(TES3::Spell *spell, TES3::MobileActor *caster, float castChance, int weakestSchool) :
				GenericEvent("spellCast"),
				m_Spell(spell),
				m_Caster(caster),
				m_CastChance(castChance),
				m_WeakestSchool(weakestSchool)
			{

			}

			sol::table SpellCastEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["caster"] = m_Caster->reference;
				eventData["source"] = m_Spell;
				eventData["castChance"] = m_CastChance;
				eventData["weakestSchool"] = m_WeakestSchool;

				return eventData;
			}

			sol::object SpellCastEvent::getEventOptions() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table options = state.create_table();

				options["filter"] = m_Spell;

				return options;
			}

			bool SpellCastEvent::m_EventEnabled = false;
		}
	}
}
