#include "NITextureLua.h"

#include "LuaManager.h"

namespace mwse::lua {
	void bindNITexture() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<NI::Texture>("niTexture");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<NI::ObjectNET, NI::Object>();
		setUserdataForNITexture(usertypeDefinition);
	}
}
