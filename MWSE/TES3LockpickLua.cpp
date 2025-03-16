#include "TES3LockpickLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"

#include "TES3Lockpick.h"
#include "TES3Script.h"

namespace mwse::lua {
	void bindTES3Lockpick() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Lockpick>("tes3lockpick");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Item(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["maxCondition"] = &TES3::Lockpick::maxCondition;
		usertypeDefinition["quality"] = &TES3::Lockpick::quality;
		usertypeDefinition["script"] = &TES3::Lockpick::script;
		usertypeDefinition["value"] = &TES3::Lockpick::value;
		usertypeDefinition["weight"] = &TES3::Lockpick::weight;

		// TODO: Deprecated. Remove before 2.1-stable.
		usertypeDefinition["condition"] = &TES3::Lockpick::maxCondition;
	}
}
