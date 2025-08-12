#include "NIRendererLua.h"
#include "NIObjectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIRenderer.h"

namespace mwse::lua {
	void bindNIRenderer() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<NI::Renderer>("niRenderer");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
		setUserdataForNIObject(usertypeDefinition);

		// Basic function binding.
		usertypeDefinition["takeScreenshot"] = &NI::Renderer::takeScreenshot;
		usertypeDefinition["setRenderTarget"] = &NI::Renderer::setRenderTarget;
	}
}
