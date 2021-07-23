#include "TES3ProbeLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Probe.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		void bindTES3Probe() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Probe>("tes3probe");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["maxCondition"] = &TES3::Probe::maxCondition;
			usertypeDefinition["script"] = &TES3::Probe::script;
			usertypeDefinition["quality"] = &TES3::Probe::quality;
			usertypeDefinition["value"] = &TES3::Probe::value;
			usertypeDefinition["weight"] = &TES3::Probe::weight;

			// Functions exposed as properties.
			usertypeDefinition["icon"] = sol::property(&TES3::Probe::getIconPath, &TES3::Probe::setIconPath);
			usertypeDefinition["mesh"] = sol::property(&TES3::Probe::getModelPath, &TES3::Probe::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::Probe::getName, &TES3::Probe::setName);

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["condition"] = &TES3::Probe::maxCondition;
			usertypeDefinition["model"] = sol::property(&TES3::Probe::getModelPath, &TES3::Probe::setModelPath);
		}
	}
}
