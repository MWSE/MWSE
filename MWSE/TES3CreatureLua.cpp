#include "TES3CreatureLua.h"

#include "LuaManager.h"
#include "TES3ActorLua.h"

#include "TES3AIConfig.h"
#include "TES3Creature.h"
#include "TES3Item.h"
#include "TES3Script.h"
#include "TES3SpellList.h"

#include "BitUtil.h"

namespace mwse::lua {
	void bindTES3Creature() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::Creature
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Creature>("tes3creature");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Actor, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3Actor(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["aiConfig"] = sol::readonly_property(&TES3::Creature::aiConfig);
			usertypeDefinition["isInstance"] = sol::var(false);
			usertypeDefinition["soul"] = sol::property(&TES3::Creature::getSoulValue, &TES3::Creature::setSoulValue);
			usertypeDefinition["soundCreature"] = &TES3::Creature::soundGenerator;
			usertypeDefinition["spells"] = sol::readonly_property(&TES3::Creature::spellList);
			usertypeDefinition["type"] = &TES3::Creature::creatureType;

			// Indirect bindings to unions and arrays.
			usertypeDefinition["attacks"] = sol::readonly_property(&TES3::Creature::getAttacks);
			usertypeDefinition["attributes"] = sol::readonly_property(&TES3::Creature::getAttributes);
			usertypeDefinition["skills"] = sol::readonly_property(&TES3::Creature::getSkills);

			// Functions exposed as properties.
			usertypeDefinition["fatigue"] = sol::readonly_property(&TES3::Creature::getFatigue);
			usertypeDefinition["health"] = sol::readonly_property(&TES3::Creature::getDurability);
			usertypeDefinition["isEssential"] = sol::readonly_property(&TES3::Creature::isEssential);
			usertypeDefinition["isRespawn"] = sol::readonly_property(&TES3::Creature::isRespawn);
			usertypeDefinition["level"] = sol::readonly_property(&TES3::Creature::getLevel);
			usertypeDefinition["magicka"] = sol::readonly_property(&TES3::Creature::getMagicka);
			usertypeDefinition["mesh"] = sol::property(&TES3::Creature::getModelPath, &TES3::Creature::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::Creature::getName, &TES3::Creature::setName);
			usertypeDefinition["script"] = &TES3::Creature::script;

			// Easy access to actor flags.
			usertypeDefinition["biped"] = sol::property(&TES3::Creature::getIsBiped, &TES3::Creature::setIsBiped);
			usertypeDefinition["respawns"] = sol::property(&TES3::Creature::getRespawns, &TES3::Creature::setRespawns);
			usertypeDefinition["usesEquipment"] = sol::property(&TES3::Creature::getUsesEquipment, &TES3::Creature::setUsesEquipment);
			usertypeDefinition["swims"] = sol::property(&TES3::Creature::getSwims, &TES3::Creature::setSwims);
			usertypeDefinition["flies"] = sol::property(&TES3::Creature::getFlies, &TES3::Creature::setFlies);
			usertypeDefinition["walks"] = sol::property(&TES3::Creature::getWalks, &TES3::Creature::setWalks);

			// Utility function bindings.
			usertypeDefinition["createCopy"] = &TES3::Creature::createCopy_lua<TES3::Creature>;

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["model"] = sol::property(&TES3::Creature::getModelPath, &TES3::Creature::setModelPath);
		}

		// Binding for TES3::CreatureInstance
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::CreatureInstance>("tes3creatureInstance");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Actor, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3Actor(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["equipment"] = sol::readonly_property(&TES3::CreatureInstance::equipment);
			usertypeDefinition["fatigue"] = sol::readonly_property(&TES3::CreatureInstance::getFatigue);
			usertypeDefinition["health"] = sol::readonly_property(&TES3::CreatureInstance::getDurability);
			usertypeDefinition["inventory"] = sol::readonly_property(&TES3::CreatureInstance::inventory);
			usertypeDefinition["isInstance"] = sol::var(true);
			usertypeDefinition["weapon"] = sol::readonly_property(&TES3::CreatureInstance::weapon);

			// Basic function binding.
			usertypeDefinition["reevaluateEquipment"] = &TES3::CreatureInstance::reevaluateEquipment;

			// Properties that directly point to the base creature.
			usertypeDefinition["aiConfig"] = sol::readonly_property(&TES3::CreatureInstance::getBaseAIConfig);
			usertypeDefinition["attacks"] = sol::readonly_property(&TES3::CreatureInstance::getBaseAttacks);
			usertypeDefinition["attributes"] = sol::readonly_property(&TES3::CreatureInstance::getBaseAttributes);
			usertypeDefinition["baseObject"] = sol::readonly_property(&TES3::CreatureInstance::baseCreature);
			usertypeDefinition["soul"] = sol::property(&TES3::CreatureInstance::getBaseSoulValue, &TES3::CreatureInstance::setBaseSoulValue);
			usertypeDefinition["soundCreatureInstance"] = sol::readonly_property(&TES3::CreatureInstance::getBaseSoundGenerator);
			usertypeDefinition["skills"] = sol::readonly_property(&TES3::CreatureInstance::getBaseSkills);
			usertypeDefinition["type"] = sol::readonly_property(&TES3::CreatureInstance::getBaseCreatureType);
			usertypeDefinition["spells"] = sol::readonly_property(&TES3::CreatureInstance::getBaseSpells);

			// Functions exposed as properties.
			usertypeDefinition["barterGold"] = sol::property(&TES3::CreatureInstance::getBaseBarterGold, &TES3::CreatureInstance::setBaseBarterGold);
			usertypeDefinition["isEssential"] = sol::readonly_property(&TES3::CreatureInstance::isEssential);
			usertypeDefinition["isRespawn"] = sol::readonly_property(&TES3::CreatureInstance::isRespawn);
			usertypeDefinition["level"] = sol::readonly_property(&TES3::CreatureInstance::getLevel);
			usertypeDefinition["magicka"] = sol::readonly_property(&TES3::CreatureInstance::getMagicka);
			usertypeDefinition["mesh"] = sol::property(&TES3::CreatureInstance::getModelPath, &TES3::CreatureInstance::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::CreatureInstance::getName, &TES3::CreatureInstance::setName);
			usertypeDefinition["script"] = sol::readonly_property(&TES3::CreatureInstance::getScript);

			// Easy access to actor flags.
			usertypeDefinition["biped"] = sol::property(&TES3::CreatureInstance::getIsBiped, &TES3::CreatureInstance::setIsBiped);
			usertypeDefinition["respawns"] = sol::property(&TES3::CreatureInstance::getRespawns, &TES3::CreatureInstance::setRespawns);
			usertypeDefinition["usesEquipment"] = sol::property(&TES3::CreatureInstance::getUsesEquipment, &TES3::CreatureInstance::setUsesEquipment);
			usertypeDefinition["swims"] = sol::property(&TES3::CreatureInstance::getSwims, &TES3::CreatureInstance::setSwims);
			usertypeDefinition["flies"] = sol::property(&TES3::CreatureInstance::getFlies, &TES3::CreatureInstance::setFlies);
			usertypeDefinition["walks"] = sol::property(&TES3::CreatureInstance::getWalks, &TES3::CreatureInstance::setWalks);

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["baseCreature"] = sol::readonly_property(&TES3::CreatureInstance::baseCreature);
			usertypeDefinition["model"] = sol::property(&TES3::CreatureInstance::getModelPath, &TES3::CreatureInstance::setModelPath);
		}
	}
}
