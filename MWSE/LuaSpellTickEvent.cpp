#include "LuaSpellTickEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			SpellTickEvent::SpellTickEvent(int effectId, TES3::MagicSourceInstance * sourceInstance, float deltaTime, TES3::MagicEffectInstance * effectInstance, int effectIndex) :
				GenericEvent("spellTick"),
				m_EffectId(effectId),
				m_SourceInstance(sourceInstance),
				m_DeltaTime(deltaTime),
				m_EffectInstance(effectInstance),
				m_EffectIndex(effectIndex)
			{

			}

			sol::table SpellTickEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["caster"] = makeLuaObject(m_SourceInstance->caster);
				eventData["target"] = makeLuaObject(m_EffectInstance->target);

				eventData["effectId"] = m_EffectId;
				eventData["source"] = makeLuaObject(m_SourceInstance->sourceCombo.source.asGeneric);
				eventData["sourceInstance"] = makeLuaObject(m_SourceInstance);
				eventData["deltaTime"] = m_DeltaTime;
				eventData["effectIndex"] = m_EffectIndex;
				eventData["effectInstance"] = m_EffectInstance;

				// Get the specific effect on the source.
				TES3::Effect * effects = m_SourceInstance->sourceCombo.getSourceEffects();
				if (effects) {
					eventData["effect"] = effects[m_EffectIndex];
				}

				return eventData;
			}

			sol::object SpellTickEvent::getEventOptions() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table options = LuaManager::getInstance().createTable();

				options["filter"] = makeLuaObject(m_SourceInstance->sourceCombo.source.asGeneric);

				return options;
			}
		}
	}
}
