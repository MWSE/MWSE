#include "TES3MagicEffect.h"

#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3MagicEffectController.h"

namespace TES3 {
	const auto TES3_MagicEffect_ctor = reinterpret_cast<void(__thiscall*)(MagicEffect*)>(0x4A8C90);
	MagicEffect::MagicEffect() {
		TES3_MagicEffect_ctor(this);
	}

	MagicEffect::MagicEffect(int newId) {
		TES3_MagicEffect_ctor(this);
		id = newId;
	}

	const auto TES3_MagicEffect_dtor = reinterpret_cast<void(__thiscall*)(MagicEffect*)>(0x4A8E70);
	MagicEffect::~MagicEffect() {
		TES3_MagicEffect_dtor(this);
	}

	const auto TES3_MagicEffect_resolveLinks = reinterpret_cast<void(__thiscall*)(MagicEffect*, NonDynamicData*)>(0x4A9240);
	void MagicEffect::resolveLinks(NonDynamicData * nonDynamicData) {
		TES3_MagicEffect_resolveLinks(this, nonDynamicData);
	}

	const auto TES3_MagicEffect_clearData = reinterpret_cast<void(__thiscall*)(MagicEffect*)>(0x4A8D20);
	void MagicEffect::clearData() {
		TES3_MagicEffect_clearData(this);
	}

	int MagicEffect::getNameGMST() {
		if (id < EffectID::FirstEffect || id > EffectID::LastEffect) {
			return -1;
		}
		return reinterpret_cast<int*>(0x79454C)[id];
	}

	void MagicEffect::setDescription( const char *value )
	{
		mwse::tes3::setDataString( &description, value );
	}

	MagicEffect * Effect::getEffectData() {
		return TES3::DataHandler::get()->nonDynamicData->magicEffects->getEffectObject(effectID);
	}

	bool Effect::matchesEffectsWith(const Effect * other ) {
		return effectID == other->effectID &&
			skillID == other->skillID &&
			attributeID == other->attributeID &&
			rangeType == other->rangeType &&
			radius == other->radius &&
			duration == other->duration &&
			magnitudeMin == other->magnitudeMin &&
			magnitudeMax == other->magnitudeMax;
	}

	std::string Effect::toString() {
		if (effectID == -1) {
			return "";
		}

		// Some data we'll want to hold onto.
		NonDynamicData * ndd = TES3::DataHandler::get()->nonDynamicData;
		MagicEffect * effectData = getEffectData();

		// We'll use a string stream and build up the result.
		std::stringstream ss;

		// Get the base name. If the effect uses skills/attributes we need to remap the name.
		int nameGMST = effectData->getNameGMST();
		if (ndd->magicEffects->getEffectFlag(effectID, EffectFlag::TargetSkillBit)) {
			const char* skillName = ndd->GMSTs[mwse::tes3::getSkillNameGMST(skillID)]->value.asString;
			switch (nameGMST) {
			case GMST::sEffectFortifySkill:
				ss << ndd->GMSTs[GMST::sFortify]->value.asString << " " << skillName;
				break;
			case GMST::sEffectDrainSkill:
				ss << ndd->GMSTs[GMST::sDrain]->value.asString << " " << skillName;
				break;
			case GMST::sEffectDamageSkill:
				ss << ndd->GMSTs[GMST::sDamage]->value.asString << " " << skillName;
				break;
			case GMST::sEffectRestoreSkill:
				ss << ndd->GMSTs[GMST::sRestore]->value.asString << " " << skillName;
				break;
			case GMST::sEffectAbsorbSkill:
				ss << ndd->GMSTs[GMST::sAbsorb]->value.asString << " " << skillName;
				break;
			}
		}
		else if (ndd->magicEffects->getEffectFlag(effectID, EffectFlag::TargetAttributeBit)) {
			const char* attributeName = ndd->GMSTs[mwse::tes3::getAttributeNameGMST(attributeID)]->value.asString;
			switch (nameGMST) {
			case GMST::sEffectFortifyAttribute:
				ss << ndd->GMSTs[GMST::sFortify]->value.asString << " " << attributeName;
				break;
			case GMST::sEffectDrainAttribute:
				ss << ndd->GMSTs[GMST::sDrain]->value.asString << " " << attributeName;
				break;
			case GMST::sEffectDamageAttribute:
				ss << ndd->GMSTs[GMST::sDamage]->value.asString << " " << attributeName;
				break;
			case GMST::sEffectRestoreAttribute:
				ss << ndd->GMSTs[GMST::sRestore]->value.asString << " " << attributeName;
				break;
			case GMST::sEffectAbsorbAttribute:
				ss << ndd->GMSTs[GMST::sAbsorb]->value.asString << " " << attributeName;
				break;
			}
		}
		else if (nameGMST > 0) {
			ss << ndd->GMSTs[nameGMST]->value.asString;
		}
		else {
			ss << ndd->magicEffects->effectCustomNames[effectID];
		}

		// Add on the magnitude. Fortify magicka has its own logic because it has an x suffix.
		if (effectID == EffectID::FortifyMagickaMultiplier) {
			float min = magnitudeMin * 0.1f;
			if (magnitudeMin == magnitudeMax) {
				ss << " " << min << ndd->GMSTs[GMST::sXTimesINT]->value.asString;
			}
			else {
				float max = magnitudeMax * 0.1f;
				ss << " " << min << ndd->GMSTs[GMST::sXTimes]->value.asString << " " << ndd->GMSTs[GMST::sTo]->value.asString << " " << max << ndd->GMSTs[GMST::sXTimesINT]->value.asString;
			}
		}
		else {
			if (!ndd->magicEffects->getEffectFlag(effectID, EffectFlag::NoMagnitudeBit)) {
				if (magnitudeMin != magnitudeMax) {
					ss << " " << magnitudeMin << " " << ndd->GMSTs[GMST::sTo]->value.asString << " " << magnitudeMax;
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
					ss << ndd->GMSTs[GMST::spercent]->value.asString;
					break;
				case EffectID::Telekinesis:
				case EffectID::DetectAnimal:
				case EffectID::DetectEnchantment:
				case EffectID::DetectKey:
					if (magnitudeMax == 1) {
						ss << " " << ndd->GMSTs[GMST::sfootarea]->value.asString;
					}
					else {
						ss << " " << ndd->GMSTs[GMST::sfeet]->value.asString;
					}
					break;
				case EffectID::CommandCreature:
				case EffectID::CommandHumanoid:
					if (magnitudeMax == 1) {
						ss << " " << ndd->GMSTs[GMST::sLevel]->value.asString;
					}
					else {
						ss << " " << ndd->GMSTs[GMST::sLevels]->value.asString;
					}
					break;
				default:
					if (magnitudeMax == 1) {
						ss << " " << ndd->GMSTs[GMST::spoint]->value.asString;
					}
					else {
						ss << " " << ndd->GMSTs[GMST::spoints]->value.asString;
					}
				}
			}
		}

		// Add on the duration.
		if (!ndd->magicEffects->getEffectFlag(effectID, EffectFlag::NoDurationBit) && duration > 1) {
			ss << " " << ndd->GMSTs[GMST::sfor]->value.asString << " " << duration << " " << ndd->GMSTs[GMST::sseconds]->value.asString;
		}

		// Add on range.
		if (radius) {
			ss << " " << ndd->GMSTs[GMST::sin]->value.asString << " " << radius << " " << ndd->GMSTs[GMST::sfootarea]->value.asString;
		}

		// Add on range type.
		ss << " " << ndd->GMSTs[GMST::sonword]->value.asString << " " << ndd->GMSTs[mwse::tes3::getCastRangeNameGMST((int)rangeType)]->value.asString;

		return ss.str();
	}
}
