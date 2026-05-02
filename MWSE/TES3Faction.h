#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

#include "TES3IteratedList.h"

namespace TES3 {
	namespace FactionMembershipFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			PlayerJoined = 0x1,
			PlayerExpelled = 0x2
		};
	}
	struct Faction : BaseObject {
		struct Rank {
			int reqAttributes[2];
			int reqSkills[2];
			int reputation;

			//
			// Custom functions.
			//

			std::reference_wrapper<int[2]> getRequiredAttributeValues();
			std::reference_wrapper<int[2]> getRequiredSkillValues();

		};
		struct ReactionNode {
			Faction * faction;
			int reaction;
		};
		char objectID[32]; // 0x10
		char name[32]; // 0x30
		char rankNames[10][32]; // 0x50
		int attributes[2]; // 0x190
		Rank ranks[10]; // 0x198
		int skills[7]; // 0x260
		int unknown_0x27C;
		IteratedList<ReactionNode*> reactions; // 0x280
		int playerRank; // 0x294
		int playerReputation; // 0x298
		unsigned int playerMembershipFlags; // 0x29C

		static constexpr auto OBJECT_TYPE = ObjectType::Faction;

		Faction() = delete;
		~Faction() = delete;

		//
		// Virtual table overrides.
		//

		char* getObjectID();

		//
		// Custom functions.
		//

		char * getName();
		void setName(const char*);

		const char* getRankName(int rank) const;
		void setRankName(int rank, const char* name);

		bool getMembershipFlag(unsigned int) const;
		void setMembershipFlag(unsigned int, bool);

		int getEffectivePlayerRank() const;
		void setEffectivePlayerRank(int rank);

		bool getPlayerJoined() const;
		void setPlayerJoined(bool value);

		bool getPlayerExpelled() const;
		void setPlayerExpelled(bool value);

		bool getReactionWithFaction(const Faction* faction, int& out_reaction) const;
		sol::optional<int> getReactionWithFaction_lua(const Faction* faction) const;

		int getLowestJoinedReaction(Faction** out_faction = nullptr) const;
		sol::optional<std::tuple<int, Faction*>> getLowestJoinedReaction_lua() const;

		int getHighestJoinedReaction(Faction** out_faction = nullptr) const;
		sol::optional<std::tuple<int, Faction*>> getHighestJoinedReaction_lua() const;

		enum class AdvancementPotential : unsigned int {
			NeedSkillsAndReputation,
			NeedReputation,
			NeedSkills,
			CanAdvance,
		};

		AdvancementPotential getAdvancementPotential() const;

		std::reference_wrapper<int[2]> getAttributes();
		std::reference_wrapper<int[7]> getSkills();
		std::reference_wrapper<Rank[10]> getRanks();

		bool playerJoin_lua();
		bool playerLeave_lua();
		bool playerExpel_lua();
		bool playerClearExpel_lua();
		bool playerPromote_lua();
		bool playerDemote_lua();
	};
	static_assert(sizeof(Faction::Rank) == 0x14, "TES3::Faction::Rank failed size validation");
	static_assert(sizeof(Faction::ReactionNode) == 0x8, "TES3::Faction::ReactionNode failed size validation");
	static_assert(sizeof(Faction) == 0x2A0, "TES3::Faction failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Faction)
