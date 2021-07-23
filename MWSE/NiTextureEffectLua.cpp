#include "NITextureEffectLua.h"

#include "NIDynamicEffectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NINode.h"
#include "NIRTTI.h"
#include "NISourceTexture.h"
#include "NITextureEffect.h"

namespace mwse {
	namespace lua {
		void bindNITextureEffect() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for NI::TextureEffect.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::TextureEffect>("niTextureEffect");
				usertypeDefinition["new"] = &NI::TextureEffect::create;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::DynamicEffect, NI::AVObject, NI::ObjectNET, NI::Object>();
				setUserdataForNITextureEffect(usertypeDefinition);
			}
		}
	}
}
