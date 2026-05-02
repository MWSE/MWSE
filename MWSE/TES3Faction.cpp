#include "TES3Faction.h"

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3UIManager.h"

namespace TES3 {
	std::reference_wrapper<int[2]> Faction::Rank::getRequiredAttributeValues() {
		return std::ref(reqAttributes);
	}

	std::reference_wrapper<int[2]> Faction::Rank::getRequiredSkillValues() {
		return std::ref(reqSkills);
	}

	int Faction::getEffectivePlayerRank() const {
		if (getMembershipFlag(FactionMembershipFlag::PlayerJoined)) {
			return playerRank;
		}

		return -1;
	}

	char* Faction::getObjectID() {
		return objectID;
	}

	char* Faction::getName() {
		return name;
	}

	void Faction::setName(const char* value) {
		if (value != nullptr && strlen(value) < 32) {
			strcpy(name, value);
		}
	}

	const char* Faction::getRankName(int rank) const {
		// Clamp rank instead of throwing an exception, as there are mods that do rank arithmetic without checking.
		// The rank parameter accepts negative numbers so they can be clamped instead of wrapping.
		rank = std::clamp(rank, 0, 9);
		return rankNames[rank];
	}

	void Faction::setRankName(int rank, const char* name) {
		if (rank < 0 || rank > 9) {
			throw std::invalid_argument("Rank must be between inclusive values 0 and 9.");
		}
		else if (strnlen_s(name, 32) > 31) {
			throw std::invalid_argument("Name must not be more than 31 characters.");
		}
		strncpy_s(rankNames[rank], name, sizeof(rankNames[rank]));
	}

	bool Faction::getMembershipFlag(unsigned int flag) const {
		return (playerMembershipFlags & flag) != 0;
	}

	void Faction::setMembershipFlag(unsigned int flag, bool set) {
		if (set) {
			playerMembershipFlags |= flag;
		}
		else {
			playerMembershipFlags &= ~flag;
		}
	}

	void Faction::setEffectivePlayerRank(int rank) {
		if (rank < 0) {
			setMembershipFlag(FactionMembershipFlag::PlayerJoined, false);
			playerRank = -1;
		}
		else {
			setMembershipFlag(FactionMembershipFlag::PlayerJoined, true);
			playerRank = rank;
		}
	}

	bool Faction::getPlayerJoined() const {
		return getMembershipFlag(TES3::FactionMembershipFlag::PlayerJoined);
	}

	void Faction::setPlayerJoined(bool value) {
		setMembershipFlag(TES3::FactionMembershipFlag::PlayerJoined, value);
	}

	bool Faction::getPlayerExpelled() const {
		return getMembershipFlag(TES3::FactionMembershipFlag::PlayerExpelled);
	}

	void Faction::setPlayerExpelled(bool value) {
		setMembershipFlag(TES3::FactionMembershipFlag::PlayerExpelled, value);
	}

	bool Faction::getReactionWithFaction(const Faction* faction, int& out_reaction) const {
		for (const auto& itt : reactions) {
			if (itt->faction == faction) {
				out_reaction = itt->reaction;
				return true;
			}
		}
		return false;
	}

	sol::optional<int> Faction::getReactionWithFaction_lua(const Faction* faction) const {
		int reaction = 0;
		if (getReactionWithFaction(faction, reaction)) {
			return reaction;
		}
		return {};
	}

	const auto TES3_Faction_getLowestJoinedReaction = reinterpret_cast<int(__thiscall*)(const Faction*, Faction**)>(0x4AD5E0);
	int Faction::getLowestJoinedReaction(Faction** out_faction) const {
		return TES3_Faction_getLowestJoinedReaction(this, out_faction);
	}

	sol::optional<std::tuple<int, Faction*>> Faction::getLowestJoinedReaction_lua() const {
		Faction* faction = nullptr;
		auto reaction = getLowestJoinedReaction(&faction);
		if (faction) {
			return { { reaction, faction } };
		}
		return {};
	}

	const auto TES3_Faction_getHighestJoinedReaction = reinterpret_cast<int(__thiscall*)(const Faction*, Faction**)>(0x4AD650);
	int Faction::getHighestJoinedReaction(Faction** out_faction) const {
		return TES3_Faction_getHighestJoinedReaction(this, out_faction);
	}

	sol::optional<std::tuple<int, Faction*>> Faction::getHighestJoinedReaction_lua() const {
		Faction* faction = nullptr;
		auto reaction = getHighestJoinedReaction(&faction);
		if (faction) {
			return { { reaction, faction } };
		}
		return {};
	}

	const auto TES3_Faction_getAdvancementPotential = reinterpret_cast<Faction::AdvancementPotential(__thiscall*)(const Faction*)>(0x4AD420);
	Faction::AdvancementPotential Faction::getAdvancementPotential() const {
		return TES3_Faction_getAdvancementPotential(this);
	}

	std::reference_wrapper<int[2]> Faction::getAttributes() {
		return std::ref(attributes);
	}

	std::reference_wrapper<int[7]> Faction::getSkills() {
		return std::ref(skills);
	}

	std::reference_wrapper<Faction::Rank[10]> Faction::getRanks() {
		return std::ref(ranks);
	}

	bool Faction::playerJoin_lua() {
		if (getPlayerJoined()) {
			return false;
		}

		setPlayerJoined(true);
		TES3::UI::updateStatsPane();
		return true;
	}

	bool Faction::playerLeave_lua() {
		if (!getPlayerJoined()) {
			return false;
		}

		setPlayerJoined(false);
		TES3::UI::updateStatsPane();
		return true;
	}

	bool Faction::playerExpel_lua() {
		if (!getPlayerJoined() || getPlayerExpelled()) {
			return false;
		}

		setPlayerExpelled(true);
		TES3::UI::updateStatsPane();

		char message[256];
		auto sExpelledMessage = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::sExpelledMessage]->value.asString;
		std::snprintf(message, sizeof(message), "%s%s", sExpelledMessage, name);
		TES3::UI::showMessageBox(message);
		return true;
	}

	bool Faction::playerClearExpel_lua() {
		if (!getPlayerJoined() || !getPlayerExpelled()) {
			return false;
		}

		setPlayerExpelled(false);
		TES3::UI::updateStatsPane();
		return true;
	}

	bool Faction::playerPromote_lua() {
		if (!getPlayerJoined() || getPlayerExpelled()) {
			return false;
		}
		auto nextRank = playerRank + 1;
		if (playerRank == 9 || strlen(getRankName(nextRank)) == 0) {
			return false;
		}

		playerRank = nextRank;
		TES3::UI::updateStatsPane();
		return true;
	}

	bool Faction::playerDemote_lua() {
		if (!getPlayerJoined() || getPlayerExpelled()) {
			return false;
		}
		if (playerRank == 0) {
			return false;
		}

		playerRank--;
		TES3::UI::updateStatsPane();
		return true;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Faction)
