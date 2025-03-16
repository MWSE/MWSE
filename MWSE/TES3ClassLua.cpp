#include "TES3ClassLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Class.h"

namespace mwse::lua {
	void bindTES3Class() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Class>("tes3class");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
		setUserdataForTES3BaseObject(usertypeDefinition);

		// Base property overrides.
		usertypeDefinition[sol::meta_function::to_string] = &TES3::Class::getObjectID;
		usertypeDefinition["id"] = sol::readonly_property(&TES3::Class::getObjectID);

		// Basic property binding.
		usertypeDefinition["services"] = &TES3::Class::services;
		usertypeDefinition["specialization"] = &TES3::Class::specialization;

		// Indirect bindings to unions and arrays.
		usertypeDefinition["attributes"] = sol::readonly_property(&TES3::Class::getAttributes);
		usertypeDefinition["skills"] = sol::readonly_property(&TES3::Class::getSkills);

		// Provide friendly access to service flags.
		usertypeDefinition["bartersAlchemy"] = sol::property(&TES3::Class::getBartersAlchemy, &TES3::Class::setBartersAlchemy);
		usertypeDefinition["bartersApparatus"] = sol::property(&TES3::Class::getBartersApparatus, &TES3::Class::setBartersApparatus);
		usertypeDefinition["bartersArmor"] = sol::property(&TES3::Class::getBartersArmor, &TES3::Class::setBartersArmor);
		usertypeDefinition["bartersBooks"] = sol::property(&TES3::Class::getBartersBooks, &TES3::Class::setBartersBooks);
		usertypeDefinition["bartersClothing"] = sol::property(&TES3::Class::getBartersClothing, &TES3::Class::setBartersClothing);
		usertypeDefinition["bartersEnchantedItems"] = sol::property(&TES3::Class::getBartersEnchantedItems, &TES3::Class::setBartersEnchantedItems);
		usertypeDefinition["bartersIngredients"] = sol::property(&TES3::Class::getBartersIngredients, &TES3::Class::setBartersIngredients);
		usertypeDefinition["bartersLights"] = sol::property(&TES3::Class::getBartersLights, &TES3::Class::setBartersLights);
		usertypeDefinition["bartersLockpicks"] = sol::property(&TES3::Class::getBartersLockpicks, &TES3::Class::setBartersLockpicks);
		usertypeDefinition["bartersMiscItems"] = sol::property(&TES3::Class::getBartersMiscItems, &TES3::Class::setBartersMiscItems);
		usertypeDefinition["bartersProbes"] = sol::property(&TES3::Class::getBartersProbes, &TES3::Class::setBartersProbes);
		usertypeDefinition["bartersRepairTools"] = sol::property(&TES3::Class::getBartersRepairTools, &TES3::Class::setBartersRepairTools);
		usertypeDefinition["bartersWeapons"] = sol::property(&TES3::Class::getBartersWeapons, &TES3::Class::setBartersWeapons);
		usertypeDefinition["offersBartering"] = sol::readonly_property(&TES3::Class::getOffersBartering);
		usertypeDefinition["offersEnchanting"] = sol::property(&TES3::Class::getOffersEnchanting, &TES3::Class::setOffersEnchanting);
		usertypeDefinition["offersRepairs"] = sol::property(&TES3::Class::getOffersRepairs, &TES3::Class::setOffersRepairs);
		usertypeDefinition["offersSpellmaking"] = sol::property(&TES3::Class::getOffersSpellmaking, &TES3::Class::setOffersSpellmaking);
		usertypeDefinition["offersSpells"] = sol::property(&TES3::Class::getOffersSpells, &TES3::Class::setOffersSpells);
		usertypeDefinition["offersTraining"] = sol::property(&TES3::Class::getOffersTraining, &TES3::Class::setOffersTraining);

		// Properties that have type remapping.
		usertypeDefinition["playable"] = sol::property(&TES3::Class::getIsPlayable, &TES3::Class::setIsPlayable);

		// Functions exposed as properties.
		usertypeDefinition["majorSkills"] = sol::readonly_property(&TES3::Class::getMajorSkills_lua);
		usertypeDefinition["minorSkills"] = sol::readonly_property(&TES3::Class::getMinorSkills_lua);
		usertypeDefinition["name"] = sol::property(&TES3::Class::getName, &TES3::Class::setName);
		usertypeDefinition["image"] = sol::property(&TES3::Class::getName, &TES3::Class::getLevelUpImage);

		// Description may need to be loaded from disk, handle it specially.
		usertypeDefinition["description"] = sol::property(&TES3::Class::getOrLoadDescription, &TES3::Class::setDescription_lua);
	}
}
