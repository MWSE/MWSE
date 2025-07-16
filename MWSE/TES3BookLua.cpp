#include "TES3BookLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"

#include "TES3Book.h"
#include "TES3Enchantment.h"
#include "TES3Script.h"

namespace mwse::lua {
	void bindTES3Book() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Book>("tes3book");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Item(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["enchantCapacity"] = &TES3::Book::enchantCapacity;
		usertypeDefinition["skill"] = &TES3::Book::skillToRaise;
		usertypeDefinition["type"] = &TES3::Book::bookType;
		usertypeDefinition["value"] = &TES3::Book::value;
		usertypeDefinition["weight"] = &TES3::Book::weight;

		// Functions exposed as properties.
		usertypeDefinition["enchantment"] = sol::property(&TES3::Book::getEnchantment, &TES3::Book::setEnchantment);
		usertypeDefinition["script"] = &TES3::Book::script;
		usertypeDefinition["text"] = sol::property(&TES3::Book::getBookText);
	}
}
