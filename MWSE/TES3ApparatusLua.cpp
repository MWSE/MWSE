#include "TES3ApparatusLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Apparatus.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		void bindTES3Apparatus() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Apparatus>("tes3apparatus");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["type"] = &TES3::Apparatus::type;
			usertypeDefinition["quality"] = &TES3::Apparatus::quality;
			usertypeDefinition["value"] = &TES3::Apparatus::value;
			usertypeDefinition["weight"] = &TES3::Apparatus::weight;

			// Functions exposed as properties.
			usertypeDefinition["icon"] = sol::property(&TES3::Apparatus::getIconPath, &TES3::Apparatus::setIconPath);
			usertypeDefinition["mesh"] = sol::property(&TES3::Apparatus::getModelPath, &TES3::Apparatus::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::Apparatus::getName, &TES3::Apparatus::setName);
			usertypeDefinition["script"] = &TES3::Apparatus::script;

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["model"] = sol::property(&TES3::Apparatus::getModelPath, &TES3::Apparatus::setModelPath);
		}
	}
}
