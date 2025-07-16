#include "TES3NPCLua.h"

#include "LuaManager.h"
#include "TES3ActorLua.h"

#include "TES3NPC.h"
#include "TES3BodyPart.h"
#include "TES3Race.h"
#include "TES3Class.h"
#include "TES3Faction.h"
#include "TES3Script.h"

#include "BitUtil.h"

namespace mwse::lua {
	void bindTES3NPC() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::NPC
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::NPC>("tes3npc");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Actor, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3Actor(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["baseDisposition"] = &TES3::NPC::baseDisposition;
			usertypeDefinition["factionRank"] = &TES3::NPC::factionRank;
			usertypeDefinition["fatigue"] = &TES3::NPC::fatigue;
			usertypeDefinition["hair"] = &TES3::NPC::hair;
			usertypeDefinition["head"] = &TES3::NPC::head;
			usertypeDefinition["health"] = &TES3::NPC::health;
			usertypeDefinition["level"] = &TES3::NPC::level;
			usertypeDefinition["magicka"] = &TES3::NPC::magicka;
			usertypeDefinition["reputation"] = &TES3::NPC::initialReputation;
			usertypeDefinition["class"] = &TES3::NPC::class_;
			usertypeDefinition["faction"] = &TES3::NPC::faction;
			usertypeDefinition["race"] = sol::property(&TES3::NPC::getRace, &TES3::NPC::setRace);
			usertypeDefinition["script"] = &TES3::NPC::script;
			usertypeDefinition["soul"] = sol::readonly_property(&TES3::NPC::getSoulValue);

			// Indirect bindings to unions and arrays.
			usertypeDefinition["attributes"] = sol::property(&TES3::NPC::getAttributes);
			usertypeDefinition["skills"] = sol::property(&TES3::NPC::getSkills);

			// Constants.
			usertypeDefinition["isInstance"] = sol::var(false);

			// Functions exposed as properties.
			usertypeDefinition["aiConfig"] = sol::readonly_property(&TES3::NPC::getAIConfig);
			usertypeDefinition["height"] = sol::readonly_property(&TES3::NPC::getHeight);
			usertypeDefinition["isEssential"] = sol::readonly_property(&TES3::NPC::isEssential);
			usertypeDefinition["isGuard"] = sol::readonly_property(&TES3::NPC::isGuard);
			usertypeDefinition["isRespawn"] = sol::readonly_property(&TES3::NPC::isRespawn);
			usertypeDefinition["mesh"] = sol::property(&TES3::NPC::getModelPath, &TES3::NPC::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::NPC::getName, &TES3::NPC::setName);
			usertypeDefinition["spells"] = sol::readonly_property(&TES3::NPC::spellList);
			usertypeDefinition["weight"] = sol::readonly_property(&TES3::NPC::getWeight);

			// Easy access to actor flags.
			usertypeDefinition["female"] = sol::property(&TES3::NPC::getIsFemale, &TES3::NPC::setIsFemale);
			usertypeDefinition["autoCalc"] = sol::property(&TES3::NPC::getAutoCalc, &TES3::NPC::setAutoCalc);

			// Utility function bindings.
			//usertypeDefinition["createCopy"] = &TES3::NPC::createCopy_lua<TES3::NPC>; // NPC serialization does not support saving new NPCs at the moment.

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["model"] = sol::property(&TES3::NPC::getModelPath, &TES3::NPC::setModelPath);
			usertypeDefinition["essential"] = sol::property(&TES3::NPC::getIsEssential_legacy, &TES3::NPC::setIsEssential_legacy);
			usertypeDefinition["factionIndex"] = &TES3::NPC::initialReputation;
			usertypeDefinition["respawns"] = sol::property(&TES3::NPC::getRespawns_legacy, &TES3::NPC::setRespawns_legacy);
		}

		// Binding for TES3::NPCInstance
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::NPCInstance>("tes3npcInstance");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Actor, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3Actor(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["baseObject"] = sol::readonly_property(&TES3::NPCInstance::baseNPC);
			usertypeDefinition["disposition"] = sol::property(&TES3::NPCInstance::getDisposition_lua);
			usertypeDefinition["baseDisposition"] = sol::property(&TES3::NPCInstance::getBaseDisposition, &TES3::NPCInstance::setBaseDisposition);
			usertypeDefinition["reputation"] = &TES3::NPCInstance::reputation;

			// Basic function binding.
			usertypeDefinition["reevaluateEquipment"] = &TES3::NPCInstance::reevaluateEquipment;

			// Indirect bindings to unions and arrays.
			usertypeDefinition["attributes"] = sol::property(&TES3::NPCInstance::getAttributes);
			usertypeDefinition["skills"] = sol::property(&TES3::NPCInstance::getSkills);

			// Constants.
			usertypeDefinition["isInstance"] = sol::var(true);

			// Convenient base object access.
			usertypeDefinition["class"] = sol::readonly_property(&TES3::NPCInstance::getBaseClass);
			usertypeDefinition["factionRank"] = sol::readonly_property(&TES3::NPCInstance::getBaseFactionRank);
			usertypeDefinition["faction"] = sol::readonly_property(&TES3::NPCInstance::getBaseFaction);
			usertypeDefinition["race"] = sol::readonly_property(&TES3::NPCInstance::getBaseRace);
			usertypeDefinition["script"] = sol::readonly_property(&TES3::NPCInstance::getBaseScript);
			usertypeDefinition["soul"] = sol::readonly_property(&TES3::NPCInstance::getBaseSoulValue);

			// Functions exposed as properties.
			usertypeDefinition["aiConfig"] = sol::readonly_property(&TES3::NPCInstance::getAIConfig);
			usertypeDefinition["barterGold"] = sol::property(&TES3::NPCInstance::getBaseBarterGold, &TES3::NPCInstance::setBaseBarterGold);
			usertypeDefinition["fatigue"] = sol::readonly_property(&TES3::NPCInstance::getFatigue);
			usertypeDefinition["health"] = sol::readonly_property(&TES3::NPCInstance::getDurability);
			usertypeDefinition["height"] = sol::readonly_property(&TES3::NPCInstance::getHeight);
			usertypeDefinition["isEssential"] = sol::readonly_property(&TES3::NPCInstance::isEssential);
			usertypeDefinition["isGuard"] = sol::readonly_property(&TES3::NPCInstance::isGuard);
			usertypeDefinition["isRespawn"] = sol::readonly_property(&TES3::NPCInstance::isRespawn);
			usertypeDefinition["level"] = sol::readonly_property(&TES3::NPCInstance::getLevel);
			usertypeDefinition["magicka"] = sol::readonly_property(&TES3::NPCInstance::getMagicka);
			usertypeDefinition["mesh"] = sol::property(&TES3::NPCInstance::getModelPath, &TES3::NPCInstance::setModelPath);
			usertypeDefinition["name"] = sol::property(&TES3::NPCInstance::getName, &TES3::NPCInstance::setName);
			usertypeDefinition["spells"] = sol::readonly_property(&TES3::NPCInstance::getBaseSpellList);
			usertypeDefinition["weight"] = sol::readonly_property(&TES3::NPCInstance::getWeight);

			// Easy access to actor flags.
			usertypeDefinition["female"] = sol::property(&TES3::NPCInstance::getIsFemale, &TES3::NPCInstance::setIsFemale);
			usertypeDefinition["autoCalc"] = sol::property(&TES3::NPCInstance::getAutoCalc, &TES3::NPCInstance::setAutoCalc);

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["model"] = sol::property(&TES3::NPCInstance::getModelPath, &TES3::NPCInstance::setModelPath);
			usertypeDefinition["essential"] = sol::property(&TES3::NPCInstance::getIsEssential_legacy, &TES3::NPCInstance::setIsEssential_legacy);
			usertypeDefinition["factionIndex"] = &TES3::NPCInstance::reputation;
			usertypeDefinition["respawns"] = sol::property(&TES3::NPCInstance::getRespawns_legacy, &TES3::NPCInstance::setRespawns_legacy);
		}
	}
}
