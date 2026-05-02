#include "TES3GameSettingLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3GameSetting.h"

namespace mwse::lua {
	void bindTES3GameSetting() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::GameSetting>("tes3gameSetting");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
		setUserdataForTES3BaseObject(usertypeDefinition);

		// Base class overrides.
		usertypeDefinition[sol::meta_function::to_string] = &TES3::GameSetting::getObjectID;
		usertypeDefinition["id"] = sol::readonly_property(&TES3::GameSetting::getObjectID);

		// Allow object to be serialized to json.
		usertypeDefinition["__tojson"] = &TES3::GameSetting::toJson;

		// Basic property binding.
		usertypeDefinition["index"] = sol::readonly_property(&TES3::GameSetting::index);

		// Functions exposed as properties.
		usertypeDefinition["defaultValue"] = sol::readonly_property(&TES3::GameSetting::getDefaultValue_lua);
		usertypeDefinition["rawValue"] = &TES3::GameSetting::value;
		usertypeDefinition["type"] = sol::readonly_property(&TES3::GameSetting::getType);
		usertypeDefinition["value"] = sol::property(&TES3::GameSetting::getValue_lua, &TES3::GameSetting::setValue_lua);
	}
}
