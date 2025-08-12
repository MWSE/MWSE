#include "NIDynamicEffectLua.h"

#include "LuaManager.h"

namespace mwse::lua {
	void bindNIDynamicEffect() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for NI::DynamicEffect.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::DynamicEffect>("niDynamicEffect");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNIDynamicEffect(usertypeDefinition);
		}
	}
}
