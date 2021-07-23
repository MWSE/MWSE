#include "TES3BookLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Book.h"
#include "TES3Enchantment.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		void bindTES3Book() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Book>("tes3book");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["enchantCapacity"] = &TES3::Book::enchantCapacity;
			usertypeDefinition["skill"] = &TES3::Book::skillToRaise;
			usertypeDefinition["type"] = &TES3::Book::bookType;
			usertypeDefinition["value"] = &TES3::Book::value;
			usertypeDefinition["weight"] = &TES3::Book::weight;

			// Functions exposed as properties.
			usertypeDefinition["enchantment"] = sol::property(&TES3::Book::getEnchantment, &TES3::Book::setEnchantment);
			usertypeDefinition["icon"] = sol::property(&TES3::Book::getIconPath, &TES3::Book::setIconPath);
			usertypeDefinition["mesh"] = sol::property(&TES3::Book::getModelPath, &TES3::Book::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::Book::getName, &TES3::Book::setName);
			usertypeDefinition["script"] = &TES3::Book::script;
			usertypeDefinition["text"] = sol::property(&TES3::Book::getBookText);

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["model"] = sol::property(&TES3::Book::getModelPath, &TES3::Book::setModelPath);
		}
	}
}
