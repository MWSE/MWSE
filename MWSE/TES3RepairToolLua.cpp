#include "TES3RepairToolLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"

#include "TES3RepairTool.h"
#include "TES3Script.h"

namespace mwse::lua {
	void bindTES3RepairTool() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::RepairTool>("tes3repairTool");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Item(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["maxCondition"] = &TES3::RepairTool::maxCondition;
		usertypeDefinition["quality"] = &TES3::RepairTool::quality;
		usertypeDefinition["script"] = &TES3::RepairTool::script;
		usertypeDefinition["value"] = &TES3::RepairTool::value;
		usertypeDefinition["weight"] = &TES3::RepairTool::weight;

		// TODO: Deprecated. Remove before 2.1-stable.
		usertypeDefinition["condition"] = &TES3::RepairTool::maxCondition;
	}
}
