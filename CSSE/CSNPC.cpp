#include "CSNPC.h"

#include "CSFaction.h"

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

	std::string NPC::getTraining() const {
		const int TRAINING_SERVICE_FLAG = 16384;
		// If provides a training
		if (aiConfig.merchantFlags & TRAINING_SERVICE_FLAG) {
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
			oss << getSkillName(trainingSkills[0][0]) << "(" << trainingSkills[0][1] << "), " << getSkillName(trainingSkills[1][0]) << "(" << trainingSkills[1][1] << "), " << getSkillName(trainingSkills[2][0]) << "(" << trainingSkills[2][1] << ")";
			std::string textToDisplay = oss.str();
			return textToDisplay;
		} 
		
		// Not a trainer
		return "";
		
	}

	const char* NPC::getSkillName(int skillIndex) const {
		const char* skillNames[27] = {
			"Block", 
			"Armorer", 
			"Medium Armor", 
			"Heavy Armor",
			"Blunt Weapon",
			"Long Blade",
			"Axe",
			"Spear",
			"Athletics",
			"Enchant",
			"Destruction",
			"Alteration",
			"Illusion",
			"Conjuration",
			"Mysticism",
			"Restoration",
			"Alchemy",
			"Unarmored",
			"Security",
			"Sneak",
			"Acrobatics",
			"Light armor",
			"Short Blade",
			"Marksman",
			"Mercantile",
			"Speechcraft",
			"Hand-to-hand"
		};
		
		return skillNames[skillIndex];
	}
}
