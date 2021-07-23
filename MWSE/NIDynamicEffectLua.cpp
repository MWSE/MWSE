#include "NIDynamicEffectLua.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		void bindNIDynamicEffect() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for NI::DynamicEffect.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::DynamicEffect>("niDynamicEffect");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::AVObject, NI::ObjectNET, NI::Object>();
				setUserdataForNIDynamicEffect(usertypeDefinition);
			}
		}
	}
}
