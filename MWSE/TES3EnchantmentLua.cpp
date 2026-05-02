#include "TES3EnchantmentLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Enchantment.h"
#include "LuaObject.h"

namespace mwse::lua {
	auto createEnchantment(sol::table params)
	{
		return makeObjectCreator(TES3::ObjectType::Enchantment)->create(params, false);
	}

	void bindTES3Enchantment() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Enchantment>("tes3enchantment");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Object(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["castType"] = &TES3::Enchantment::castType;
		usertypeDefinition["chargeCost"] = &TES3::Enchantment::chargeCost;
		usertypeDefinition["flags"] = &TES3::Enchantment::flags;
		usertypeDefinition["maxCharge"] = &TES3::Enchantment::maxCharge;

		// Convenient flag access.
		usertypeDefinition["autoCalc"] = sol::property(&TES3::Enchantment::getAutoCalc, &TES3::Enchantment::setAutoCalc);

		// Basic function binding.
		usertypeDefinition["getActiveEffectCount"] = &TES3::Enchantment::getActiveEffectCount;
		usertypeDefinition["getFirstIndexOfEffect"] = &TES3::Enchantment::getFirstIndexOfEffect;
		usertypeDefinition["hasEffect"] = &TES3::Enchantment::hasEffect;

		// Indirect bindings to unions and arrays.
		usertypeDefinition["effects"] = sol::readonly_property(&TES3::Enchantment::getEffects);

		// utility function bindings
		usertypeDefinition["create"] = &createEnchantment;
		usertypeDefinition["createCopy"] = &TES3::Enchantment::createCopy_lua<TES3::Enchantment>;
	}
}
