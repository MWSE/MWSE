#pragma once

#include "LuaDisableableEvent.h"
#include "LuaObjectFilteredEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	class ScriptExecutedEvent : public ObjectFilteredEvent, public DisableableEvent<ScriptExecutedEvent> {
	public:
		ScriptExecutedEvent(TES3::Script* script, TES3::Reference* reference, TES3::ScriptVariables* variables, TES3::DialogueInfo* info, TES3::Reference* reference2);
		sol::table createEventTable();

	protected:
		TES3::Script* m_Script;
		TES3::Reference* m_Reference;
		TES3::ScriptVariables* m_Variables;
		TES3::DialogueInfo* m_Info;
		TES3::Reference* m_Reference2;
	};
}
