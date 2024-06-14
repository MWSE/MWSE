#include "LuaMagicCastedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	MagicCastedEvent::MagicCastedEvent(TES3::MagicSourceInstance* magicInstance) :
		GenericEvent("magicCasted"),
		m_MagicSourceInstance(magicInstance)
	{

	}

	sol::table MagicCastedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["caster"] = m_MagicSourceInstance->caster;
		eventData["target"] = m_MagicSourceInstance->target;
		eventData["source"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		eventData["sourceInstance"] = m_MagicSourceInstance;

		return eventData;
	}

	sol::object MagicCastedEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_MagicSourceInstance->sourceCombo.source.asGeneric;
		return options;
	}
}
