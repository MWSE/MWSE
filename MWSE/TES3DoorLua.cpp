#include "TES3DoorLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Door.h"
#include "TES3Script.h"
#include "TES3Sound.h"

namespace mwse {
	namespace lua {
		void bindTES3Door() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Door>("tes3door");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["closeSound"] = &TES3::Door::closeSound;
			usertypeDefinition["openSound"] = &TES3::Door::openSound;

			// Functions exposed as properties.
			usertypeDefinition["mesh"] = sol::property(&TES3::Door::getModelPath, &TES3::Door::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::Door::getName, &TES3::Door::setName);
			usertypeDefinition["script"] = &TES3::Door::script;

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["model"] = sol::property(&TES3::Door::getModelPath, &TES3::Door::setModelPath);
		}
	}
}
