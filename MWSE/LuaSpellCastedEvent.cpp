#include "LuaSpellCastedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

namespace mwse::lua::event {
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
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["caster"] = m_MagicSourceInstance->caster;
		eventData["target"] = m_MagicSourceInstance->target;
		eventData["source"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		eventData["sourceInstance"] = m_MagicSourceInstance;
		eventData["expGainSchool"] = m_ExpGainSchool;

		return eventData;
	}

	sol::object SpellCastedEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		return options;
	}
}
