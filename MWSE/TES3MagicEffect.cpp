#include "TES3MagicEffect.h"

#include "BitUtil.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3MagicEffectController.h"
#include "TES3MobileActor.h"
#include "TES3Sound.h"

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
	void MagicEffect::resolveLinks(NonDynamicData* nonDynamicData) {
		TES3_MagicEffect_resolveLinks(this, nonDynamicData);
	}

	const auto TES3_MagicEffect_clearData = reinterpret_cast<void(__thiscall*)(MagicEffect*)>(0x4A8D20);
	void MagicEffect::clearData() {
		TES3_MagicEffect_clearData(this);
	}

	const char* MagicEffect::getName() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectName(id);
	}

	std::string MagicEffect::getComplexName(int attribute, int skill) const {
		auto ndd = DataHandler::get()->nonDynamicData;
		const auto extendedData = getExtendedData();

		std::stringstream ss;

		int nameGMST = getNameGMST();
		if (skill >= 0 && getFlagTargetSkill()) {
			const char* skillName = ndd->GMSTs[mwse::tes3::getSkillNameGMST(skill)]->value.asString;
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
		else if (attribute >= 0 && getFlagTargetAttribute()) {
			const char* attributeName = ndd->GMSTs[mwse::tes3::getAttributeNameGMST(attribute)]->value.asString;
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
		else if (extendedData && !extendedData->name.empty()) {
			ss << extendedData->name;
		}
		else {
			ss << "<invalid effect>";
		}

		return std::move(ss.str());
	}

	int MagicEffect::getNameGMST() const {
		if (id < EffectID::FirstEffect || id > EffectID::LastEffect) {
			return -1;
		}
		return reinterpret_cast<int*>(0x79454C)[id];
	}

	void MagicEffect::setDescription(const char* value) {
		mwse::tes3::setDataString(&description, value);
	}

	const char* MagicEffect::getDescription() const noexcept {
		return description;
	}

	const char* MagicEffect::getIcon() const {
		return icon;
	}

	std::string MagicEffect::getBigIcon() const {
		std::string bigIcon = icon;

		auto lastSeparator = bigIcon.rfind('\\');
		if (lastSeparator != std::string::npos) {
			bigIcon.insert(lastSeparator + 1, "B_");
		}
		else {
			bigIcon.insert(0, "B_");
		}

		return std::move(bigIcon);
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

	const auto TES3_MagicEffect_getCastSoundID = reinterpret_cast<const char* (__thiscall*)(const MagicEffect*)>(0x4A9E40);
	Sound* MagicEffect::getCastSoundEffect() const {
		auto id = TES3_MagicEffect_getCastSoundID(this);
		return static_cast<Sound*>(DataHandler::get()->nonDynamicData->findSound(id));
	}

	void MagicEffect::setCastSoundEffect(Sound* sound) {
		auto id = sound ? sound->id : "";
		strcpy_s(castSoundEffectID, sizeof(castSoundEffectID), id);
	}

	const auto TES3_MagicEffect_getBoltSoundID = reinterpret_cast<const char* (__thiscall*)(const MagicEffect*)>(0x4A9E60);
	Sound* MagicEffect::getBoltSoundEffect() const {
		auto id = TES3_MagicEffect_getBoltSoundID(this);
		return static_cast<Sound*>(DataHandler::get()->nonDynamicData->findSound(id));
	}

	void MagicEffect::setBoltSoundEffect(Sound* sound) {
		auto id = sound ? sound->id : "";
		strcpy_s(boltSoundEffectID, sizeof(boltSoundEffectID), id);
	}

	const auto TES3_MagicEffect_getHitSoundID = reinterpret_cast<const char* (__thiscall*)(const MagicEffect*)>(0x4A9E80);
	Sound* MagicEffect::getHitSoundEffect() const {
		auto id = TES3_MagicEffect_getHitSoundID(this);
		return static_cast<Sound*>(DataHandler::get()->nonDynamicData->findSound(id));
	}

	void MagicEffect::setHitSoundEffect(Sound* sound) {
		auto id = sound ? sound->id : "";
		strcpy_s(hitSoundEffectID, sizeof(hitSoundEffectID), id);
	}

	const auto TES3_MagicEffect_getAreaSoundID = reinterpret_cast<const char* (__thiscall*)(const MagicEffect*)>(0x4A9EA0);
	Sound* MagicEffect::getAreaSoundEffect() const {
		auto id = TES3_MagicEffect_getAreaSoundID(this);
		return static_cast<Sound*>(DataHandler::get()->nonDynamicData->findSound(id));
	}

	void MagicEffect::setAreaSoundEffect(Sound* sound) {
		auto id = sound ? sound->id : "";
		strcpy_s(areaSoundEffectID, sizeof(areaSoundEffectID), id);
	}

	const auto TES3_MagicEffect_getSpellFailureSound = reinterpret_cast<Sound * (__thiscall*)(const MagicEffect*)>(0x4A9F00);
	Sound* MagicEffect::getSpellFailureSoundEffect() const {
		return TES3_MagicEffect_getSpellFailureSound(this);
	}

	unsigned int MagicEffect::getEffectFlags() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlags(id);
	}

	void MagicEffect::setEffectFlags(unsigned int flags) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlags(id, flags);
	}

	bool MagicEffect::getFlagTargetSkill() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::TargetSkillBit);
	}

	void MagicEffect::setFlagTargetSkill(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::TargetSkillBit, value);
	}

	bool MagicEffect::getFlagTargetAttribute() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::TargetAttributeBit);
	}

	void MagicEffect::setFlagTargetAttribute(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::TargetAttributeBit, value);
	}

	bool MagicEffect::getFlagNoDuration() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::NoDurationBit);
	}

	void MagicEffect::setFlagNoDuration(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::NoDurationBit, value);
	}

	bool MagicEffect::getFlagNoMagnitude() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::NoMagnitudeBit);
	}

	void MagicEffect::setFlagNoMagnitude(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::NoMagnitudeBit, value);
	}

	bool MagicEffect::getFlagHarmful() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::HarmfulBit);
	}

	void MagicEffect::setFlagHarmful(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::HarmfulBit, value);
	}

	bool MagicEffect::getFlagContinuousVFX() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::ContinuousVFXBit);
	}

	void MagicEffect::setFlagContinuousVFX(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::ContinuousVFXBit, value);
	}

	bool MagicEffect::getFlagCanCastSelf() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::CanCastSelfBit);
	}

	void MagicEffect::setFlagCanCastSelf(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::CanCastSelfBit, value);
	}

	bool MagicEffect::getFlagCanCastTouch() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::CanCastTouchBit);
	}

	void MagicEffect::setFlagCanCastTouch(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::CanCastTouchBit, value);
	}

	bool MagicEffect::getFlagCanCastTarget() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::CanCastTargetBit);
	}

	void MagicEffect::setFlagCanCastTarget(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::CanCastTargetBit, value);
	}

	bool MagicEffect::getFlagNegativeLighting() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::NegativeLightingBit);
	}

	void MagicEffect::setFlagNegativeLighting(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::NegativeLightingBit, value);
	}

	bool MagicEffect::getFlagAppliedOnce() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::AppliedOnceBit);
	}

	void MagicEffect::setFlagAppliedOnce(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::AppliedOnceBit, value);
	}

	bool MagicEffect::getFlagNonRecastable() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::NonRecastableBit);
	}

	void MagicEffect::setFlagNonRecastable(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::NonRecastableBit, value);
	}

	bool MagicEffect::getFlagIllegalDaedra() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::IllegalDaedraBit);
	}

	void MagicEffect::setFlagIllegalDaedra(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::IllegalDaedraBit, value);
	}

	bool MagicEffect::getFlagUnreflectable() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::UnreflectableBit);
	}

	void MagicEffect::setFlagUnreflectable(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::UnreflectableBit, value);
	}

	bool MagicEffect::getFlagCasterLinked() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectFlag(id, EffectFlag::CasterLinkedBit);
	}

	void MagicEffect::setFlagCasterLinked(bool value) const {
		DataHandler::get()->nonDynamicData->magicEffects->setEffectFlag(id, EffectFlag::CasterLinkedBit, value);
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

	MagicEffectExtendedData* MagicEffect::getExtendedData() const {
		return DataHandler::get()->nonDynamicData->magicEffects->effectExtendedData[id];
	}


	//
	// MagicEffectExtendedData
	//

	MagicEffectExtendedData::MagicEffectExtendedData() {
		name = "Unnamed Effect";
	}

	bool MagicEffectExtendedData::hasName() const {
		return !name.empty();
	}

	bool MagicEffectExtendedData::hasMagnitudeType() const {
		return !magnitudeType.empty() && !magnitudeTypePlural.empty();
	}

	std::string_view MagicEffectExtendedData::getMagnitudeType(bool plural) const {
		if (plural) {
			return magnitudeTypePlural;
		}
		else {
			return magnitudeType;
		}
	}


	//
	// Effect
	//

	Effect::Effect() {
		effectID = EffectID::None;
		skillID = SkillID::Invalid;
		attributeID = Attribute::Invalid;
		rangeType = EffectRange::Invalid;
		radius = 0;
		duration = 0;
		magnitudeMin = 0;
		magnitudeMax = 0;
	}

	Effect::Effect(const Effect& from) {
		effectID = from.effectID;
		skillID = from.skillID;
		attributeID = from.attributeID;
		rangeType = from.rangeType;
		radius = from.radius;
		duration = from.duration;
		magnitudeMin = from.magnitudeMin;
		magnitudeMax = from.magnitudeMax;
	}

	Effect::Effect(const sol::table& from) {
		effectID = from.get_or("id", EffectID::None);
		skillID = from.get_or("skill", SkillID::Invalid);
		attributeID = from.get_or("attribute", Attribute::Invalid);
		rangeType = from.get_or("rangeType", EffectRange::Self);
		radius = from.get_or("radius", 0);
		duration = from.get_or("duration", 0);
		magnitudeMin = from.get_or("min", 0);
		magnitudeMax = from.get_or("max", 0);
	}

	Effect& Effect::operator=(const Effect& effect) {
		effectID = effect.effectID;
		skillID = effect.skillID;
		attributeID = effect.attributeID;
		rangeType = effect.rangeType;
		radius = effect.radius;
		duration = effect.duration;
		magnitudeMin = effect.magnitudeMin;
		magnitudeMax = effect.magnitudeMax;
		return *this;
	}

	Effect& Effect::operator=(const sol::table table) {
		effectID = table.get_or("id", EffectID::None);
		skillID = table.get_or("skill", SkillID::Invalid);
		attributeID = table.get_or("attribute", Attribute::Invalid);
		rangeType = table.get_or("rangeType", EffectRange::Self);
		radius = table.get_or("radius", 0);
		duration = table.get_or("duration", 0);
		magnitudeMin = table.get_or("min", 0);
		magnitudeMax = table.get_or("max", 0);
		return *this;
	}

	Effect& Effect::operator=(const sol::object object) {
		if (object.is<Effect>()) {
			*this = object.as<Effect>();
		}
		else if (object.is<sol::table>()) {
			*this = object.as<sol::table>();
		}
		else {
			throw std::invalid_argument("Could not convert lua object to tes3effect.");
		}
		return *this;
	}

	bool Effect::operator==(const Effect& other) const {
		return effectID == other.effectID &&
			skillID == other.skillID &&
			attributeID == other.attributeID &&
			rangeType == other.rangeType &&
			radius == other.radius &&
			duration == other.duration &&
			magnitudeMin == other.magnitudeMin &&
			magnitudeMax == other.magnitudeMax;
	}

	bool Effect::operator!=(const Effect& other) const {
		return effectID != other.effectID ||
			skillID != other.skillID ||
			attributeID != other.attributeID ||
			rangeType != other.rangeType ||
			radius != other.radius ||
			duration != other.duration ||
			magnitudeMin != other.magnitudeMin ||
			magnitudeMax != other.magnitudeMax;
	}

	const auto TES3_Effect_calculateCost = reinterpret_cast<double(__cdecl*)(const Effect*)>(0x4AA700);
	float Effect::calculateCost() const {
		return TES3_Effect_calculateCost(this);
	}

	MagicEffect* Effect::getEffectData() const {
		return DataHandler::get()->nonDynamicData->magicEffects->getEffectObject(effectID);
	}

	bool Effect::matchesEffectsWith(const Effect* other) const {
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
		if (effectID == EffectID::None) {
			return {};
		}

		// Some data we'll want to hold onto.
		auto ndd = DataHandler::get()->nonDynamicData;
		const auto effectData = getEffectData();
		if (effectData == nullptr) {
			return {};
		}

		const auto extendedData = effectData->getExtendedData();

		// We'll use a string stream and build up the result.
		std::stringstream ss;

		// Get the base name. If the effect uses skills/attributes we need to remap the name.
		ss << effectData->getComplexName(attributeID, skillID);

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
				const auto pluralMagnitude = (magnitudeMax != 1);
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
					if (pluralMagnitude) {
						ss << " " << ndd->GMSTs[GMST::sfeet]->value.asString;
					}
					else {
						ss << " " << ndd->GMSTs[GMST::sfootarea]->value.asString;
					}
					break;
				case EffectID::CommandCreature:
				case EffectID::CommandHumanoid:
					if (pluralMagnitude) {
						ss << " " << ndd->GMSTs[GMST::sLevels]->value.asString;
					}
					else {
						ss << " " << ndd->GMSTs[GMST::sLevel]->value.asString;
					}
					break;
				default:
					if (extendedData && extendedData->hasMagnitudeType()) {
						ss << extendedData->getMagnitudeType(pluralMagnitude);
					}
					else if (pluralMagnitude) {
						ss << " " << ndd->GMSTs[GMST::spoints]->value.asString;
					}
					else {
						ss << " " << ndd->GMSTs[GMST::spoint]->value.asString;
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
