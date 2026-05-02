#include "CSNPC.h"

#include "CSFaction.h"
#include "CSDataHandler.h"
#include "CSRecordHandler.h"
#include "CSGameSetting.h"
#include "StringUtil.h"

namespace se::cs {
	const char* NPC::getFactionRankName() const {
		auto faction = getFaction();
		if (faction == nullptr || factionRank == -1) {
			return "";
		}
		return faction->rankNames[factionRank];
	}

	int NPC::getFight() const {
		// Return the fight value for the NPC
		return aiConfig.fight;
	}

	void NPC::getTraining(char* buffer, size_t bufferSize) const {
		const int TRAINING_SERVICE_FLAG = 16384;
		// If NPC doesn't provide training then the buffer is empty
		if ((aiConfig.merchantFlags & TRAINING_SERVICE_FLAG) == false) {
			buffer[0] = '\0';
			return;
		}
		
		// Store the skills index along with their level in a vector 
		std::vector<std::vector<int>> trainingSkills;
		for (int i = 0; i < sizeof(skills); i++) {
			trainingSkills.push_back({i, skills[i]});
		}

		// Sort the array in descending order based on the second index (skill level)
		std::sort(trainingSkills.begin(), trainingSkills.end(), 
			[](const std::vector<int>& above, const std::vector<int>& below) {
				return (above[1] > below[1]);
			});

		// Construct the skill string, ex: "Alchemy(100), Blunt Weapon(90), Long Blade(60)"
		std::ostringstream oss;
		// Grab the skill name from game settings
		const auto recordHandler = DataHandler::get()->recordHandler;
		oss << recordHandler->getGameSettingForSkill(trainingSkills[0][0])->value.asString << "(" << trainingSkills[0][1] << "), " 
			<< recordHandler->getGameSettingForSkill(trainingSkills[1][0])->value.asString << "(" << trainingSkills[1][1] << "), "
			<< recordHandler->getGameSettingForSkill(trainingSkills[2][0])->value.asString << "(" << trainingSkills[2][1] << ")";
		std::string textToDisplay = oss.str();
		strncpy_s(buffer, bufferSize, textToDisplay.c_str(), textToDisplay.size());
		return;
	}

	bool NPC::search(const std::string_view& needle, const BaseObject::SearchSettings& settings, std::regex* regex) const {
		if (Object::search(needle, settings, regex)) {
			return true;
		}

		if (settings.training) {
			char buffer[256];
			getTraining(buffer, sizeof(buffer));
			return string::complex_contains(buffer, needle, settings, regex);
		}

		return false;
	}
}
