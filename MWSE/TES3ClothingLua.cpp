#include "TES3ClothingLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3BodyPart.h"
#include "TES3Clothing.h"
#include "TES3Enchantment.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		void bindTES3Clothing() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Clothing>("tes3clothing");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["enchantCapacity"] = &TES3::Clothing::enchantCapacity;
			usertypeDefinition["parts"] = sol::readonly_property(&TES3::Clothing::getParts);
			usertypeDefinition["slot"] = &TES3::Clothing::slot;
			usertypeDefinition["value"] = &TES3::Clothing::value;
			usertypeDefinition["weight"] = &TES3::Clothing::weight;

			// Functions exposed as properties.
			usertypeDefinition["enchantment"] = sol::property(&TES3::Clothing::getEnchantment, &TES3::Clothing::setEnchantment);
			usertypeDefinition["icon"] = sol::property(&TES3::Clothing::getIconPath, &TES3::Clothing::setIconPath);
			usertypeDefinition["isLeftPart"] = sol::property(&TES3::Clothing::isLeftPartOfPair);
			usertypeDefinition["mesh"] = sol::property(&TES3::Clothing::getModelPath, &TES3::Clothing::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::Clothing::getName, &TES3::Clothing::setName);
			usertypeDefinition["script"] = &TES3::Clothing::script;
			usertypeDefinition["slotName"] = sol::property(&TES3::Clothing::getTypeName);

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["model"] = sol::property(&TES3::Clothing::getModelPath, &TES3::Clothing::setModelPath);
		}
	}
}
