#include "TES3WeaponLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"
#include "LuaUtil.h"

#include "TES3Enchantment.h"
#include "TES3Script.h"
#include "TES3Weapon.h"
#include "TES3WorldController.h"

namespace mwse::lua {
	void bindTES3Weapon() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Weapon>("tes3weapon");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Item(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["chopMax"] = &TES3::Weapon::chopMax;
		usertypeDefinition["chopMin"] = &TES3::Weapon::chopMin;
		usertypeDefinition["enchantCapacity"] = &TES3::Weapon::enchantCapacity;
		usertypeDefinition["enchantment"] = sol::property(&TES3::Weapon::getEnchantment, &TES3::Weapon::setEnchantment);
		usertypeDefinition["flags"] = &TES3::Weapon::materialFlags;
		usertypeDefinition["hasDurability"] = sol::readonly_property(&TES3::Weapon::hasDurability);
		usertypeDefinition["ignoresNormalWeaponResistance"] = sol::property(&TES3::Weapon::getIgnoresNormalWeaponResistance, &TES3::Weapon::setIgnoresNormalWeaponResistance);
		usertypeDefinition["isAmmo"] = sol::readonly_property(&TES3::Weapon::isAmmo);
		usertypeDefinition["isMelee"] = sol::readonly_property(&TES3::Weapon::isMelee);
		usertypeDefinition["isOneHanded"] = sol::readonly_property(&TES3::Weapon::isOneHanded);
		usertypeDefinition["isProjectile"] = sol::readonly_property(&TES3::Weapon::isProjectile);
		usertypeDefinition["isRanged"] = sol::readonly_property(&TES3::Weapon::isRanged);
		usertypeDefinition["isSilver"] = sol::property(&TES3::Weapon::getIsSilver, &TES3::Weapon::setIsSilver);
		usertypeDefinition["isTwoHanded"] = sol::readonly_property(&TES3::Weapon::isTwoHanded);
		usertypeDefinition["maxCondition"] = sol::property(&TES3::Weapon::getDurability, &TES3::Weapon::setDurability);
		usertypeDefinition["reach"] = &TES3::Weapon::reach;
		usertypeDefinition["script"] = &TES3::Weapon::script;
		usertypeDefinition["skill"] = sol::property(&TES3::Weapon::getSkill);
		usertypeDefinition["skillId"] = sol::property(&TES3::Weapon::getSkillId);
		usertypeDefinition["slashMax"] = &TES3::Weapon::slashMax;
		usertypeDefinition["slashMin"] = &TES3::Weapon::slashMin;
		usertypeDefinition["speed"] = &TES3::Weapon::speed;
		usertypeDefinition["thrustMax"] = &TES3::Weapon::thrustMax;
		usertypeDefinition["thrustMin"] = &TES3::Weapon::thrustMin;
		usertypeDefinition["type"] = sol::readonly_property(&TES3::Weapon::getType);
		usertypeDefinition["typeName"] = sol::readonly_property(&TES3::Weapon::getTypeName);
		usertypeDefinition["value"] = &TES3::Weapon::value;
		usertypeDefinition["weight"] = &TES3::Weapon::weight;

		// Basic function binding.
		usertypeDefinition["createCopy"] = &TES3::Weapon::createCopy_lua<TES3::Weapon>;
		usertypeDefinition["getMaterialFlag"] = &TES3::Weapon::getMaterialFlag;
		usertypeDefinition["setMaterialFlag"] = &TES3::Weapon::setMaterialFlag;

		// TODO: Deprecated. Remove before 2.1-stable.
		usertypeDefinition["health"] = sol::readonly_property(&TES3::Weapon::getDurability);
	}
}
