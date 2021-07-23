#include "TES3MagicEffect.h"

#include "BitUtil.h"
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

	const char* MagicEffect::getName() const {
		return TES3::DataHandler::get()->nonDynamicData->magicEffects->getEffectName(id);
	}

	int MagicEffect::getNameGMST() const {
		if (id < EffectID::FirstEffect || id > EffectID::LastEffect) {
			return -1;
		}
		return reinterpret_cast<int*>(0x79454C)[id];
	}

	void MagicEffect::setDescription( const char *value ) {
		mwse::tes3::setDataString( &description, value );
	}

	const char* MagicEffect::getDescription() const noexcept
	{
		return description;
	}

	const char* MagicEffect::getIcon() const {
		return icon;
	}

	void MagicEffect::setIcon(const char* path) {
		strcpy_s(icon, sizeof(icon), path);
	}

	const char* MagicEffect::getParticleTexture() const {
		return particleTexture;
	}

	void MagicEffect::setParticleTexture(const char* path) {
		strcpy_s(particleTexture, sizeof(particleTexture), path);
	}

	const char* MagicEffect::getCastSoundEffect() const {
		return castSoundEffect;
	}

	void MagicEffect::setCastSoundEffect(const char* path) {
		strcpy_s(castSoundEffect, sizeof(castSoundEffect), path);
	}

	const char* MagicEffect::getBoltSoundEffect() const {
		return boltSoundEffect;
	}

	void MagicEffect::setBoltSoundEffect(const char* path) {
		strcpy_s(boltSoundEffect, sizeof(boltSoundEffect), path);
	}

	const char* MagicEffect::getHitSoundEffect() const {
		return hitSoundEffect;
	}

	void MagicEffect::setHitSoundEffect(const char* path) {
		strcpy_s(hitSoundEffect, sizeof(hitSoundEffect), path);
	}

	const char* MagicEffect::getAreaSoundEffect() const {
		return areaSoundEffect;
	}

	void MagicEffect::setAreaSoundEffect(const char* path) {
		strcpy_s(areaSoundEffect, sizeof(areaSoundEffect), path);
	}

	unsigned int MagicEffect::getEffectFlags() const {
		return TES3::DataHandler::get()->nonDynamicData->magicEffects->getEffectFlags(id);
	}

	void MagicEffect::setEffectFlags(unsigned int flags) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlags(id, flags);
	}

	bool MagicEffect::getFlagTargetSkill() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::TargetSkillBit);
	}

	void MagicEffect::setFlagTargetSkill(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::TargetSkillBit, value);
	}

	bool MagicEffect::getFlagTargetAttribute() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::TargetAttributeBit);
	}

	void MagicEffect::setFlagTargetAttribute(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::TargetAttributeBit, value);
	}

	bool MagicEffect::getFlagNoDuration() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::NoDurationBit);
	}

	void MagicEffect::setFlagNoDuration(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::NoDurationBit, value);
	}

	bool MagicEffect::getFlagNoMagnitude() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::NoMagnitudeBit);
	}

	void MagicEffect::setFlagNoMagnitude(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::NoMagnitudeBit, value);
	}

	bool MagicEffect::getFlagHarmful() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::HarmfulBit);
	}

	void MagicEffect::setFlagHarmful(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::HarmfulBit, value);
	}

	bool MagicEffect::getFlagContinuousVFX() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::ContinuousVFXBit);
	}

	void MagicEffect::setFlagContinuousVFX(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::ContinuousVFXBit, value);
	}

	bool MagicEffect::getFlagCanCastSelf() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::CanCastSelfBit);
	}

	void MagicEffect::setFlagCanCastSelf(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::CanCastSelfBit, value);
	}

	bool MagicEffect::getFlagCanCastTouch() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::CanCastTouchBit);
	}

	void MagicEffect::setFlagCanCastTouch(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::CanCastTouchBit, value);
	}

	bool MagicEffect::getFlagCanCastTarget() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::CanCastTargetBit);
	}

	void MagicEffect::setFlagCanCastTarget(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::CanCastTargetBit, value);
	}

	bool MagicEffect::getFlagNegativeLighting() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::NegativeLightingBit);
	}

	void MagicEffect::setFlagNegativeLighting(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::NegativeLightingBit, value);
	}

	bool MagicEffect::getFlagAppliedOnce() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::AppliedOnceBit);
	}

	void MagicEffect::setFlagAppliedOnce(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::AppliedOnceBit, value);
	}

	bool MagicEffect::getFlagNonRecastable() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::NonRecastableBit);
	}

	void MagicEffect::setFlagNonRecastable(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::NonRecastableBit, value);
	}

	bool MagicEffect::getFlagIllegalDaedra() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::IllegalDaedraBit);
	}

	void MagicEffect::setFlagIllegalDaedra(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::IllegalDaedraBit, value);
	}

	bool MagicEffect::getFlagUnreflectable() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::UnreflectableBit);
	}

	void MagicEffect::setFlagUnreflectable(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::UnreflectableBit, value);
	}

	bool MagicEffect::getFlagCasterLinked() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::CasterLinkedBit);
	}

	void MagicEffect::setFlagCasterLinked(bool value) const {
		TES3::DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, TES3::EffectFlag::CasterLinkedBit, value);
	}

	bool MagicEffect::getAllowSpellmaking() const {
		return BIT_TEST(flags, EffectFlag::AllowSpellmakingBit);
	}

	void MagicEffect::setAllowSpellmaking(bool value) {
		BIT_SET(flags, EffectFlag::AllowSpellmakingBit, value);
	}

	bool MagicEffect::getAllowEnchanting() const {
		return BIT_TEST(flags, EffectFlag::AllowEnchantingBit);
	}

	void MagicEffect::setAllowEnchanting(bool value) {
		BIT_SET(flags, EffectFlag::AllowEnchantingBit, value);
	}

	int MagicEffect::getSkillForSchool() const {
		return reinterpret_cast<int*>(0x7926B8)[school];
	}

	const auto TES3_Effect_calculateCost = reinterpret_cast<double(__cdecl*)(const Effect*)>(0x4AA700);
	float Effect::calculateCost() const {
		return TES3_Effect_calculateCost(this);
	}

	MagicEffect* Effect::getEffectData() const {
		return TES3::DataHandler::get()->nonDynamicData->magicEffects->getEffectObject(effectID);
	}

	bool Effect::matchesEffectsWith(const Effect * other ) const {
		return effectID == other->effectID &&
			skillID == other->skillID &&
			attributeID == other->attributeID &&
			rangeType == other->rangeType &&
			radius == other->radius &&
			duration == other->duration &&
			magnitudeMin == other->magnitudeMin &&
			magnitudeMax == other->magnitudeMax;
	}

	signed char Effect::getSkillID() const {
		return skillID;
	}

	void Effect::setSkillID(signed char id) {
		skillID = id;
	}

	void Effect::setSkillID_lua(sol::optional<signed char> id) {
		setSkillID(id.value_or(-1));
	}

	signed char Effect::geAttributeID() const {
		return attributeID;
	}

	void Effect::seAttributeID(signed char id) {
		attributeID = id;
	}

	void Effect::seAttributeID_lua(sol::optional<signed char> id) {
		seAttributeID(id.value_or(-1));
	}


	sol::optional<std::string> Effect::toString() const {
		if (effectID == -1) {
			return {};
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

		return std::move(ss.str());
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::MagicEffect)
