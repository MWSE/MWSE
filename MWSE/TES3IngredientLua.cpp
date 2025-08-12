#include "TES3IngredientLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"

#include "TES3Ingredient.h"
#include "TES3Script.h"

namespace mwse::lua {
	void bindTES3Ingredient() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Ingredient>("tes3ingredient");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Item(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["value"] = &TES3::Ingredient::value;
		usertypeDefinition["weight"] = &TES3::Ingredient::weight;

		// Basic function binding.
		usertypeDefinition["getFirstIndexOfEffect"] = &TES3::Ingredient::getFirstIndexOfEffect;
		usertypeDefinition["hasEffect"] = &TES3::Ingredient::hasEffect;

		// Indirect bindings to unions and arrays.
		usertypeDefinition["effects"] = sol::readonly_property(&TES3::Ingredient::getEffects);
		usertypeDefinition["effectAttributeIds"] = sol::readonly_property(&TES3::Ingredient::getEffectAttributeIds);
		usertypeDefinition["effectSkillIds"] = sol::readonly_property(&TES3::Ingredient::getEffectSkillIds);

		// Functions exposed as properties.
		usertypeDefinition["script"] = &TES3::Ingredient::script;
	}
}
