#include "TES3Skill.h"

#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"

namespace TES3 {
	const char* Skill::getName() const {
		return DataHandler::get()->nonDynamicData->GMSTs[getNameGMST()]->value.asString;
	}

	int Skill::getNameGMST() const {
		return NAME_GMSTS[skill];
	}

	std::string Skill::getIconPath() const {
		std::string path = "icons\\k\\";
		path.append(ICON_PATHS[skill]);
		return path;
	}

	std::reference_wrapper<float[4]> Skill::getProgressActions() {
		return std::ref(progressActions);
	}

	const auto TES3_Skill_loadDescription = reinterpret_cast<char* (__thiscall*)(Skill*)>(0x4A8990);
	char* Skill::loadDescription() {
		return TES3_Skill_loadDescription(this);
	}

	const auto TES3_Skill_freeDescription = reinterpret_cast<void(__thiscall*)(Skill*)>(0x4A8C60);
	void Skill::freeDescription() {
		return TES3_Skill_freeDescription(this);
	}

	sol::optional<std::string> Skill::getAndLoadDescription() {
		// If the description is already loaded, just return it.
		if (description) {
			return { description };
		}

		// Otherwise we need to load it from disk, then free it.
		else {
			char* description = loadDescription();
			if (description) {
				// We loaded successfully, package, free, then return.
				std::string value = description;
				freeDescription();
				return std::move(value);
			}
		}

		return {};
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Skill)
