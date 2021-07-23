#pragma once

#include "DynamicLuaObject.h"

#include "TES3Defines.h"
#include "TES3ScriptLua.h"

namespace mwse::lua {
	struct LuaScript : public DynamicLuaObject {
		TES3::Script* m_Script;
		TES3::Reference* m_Reference;

		LuaScript();
		LuaScript(TES3::Script* script, TES3::Reference* reference = nullptr);

		std::shared_ptr<ScriptContext> getContext() const;
	};
}
