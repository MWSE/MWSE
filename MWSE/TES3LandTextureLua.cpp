#include "TES3LandTextureLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "NISourceTexture.h"
#include "TES3LandTexture.h"

namespace mwse::lua {
	void bindTES3LandTexture() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::LandTexture
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::LandTexture>("tes3landTexture");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
			setUserdataForTES3BaseObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["filename"] = sol::readonly_property(&TES3::LandTexture::texturePath);
			usertypeDefinition["id"] = sol::readonly_property(&TES3::LandTexture::id);
			usertypeDefinition["index"] = sol::readonly_property(&TES3::LandTexture::indexInTexturesList);
			usertypeDefinition["texture"] = sol::readonly_property(&TES3::LandTexture::texture);
		}
	}
}