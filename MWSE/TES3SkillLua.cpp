#include "TES3SkillLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Skill.h"

namespace mwse::lua {
	void bindTES3Skill() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Skill>("tes3skill");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
		setUserdataForTES3BaseObject(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["id"] = sol::readonly_property(&TES3::Skill::skill);
		usertypeDefinition["attribute"] = &TES3::Skill::governingAttribute;
		usertypeDefinition["specialization"] = &TES3::Skill::specialization;

		// Functions as properties.
		usertypeDefinition["name"] = sol::readonly_property(&TES3::Skill::getName);
		usertypeDefinition["iconPath"] = sol::readonly_property(&TES3::Skill::getIconPath);
		// Description may need to be loaded from disk, handle it specially.
		usertypeDefinition["description"] = sol::readonly_property(&TES3::Skill::getAndLoadDescription);

		// Indirect bindings to unions and arrays.
		usertypeDefinition["actions"] = sol::readonly_property(&TES3::Skill::getProgressActions);
	}
}
