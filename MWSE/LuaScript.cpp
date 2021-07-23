#include "LuaScript.h"

#include "TES3Reference.h"

namespace mwse::lua {
	LuaScript::LuaScript()
		: DynamicLuaObject(),
		m_Script(nullptr),
		m_Reference(nullptr)
	{

	}

	LuaScript::LuaScript(TES3::Script* script, TES3::Reference* reference)
		: DynamicLuaObject(),
		m_Script(script),
		m_Reference(reference)
	{

	}

	std::shared_ptr<ScriptContext> LuaScript::getContext() const {
		auto vars = m_Reference ? m_Reference->getScriptVariables() : nullptr;
		return std::make_shared<ScriptContext>(m_Script, vars);
	}
}
