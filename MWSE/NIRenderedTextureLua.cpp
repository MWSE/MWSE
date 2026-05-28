#include "NIRenderedTextureLua.h"
#include "NIObjectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIRenderedTexture.h"

namespace mwse::lua {
	void bindNIRenderedTexture() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<NI::RenderedTexture>("niRenderedTexture");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<NI::Texture, NI::ObjectNET, NI::Object>();
		setUserdataForNIObject(usertypeDefinition);

		// Basic function binding.
		usertypeDefinition["create"] = &NI::RenderedTexture::create;
		usertypeDefinition["readback"] = &NI::RenderedTexture::readback;
	}
}
