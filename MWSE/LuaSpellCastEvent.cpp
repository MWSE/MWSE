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
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["caster"] = makeLuaObject(m_Caster->reference);
				eventData["source"] = makeLuaObject(m_Spell);
				eventData["castChance"] = m_CastChance;
				eventData["weakestSchool"] = m_WeakestSchool;

				return eventData;
			}

			sol::object SpellCastEvent::getEventOptions() {
				sol::table options = LuaManager::getInstance().createTable();

				options["filter"] = makeLuaObject(m_Spell);
				return options;
			}
		}
	}
}
