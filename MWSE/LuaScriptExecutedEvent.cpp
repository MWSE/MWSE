#include "LuaScriptExecutedEvent.h"

#include "LuaManager.h"
#include "ScriptUtil.h"

#include "TES3DialogueInfo.h"
#include "TES3Reference.h"
#include "TES3Script.h"

namespace mwse::lua::event {
	ScriptExecutedEvent::ScriptExecutedEvent(TES3::Script* script, TES3::Reference* reference, TES3::ScriptVariables* variables, TES3::DialogueInfo* info, TES3::Reference* reference2) :
		ObjectFilteredEvent("scriptExecuted", script),
		m_Script(script),
		m_Reference(reference),
		m_Variables(variables),
		m_Info(info),
		m_Reference2(reference2)
	{

	}

	sol::table ScriptExecutedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["script"] = m_Script;
		eventData["reference"] = m_Reference;
		eventData["reference2"] = m_Reference2;
		eventData["info"] = m_Info;

		auto variables = state.create_table();
		const auto scriptVariables = m_Variables ? m_Variables : mwse::mwscript::getLocalScriptVariables();
		if (m_Script && scriptVariables) {
			for (auto i = 0u; i < m_Script->header.shortCount; ++i) {
				const char* varName = m_Script->shortVarNamePointers[i];
				variables[varName] = state.create_table_with("type", 's', "index", i, "value", scriptVariables->shortVarValues[i]);
			}

			for (auto i = 0u; i < m_Script->header.longCount; ++i) {
				const char* varName = m_Script->longVarNamePointers[i];
				variables[varName] = state.create_table_with("type", 'l', "index", i, "value", scriptVariables->longVarValues[i]);
			}

			for (auto i = 0u; i < m_Script->header.floatCount; ++i) {
				const char* varName = m_Script->floatVarNamePointers[i];
				variables[varName] = state.create_table_with("type", 'f', "index", i, "value", scriptVariables->floatVarValues[i]);
			}
		}
		eventData["variables"] = variables;

		return eventData;
	}
}
