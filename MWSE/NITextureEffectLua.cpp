#include "NITextureEffectLua.h"

#include "NIDynamicEffectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NINode.h"
#include "NITextureEffect.h"

import NIRTTI;
import NISourceTexture;

namespace mwse::lua {
	void bindNITextureEffect() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for NI::TextureEffect.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::TextureEffect>("niTextureEffect");
			usertypeDefinition["new"] = &NI::TextureEffect::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::DynamicEffect, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNITextureEffect(usertypeDefinition);
		}
	}
}
