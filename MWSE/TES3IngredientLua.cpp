#include "TES3IngredientLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Ingredient.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		void bindTES3Ingredient() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Ingredient>("tes3ingredient");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["value"] = &TES3::Ingredient::value;
			usertypeDefinition["weight"] = &TES3::Ingredient::weight;

			// Indirect bindings to unions and arrays.
			usertypeDefinition["effects"] = sol::readonly_property(&TES3::Ingredient::getEffects);
			usertypeDefinition["effectAttributeIds"] = sol::readonly_property(&TES3::Ingredient::getEffectAttributeIds);
			usertypeDefinition["effectSkillIds"] = sol::readonly_property(&TES3::Ingredient::getEffectSkillIds);

			// Functions exposed as properties.
			usertypeDefinition["icon"] = sol::property(&TES3::Ingredient::getIconPath, &TES3::Ingredient::setIconPath);
			usertypeDefinition["mesh"] = sol::property(&TES3::Ingredient::getModelPath, &TES3::Ingredient::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::Ingredient::getName, &TES3::Ingredient::setName);
			usertypeDefinition["script"] = &TES3::Ingredient::script;

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["model"] = sol::property(&TES3::Ingredient::getModelPath, &TES3::Ingredient::setModelPath);
		}
	}
}
