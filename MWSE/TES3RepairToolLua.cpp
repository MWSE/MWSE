#include "TES3RepairToolLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3RepairTool.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		void bindTES3RepairTool() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::RepairTool>("tes3repairTool");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["maxCondition"] = &TES3::RepairTool::maxCondition;
			usertypeDefinition["quality"] = &TES3::RepairTool::quality;
			usertypeDefinition["script"] = &TES3::RepairTool::script;
			usertypeDefinition["value"] = &TES3::RepairTool::value;
			usertypeDefinition["weight"] = &TES3::RepairTool::weight;

			// Functions exposed as properties.
			usertypeDefinition["icon"] = sol::property(&TES3::RepairTool::getIconPath, &TES3::RepairTool::setIconPath);
			usertypeDefinition["mesh"] = sol::property(&TES3::RepairTool::getModelPath, &TES3::RepairTool::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::RepairTool::getName, &TES3::RepairTool::setName);

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["condition"] = &TES3::RepairTool::maxCondition;
			usertypeDefinition["model"] = sol::property(&TES3::RepairTool::getModelPath, &TES3::RepairTool::setModelPath);
			
			// Finish up our usertype.
			
		}
	}
}
