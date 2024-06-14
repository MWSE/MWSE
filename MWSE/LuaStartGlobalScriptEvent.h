#pragma once

#include "LuaDisableableEvent.h"
#include "LuaObjectFilteredEvent.h"

#include "TES3Reference.h"
#include "TES3Script.h"

namespace mwse::lua::event {
	class StartGlobalScriptEvent : public ObjectFilteredEvent, public DisableableEvent<StartGlobalScriptEvent> {
	public:
		StartGlobalScriptEvent(TES3::Script* script, TES3::Reference* reference);
		sol::table createEventTable();

	protected:
		TES3::Script* m_Script;
		TES3::Reference* m_Reference;
	};
}
