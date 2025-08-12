#include "TES3SpellLua.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3Util.h"

#include "TES3MobileActor.h"
#include "TES3Spell.h"
#include "TES3SpellList.h"

namespace mwse::lua {
	void bindTES3SpellList() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::SpellList>("tes3spellList");
		usertypeDefinition["new"] = sol::no_constructor;

		// 
		usertypeDefinition["iterator"] = sol::readonly_property(&TES3::SpellList::list);

		// Basic function binding.
		usertypeDefinition["containsType"] = &TES3::SpellList::containsType;
		usertypeDefinition["getCheapest"] = &TES3::SpellList::getCheapest;

		// Ambiguous function binding.
		usertypeDefinition["add"] = &TES3::SpellList::add_lua;
		usertypeDefinition["remove"] = &TES3::SpellList::remove_lua;
		usertypeDefinition["contains"] = &TES3::SpellList::contains_lua;
	}
}
