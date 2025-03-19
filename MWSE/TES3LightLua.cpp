#include "TES3LightLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"

#include "TES3Inventory.h"
#include "TES3ItemData.h"
#include "TES3Light.h"
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3Sound.h"

namespace mwse::lua {
	void bindTES3Light() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Light>("tes3light");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Item(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["value"] = &TES3::Light::value;
		usertypeDefinition["weight"] = &TES3::Light::weight;
		usertypeDefinition["time"] = &TES3::Light::time;
		usertypeDefinition["radius"] = &TES3::Light::radius;
		usertypeDefinition["flags"] = &TES3::Light::flags;
		usertypeDefinition["sound"] = sol::readonly_property(&TES3::Light::sound);
		usertypeDefinition["script"] = &TES3::Light::script;

		// User-friendly access to flags.
		usertypeDefinition["canCarry"] = sol::property(&TES3::Light::getCanCarry, &TES3::Light::setCanCarry);
		usertypeDefinition["flickers"] = sol::property(&TES3::Light::getFlickers, &TES3::Light::setFlickers);
		usertypeDefinition["flickersSlowly"] = sol::property(&TES3::Light::getFlickersSlowly, &TES3::Light::setFlickersSlowly);
		usertypeDefinition["isDynamic"] = sol::property(&TES3::Light::getIsDynamic, &TES3::Light::setIsDynamic);
		usertypeDefinition["isFire"] = sol::property(&TES3::Light::getIsFire, &TES3::Light::setIsFire);
		usertypeDefinition["isNegative"] = sol::property(&TES3::Light::getIsNegative, &TES3::Light::setIsNegative);
		usertypeDefinition["isOffByDefault"] = sol::property(&TES3::Light::getIsOffByDefault, &TES3::Light::setIsOffByDefault);
		usertypeDefinition["pulses"] = sol::property(&TES3::Light::getPulses, &TES3::Light::setPulses);
		usertypeDefinition["pulsesSlowly"] = sol::property(&TES3::Light::getPulsesSlowly, &TES3::Light::setPulsesSlowly);

		// Indirect bindings to unions and arrays.
		usertypeDefinition["color"] = sol::readonly_property(&TES3::Light::getColor);

		// Methods.
		usertypeDefinition["createCopy"] = &TES3::Light::createCopy_lua<TES3::Light>;
		usertypeDefinition["getTimeLeft"] = &TES3::Light::getTimeLeft_lua;
	}
}
