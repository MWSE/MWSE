#include "LuaSpellResistEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Spell.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			SpellResistEvent::SpellResistEvent(TES3::MagicSourceInstance * spellInstance, TES3::MagicEffectInstance * effectInstance, int effectIndex, int resistAttribute) :
				GenericEvent("spellResist"),
				m_MagicSourceInstance(spellInstance),
				m_EffectInstance(effectInstance),
				m_EffectIndex(effectIndex),
				m_ResistAttribute(resistAttribute)
			{

			}

			sol::table SpellResistEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["caster"] = makeLuaObject(m_MagicSourceInstance->caster);
				eventData["target"] = makeLuaObject(m_EffectInstance->target);
				eventData["resistedPercent"] = m_EffectInstance->resistedPercent;

				eventData["source"] = makeLuaObject(m_MagicSourceInstance->sourceCombo.source.asGeneric);
				eventData["sourceInstance"] = makeLuaObject(m_MagicSourceInstance);
				eventData["effectIndex"] = m_EffectIndex;
				eventData["effectInstance"] = m_EffectInstance;
				eventData["resistAttribute"] = m_ResistAttribute;

				// Get the specific effect on the source.
				TES3::Effect * effects = m_MagicSourceInstance->sourceCombo.getSourceEffects();
				if (effects) {
					eventData["effect"] = effects[m_EffectIndex];
				}

				// Get cast chance if it's a spell.
				if (m_MagicSourceInstance->sourceCombo.sourceType == TES3::MagicSourceType::Spell || m_MagicSourceInstance->sourceCombo.sourceType == TES3::MagicSourceType::Enchantment) {
					eventData["spellCastChance"] = m_MagicSourceInstance->sourceCombo.source.asSpell->calculateCastChance(m_MagicSourceInstance->caster);
				}

				return eventData;
			}

			sol::object SpellResistEvent::getEventOptions() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table options = LuaManager::getInstance().createTable();

				options["filter"] = makeLuaObject(m_MagicSourceInstance->sourceCombo.source.asGeneric);

				return options;
			}
		}
	}
}
