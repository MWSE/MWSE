#include "TES3GlobalVariableLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3GlobalVariable.h"

namespace mwse::lua {
	void bindTES3GlobalVariable() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::GlobalVariable>("tes3globalVariable");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
		setUserdataForTES3BaseObject(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["value"] = sol::property(&TES3::GlobalVariable::getValue_lua, &TES3::GlobalVariable::setValue_lua);
	}
}
