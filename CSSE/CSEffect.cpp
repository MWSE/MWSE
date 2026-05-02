#include "CSEffect.h"

#include "CSMagicEffect.h"
#include "CSDataHandler.h"
#include "CSRecordHandler.h"
#include "CSGameSetting.h"

namespace se::cs {
	MagicEffect* Effect::getEffectData() const {
		return DataHandler::get()->recordHandler->getMagicEffect(effectID);
	}

	std::optional<std::string> Effect::toString() const {
		if (effectID == EffectID::None) {
			return {};
		}

		// Some data we'll want to hold onto.
		auto recordHandler = DataHandler::get()->recordHandler;
		const auto effectData = getEffectData();
		if (effectData == nullptr) {
			return {};
		}

		// We'll use a string stream and build up the result.
		std::stringstream ss;

		// Get the base name. If the effect uses skills/attributes we need to remap the name.
		ss << effectData->getComplexName(attributeID, skillID);

		// Add on the magnitude. Fortify magicka has its own logic because it has an x suffix.
		if (effectID == EffectID::FortifyMagickaMultiplier) {
			float min = magnitudeMin * 0.1f;
			if (magnitudeMin == magnitudeMax) {
				ss << " " << min << recordHandler->gameSettingsHandler->gameSettings[GMST::sXTimesINT]->value.asString;
			}
			else {
				float max = magnitudeMax * 0.1f;
				ss << " " << min << recordHandler->gameSettingsHandler->gameSettings[GMST::sXTimes]->value.asString << " " << recordHandler->gameSettingsHandler->gameSettings[GMST::sTo]->value.asString << " " << max << recordHandler->gameSettingsHandler->gameSettings[GMST::sXTimesINT]->value.asString;
			}
		}
		else {
			if (!recordHandler->magicEffects[effectID].getFlagNoMagnitude()) {
				if (magnitudeMin != magnitudeMax) {
					ss << " " << magnitudeMin << " " << recordHandler->gameSettingsHandler->gameSettings[GMST::sTo]->value.asString << " " << magnitudeMax;
				}
				else {
					ss << " " << magnitudeMin;
				}

				switch (effectID) {
				case EffectID::WeaknessToFire:
				case EffectID::WeaknessToFrost:
				case EffectID::WeaknessToShock:
				case EffectID::WeaknessToMagicka:
				case EffectID::WeaknessToCommonDisease:
				case EffectID::WeaknessToBlightDisease:
				case EffectID::WeaknessToCorprus:
				case EffectID::WeaknessToPoison:
				case EffectID::WeaknessToNormalWeapons:
				case EffectID::Chameleon:
				case EffectID::Blind:
				case EffectID::Dispel:
				case EffectID::Reflect:
				case EffectID::ResistFire:
				case EffectID::ResistFrost:
				case EffectID::ResistShock:
				case EffectID::ResistMagicka:
				case EffectID::ResistCommonDisease:
				case EffectID::ResistBlightDisease:
				case EffectID::ResistCorprus:
				case EffectID::ResistPoison:
				case EffectID::ResistNormalWeapons:
				case EffectID::ResistParalysis:
					ss << recordHandler->gameSettingsHandler->gameSettings[GMST::spercent]->value.asString;
					break;
				case EffectID::Telekinesis:
				case EffectID::DetectAnimal:
				case EffectID::DetectEnchantment:
				case EffectID::DetectKey:
					if (magnitudeMax == 1) {
						ss << " " << recordHandler->gameSettingsHandler->gameSettings[GMST::sfootarea]->value.asString;
					}
					else {
						ss << " " << recordHandler->gameSettingsHandler->gameSettings[GMST::sfeet]->value.asString;
					}
					break;
				case EffectID::CommandCreature:
				case EffectID::CommandHumanoid:
					if (magnitudeMax == 1) {
						ss << " " << recordHandler->gameSettingsHandler->gameSettings[GMST::sLevel]->value.asString;
					}
					else {
						ss << " " << recordHandler->gameSettingsHandler->gameSettings[GMST::sLevels]->value.asString;
					}
					break;
				default:
					if (magnitudeMax == 1) {
						ss << " " << recordHandler->gameSettingsHandler->gameSettings[GMST::spoint]->value.asString;
					}
					else {
						ss << " " << recordHandler->gameSettingsHandler->gameSettings[GMST::spoints]->value.asString;
					}
				}
			}
		}

		return ss.str();
	}

	bool Effect::search(const std::string_view& needle, const BaseObject::SearchSettings& settings, std::regex* regex) const {
		const auto effectData = getEffectData();
		if (effectData == nullptr) {
			return false;
		}

		return effectData->search(needle, settings, regex, attributeID, skillID);
	}
}