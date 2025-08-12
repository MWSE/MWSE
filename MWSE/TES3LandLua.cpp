#include "TES3LandLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Land.h"

namespace mwse::lua {
	void bindTES3Land() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::Land
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Land>("tes3land");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
			setUserdataForTES3BaseObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["flags"] = sol::readonly_property(&TES3::Land::flags);
			usertypeDefinition["gridX"] = sol::readonly_property(&TES3::Land::gridX);
			usertypeDefinition["gridY"] = sol::readonly_property(&TES3::Land::gridY);
			usertypeDefinition["minHeight"] = sol::readonly_property(&TES3::Land::minHeight);
			usertypeDefinition["maxHeight"] = sol::readonly_property(&TES3::Land::maxHeight);
			usertypeDefinition["sceneNode"] = sol::readonly_property(&TES3::Land::sceneNode);

			// Functions exposed as properties.
			usertypeDefinition["textureIndices"] = sol::readonly_property(&TES3::Land::getTextureIndices);
		}
	}
}