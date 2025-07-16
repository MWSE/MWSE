#include "TES3ClothingLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"

#include "TES3BodyPart.h"
#include "TES3Clothing.h"
#include "TES3Enchantment.h"
#include "TES3Script.h"
#include "TES3WorldController.h"

namespace mwse::lua {
	void bindTES3Clothing() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Clothing>("tes3clothing");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Item(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["enchantCapacity"] = &TES3::Clothing::enchantCapacity;
		usertypeDefinition["parts"] = sol::readonly_property(&TES3::Clothing::getParts);
		usertypeDefinition["slot"] = &TES3::Clothing::slot;
		usertypeDefinition["value"] = &TES3::Clothing::value;
		usertypeDefinition["weight"] = &TES3::Clothing::weight;

		// Basic function binding.
		usertypeDefinition["createCopy"] = &TES3::Clothing::createCopy_lua<TES3::Clothing>;
		usertypeDefinition["setupBodyParts"] = &TES3::Clothing::setupBodyParts;

		// Functions exposed as properties.
		usertypeDefinition["enchantment"] = sol::property(&TES3::Clothing::getEnchantment, &TES3::Clothing::setEnchantment);
		usertypeDefinition["isLeftPart"] = sol::property(&TES3::Clothing::isLeftPartOfPair);
		usertypeDefinition["isUsableByBeasts"] = sol::readonly_property(&TES3::Clothing::isUsableByBeasts);
		usertypeDefinition["script"] = &TES3::Clothing::script;
		usertypeDefinition["slotName"] = sol::property(&TES3::Clothing::getTypeName);
	}
}
