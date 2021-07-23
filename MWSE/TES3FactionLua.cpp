#include "TES3FactionLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Faction.h"

namespace mwse {
	namespace lua {
		void bindTES3Faction() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for TES3::Faction::Rank
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Faction::Rank>("tes3factionRank");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["reputation"] = &TES3::Faction::Rank::reputation;

				// Indirect bindings to unions and arrays.
				usertypeDefinition["attributes"] = sol::readonly_property(&TES3::Faction::Rank::getRequiredAttributeValues);
				usertypeDefinition["skills"] = sol::readonly_property(&TES3::Faction::Rank::getRequiredSkillValues);
			}

			// Binding for TES3::Faction::ReactionNode
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Faction::ReactionNode>("tes3factionReaction");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["reputation"] = &TES3::Faction::ReactionNode::reaction;
				usertypeDefinition["faction"] = sol::readonly_property(&TES3::Faction::ReactionNode::faction);
			}

			// Binding for TES3::Faction
			//! TODO: Provide a more friendly way to access rank names. Probably needs to be hidden behind its own struct.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Faction>("tes3faction");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
				setUserdataForTES3BaseObject(usertypeDefinition);

				// Base class overrides.
				usertypeDefinition[sol::meta_function::to_string] = &TES3::Faction::getObjectID;
				usertypeDefinition["id"] = sol::readonly_property(&TES3::Faction::getObjectID);

				// Basic property binding.
				usertypeDefinition["reactions"] = sol::readonly_property(&TES3::Faction::reactions);
				usertypeDefinition["playerReputation"] = &TES3::Faction::playerReputation;
				usertypeDefinition["playerJoined"] = sol::property(&TES3::Faction::getPlayerJoined, &TES3::Faction::setPlayerJoined);
				usertypeDefinition["playerExpelled"] = sol::property(&TES3::Faction::getPlayerExpelled, &TES3::Faction::setPlayerExpelled);

				// Indirect bindings to unions and arrays.
				usertypeDefinition["attributes"] = sol::property(&TES3::Faction::getAttributes);
				usertypeDefinition["ranks"] = sol::readonly_property(&TES3::Faction::getRanks);
				usertypeDefinition["skills"] = sol::property(&TES3::Faction::getSkills);

				// Functions exposed as properties.
				usertypeDefinition["name"] = sol::property(&TES3::Faction::getName, &TES3::Faction::setName);
				usertypeDefinition["playerRank"] = sol::property(&TES3::Faction::getEffectivePlayerRank, &TES3::Faction::setEffectivePlayerRank);

				// Basic function binding.
				usertypeDefinition["getRankName"] = &TES3::Faction::getRankName;
				usertypeDefinition["setRankName"] = &TES3::Faction::setRankName;
			}
		}
	}
}
