#include "TES3ArmorLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"
#include "LuaUtil.h"

#include "TES3Armor.h"
#include "TES3BodyPart.h"
#include "TES3Enchantment.h"
#include "TES3MobileActor.h"
#include "TES3Script.h"
#include "TES3WorldController.h"

namespace mwse::lua {
	void bindTES3Armor() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Armor>("tes3armor");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Item(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["armorRating"] = &TES3::Armor::armorRating;
		usertypeDefinition["armorScalar"] = sol::readonly_property(&TES3::Armor::getArmorScalar);
		usertypeDefinition["enchantCapacity"] = &TES3::Armor::enchantCapacity;
		usertypeDefinition["parts"] = sol::readonly_property(&TES3::Armor::getParts);
		usertypeDefinition["slot"] = &TES3::Armor::slot;
		usertypeDefinition["value"] = &TES3::Armor::value;
		usertypeDefinition["weight"] = &TES3::Armor::weight;

		// Functions exposed as properties.
		usertypeDefinition["enchantment"] = sol::property(&TES3::Armor::getEnchantment, &TES3::Armor::setEnchantment);
		usertypeDefinition["maxCondition"] = sol::property(&TES3::Armor::getDurability, &TES3::Armor::setDurability);
		usertypeDefinition["isClosedHelmet"] = sol::readonly_property(&TES3::Armor::isClosedHelmet);
		usertypeDefinition["isLeftPart"] = sol::property(&TES3::Armor::isLeftPartOfPair);
		usertypeDefinition["isUsableByBeasts"] = sol::readonly_property(&TES3::Armor::isUsableByBeasts);
		usertypeDefinition["script"] = &TES3::Armor::script;
		usertypeDefinition["slotName"] = sol::property(&TES3::Armor::getTypeName);
		usertypeDefinition["weightClass"] = sol::property(&TES3::Armor::getWeightClass);

		// Basic function binding.
		usertypeDefinition["calculateArmorRating"] = &TES3::Armor::calculateArmorRating_lua;
		usertypeDefinition["createCopy"] = &TES3::Armor::createCopy_lua<TES3::Armor>;
		usertypeDefinition["setupBodyParts"] = &TES3::Armor::setupBodyParts;

		// TODO: Deprecated. Remove before 2.1-stable.
		usertypeDefinition["health"] = sol::property(&TES3::Armor::getDurability, &TES3::Armor::setDurability);
	}
}
