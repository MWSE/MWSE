#include "TES3Birthsign.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

namespace mwse {
	namespace lua {
		void bindTES3Birthsign() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Birthsign>("tes3birthsign");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
			setUserdataForTES3BaseObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["name"] = sol::readonly_property(&TES3::Birthsign::getName);
			usertypeDefinition["texturePath"] = &TES3::Birthsign::texturePath;
			usertypeDefinition["spells"] = sol::readonly_property(&TES3::Birthsign::spellList);

			usertypeDefinition["description"] = sol::readonly_property(&TES3::Birthsign::getAndFreeDescription);
		}
	}
}
