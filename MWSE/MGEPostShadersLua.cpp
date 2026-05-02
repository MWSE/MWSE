#include "MGEPostShadersLua.h"

#include "LuaManager.h"

#include "MGEPostShaders.h"

namespace mwse::lua {
	void bindMGEPostShaders() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		sol::state& state = stateHandle.getState();

		// mge::ShaderHandle
		{
			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<mge::ShaderHandleLua>("mgeShaderHandle");
			usertypeDefinition["new"] = sol::no_constructor;

			usertypeDefinition[sol::meta_function::to_string] = &mge::ShaderHandleLua::getName;
			usertypeDefinition["__tojson"] = &mge::ShaderHandleLua::toJson;

			// Allow variables to be get/set using their variable name.
			usertypeDefinition[sol::meta_function::index] = &mge::ShaderHandleLua::getVariable;
			usertypeDefinition[sol::meta_function::new_index] = &mge::ShaderHandleLua::setVariable;

			// Properties.
			usertypeDefinition["enabled"] = sol::property(&mge::ShaderHandleLua::getEnabled, &mge::ShaderHandleLua::setEnabled);
			usertypeDefinition["name"] = sol::readonly_property(&mge::ShaderHandleLua::getName);
			usertypeDefinition["orderPriority"] = sol::property(&mge::ShaderHandleLua::getPriority, &mge::ShaderHandleLua::setPriority);
			usertypeDefinition["variables"] = sol::readonly_property(&mge::ShaderHandleLua::listVariables);

			// Functions.
			usertypeDefinition["reload"] = &mge::ShaderHandleLua::reload;
		}
	}
}
