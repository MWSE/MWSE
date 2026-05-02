#include "TES3ApparatusLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"

#include "TES3Apparatus.h"
#include "TES3Script.h"

namespace mwse::lua {
	void bindTES3Apparatus() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Apparatus>("tes3apparatus");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Item(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["type"] = &TES3::Apparatus::type;
		usertypeDefinition["quality"] = &TES3::Apparatus::quality;
		usertypeDefinition["value"] = &TES3::Apparatus::value;
		usertypeDefinition["weight"] = &TES3::Apparatus::weight;
		usertypeDefinition["script"] = &TES3::Apparatus::script;
	}
}
