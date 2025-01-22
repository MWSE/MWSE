#include "CSMagicEffect.h"

#include "CSDataHandler.h"
#include "CSRecordHandler.h"
#include "CSGameSetting.h"

#include "BitUtil.h"
#include "StringUtil.h"

namespace se::cs {
	const char* MagicEffect::getName() const {
		const auto gmst = getNameGMST();
		if (gmst == -1) {
			return nullptr;
		}
		return DataHandler::get()->recordHandler->gameSettingsHandler->gameSettings[gmst]->value.asString;
	}

	std::string MagicEffect::getComplexName(int attribute, int skill) const {
		auto ndd = DataHandler::get()->recordHandler;

		std::stringstream ss;

		const auto nameGMST = getNameGMST();
		if (skill >= 0 && getFlagTargetSkill()) {
			const auto skillName = ndd->getGameSettingForSkill(skill)->value.asString;
			switch (nameGMST) {
			case GMST::sEffectFortifySkill:
				ss << ndd->gameSettingsHandler->gameSettings[GMST::sFortify]->value.asString << " " << skillName;
				break;
			case GMST::sEffectDrainSkill:
				ss << ndd->gameSettingsHandler->gameSettings[GMST::sDrain]->value.asString << " " << skillName;
				break;
			case GMST::sEffectDamageSkill:
				ss << ndd->gameSettingsHandler->gameSettings[GMST::sDamage]->value.asString << " " << skillName;
				break;
			case GMST::sEffectRestoreSkill:
				ss << ndd->gameSettingsHandler->gameSettings[GMST::sRestore]->value.asString << " " << skillName;
				break;
			case GMST::sEffectAbsorbSkill:
				ss << ndd->gameSettingsHandler->gameSettings[GMST::sAbsorb]->value.asString << " " << skillName;
				break;
			}
		}
		else if (attribute >= 0 && getFlagTargetAttribute()) {
			const auto attributeName = ndd->getGameSettingForAttribute(attribute)->value.asString;
			switch (nameGMST) {
			case GMST::sEffectFortifyAttribute:
				ss << ndd->gameSettingsHandler->gameSettings[GMST::sFortify]->value.asString << " " << attributeName;
				break;
			case GMST::sEffectDrainAttribute:
				ss << ndd->gameSettingsHandler->gameSettings[GMST::sDrain]->value.asString << " " << attributeName;
				break;
			case GMST::sEffectDamageAttribute:
				ss << ndd->gameSettingsHandler->gameSettings[GMST::sDamage]->value.asString << " " << attributeName;
				break;
			case GMST::sEffectRestoreAttribute:
				ss << ndd->gameSettingsHandler->gameSettings[GMST::sRestore]->value.asString << " " << attributeName;
				break;
			case GMST::sEffectAbsorbAttribute:
				ss << ndd->gameSettingsHandler->gameSettings[GMST::sAbsorb]->value.asString << " " << attributeName;
				break;
			}
		}
		else if (nameGMST > 0) {
			ss << ndd->gameSettingsHandler->gameSettings[nameGMST]->value.asString;
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
		const auto gConvertEffectToGMST = reinterpret_cast<int*>(0x6A7E74);
		return gConvertEffectToGMST[id];
	}

	unsigned int MagicEffect::getEffectFlags() const {
		const auto gEffectFlags = reinterpret_cast<int*>(0x673A60);
		return gEffectFlags[id];
	}

	void MagicEffect::setEffectFlags(unsigned int flags) const {
		const auto gEffectFlags = reinterpret_cast<int*>(0x673A60);
		gEffectFlags[id] = flags;
	}

	bool MagicEffect::getEffectFlag(unsigned int flag) const {
		const auto gEffectFlags = reinterpret_cast<int*>(0x673A60);
		return BIT_TEST(gEffectFlags[id], flag);
	}

	void MagicEffect::setEffectFlag(unsigned int flag, bool value) const {
		const auto gEffectFlags = reinterpret_cast<int*>(0x673A60);
		BIT_SET(gEffectFlags[id], flag, value);
	}

	bool MagicEffect::getFlagTargetSkill() const {
		return getEffectFlag(EffectFlag::TargetSkillBit);
	}

	void MagicEffect::setFlagTargetSkill(bool value) const {
		setEffectFlag(EffectFlag::TargetSkillBit, value);
	}

	bool MagicEffect::getFlagTargetAttribute() const {
		return getEffectFlag(EffectFlag::TargetAttributeBit);
	}

	void MagicEffect::setFlagTargetAttribute(bool value) const {
		setEffectFlag(EffectFlag::TargetAttributeBit, value);
	}

	bool MagicEffect::getFlagNoDuration() const {
		return getEffectFlag(EffectFlag::NoDurationBit);
	}

	void MagicEffect::setFlagNoDuration(bool value) const {
		setEffectFlag(EffectFlag::NoDurationBit, value);
	}

	bool MagicEffect::getFlagNoMagnitude() const {
		return getEffectFlag(EffectFlag::NoMagnitudeBit);
	}

	void MagicEffect::setFlagNoMagnitude(bool value) const {
		setEffectFlag(EffectFlag::NoMagnitudeBit, value);
	}

	bool MagicEffect::getFlagHarmful() const {
		return getEffectFlag(EffectFlag::HarmfulBit);
	}

	void MagicEffect::setFlagHarmful(bool value) const {
		setEffectFlag(EffectFlag::HarmfulBit, value);
	}

	bool MagicEffect::getFlagContinuousVFX() const {
		return getEffectFlag(EffectFlag::ContinuousVFXBit);
	}

	void MagicEffect::setFlagContinuousVFX(bool value) const {
		setEffectFlag(EffectFlag::ContinuousVFXBit, value);
	}

	bool MagicEffect::getFlagCanCastSelf() const {
		return getEffectFlag(EffectFlag::CanCastSelfBit);
	}

	void MagicEffect::setFlagCanCastSelf(bool value) const {
		setEffectFlag(EffectFlag::CanCastSelfBit, value);
	}

	bool MagicEffect::getFlagCanCastTouch() const {
		return getEffectFlag(EffectFlag::CanCastTouchBit);
	}

	void MagicEffect::setFlagCanCastTouch(bool value) const {
		setEffectFlag(EffectFlag::CanCastTouchBit, value);
	}

	bool MagicEffect::getFlagCanCastTarget() const {
		return getEffectFlag(EffectFlag::CanCastTargetBit);
	}

	void MagicEffect::setFlagCanCastTarget(bool value) const {
		setEffectFlag(EffectFlag::CanCastTargetBit, value);
	}

	bool MagicEffect::getFlagNegativeLighting() const {
		return getEffectFlag(EffectFlag::NegativeLightingBit);
	}

	void MagicEffect::setFlagNegativeLighting(bool value) const {
		setEffectFlag(EffectFlag::NegativeLightingBit, value);
	}

	bool MagicEffect::getFlagAppliedOnce() const {
		return getEffectFlag(EffectFlag::AppliedOnceBit);
	}

	void MagicEffect::setFlagAppliedOnce(bool value) const {
		setEffectFlag(EffectFlag::AppliedOnceBit, value);
	}

	bool MagicEffect::getFlagNonRecastable() const {
		return getEffectFlag(EffectFlag::NonRecastableBit);
	}

	void MagicEffect::setFlagNonRecastable(bool value) const {
		setEffectFlag(EffectFlag::NonRecastableBit, value);
	}

	bool MagicEffect::getFlagIllegalDaedra() const {
		return getEffectFlag(EffectFlag::IllegalDaedraBit);
	}

	void MagicEffect::setFlagIllegalDaedra(bool value) const {
		setEffectFlag(EffectFlag::IllegalDaedraBit, value);
	}

	bool MagicEffect::getFlagUnreflectable() const {
		return getEffectFlag(EffectFlag::UnreflectableBit);
	}

	void MagicEffect::setFlagUnreflectable(bool value) const {
		setEffectFlag(EffectFlag::UnreflectableBit, value);
	}

	bool MagicEffect::getFlagCasterLinked() const {
		return getEffectFlag(EffectFlag::CasterLinkedBit);
	}

	void MagicEffect::setFlagCasterLinked(bool value) const {
		setEffectFlag(EffectFlag::CasterLinkedBit, value);
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

	bool MagicEffect::search(const std::string_view& needle, const BaseObject::SearchSettings& settings, std::regex* regex, int attribute, int skill) const {
		if (!settings.effect) {
			return false;
		}

		auto ndd = DataHandler::get()->recordHandler;

		char buffer[256] = {};

		const auto nameGMST = getNameGMST();
		if (string::complex_contains(ndd->gameSettingsHandler->gameSettings[nameGMST]->value.asString, needle, settings, regex)) {
			return true;
		}

		if (skill >= 0 && getFlagTargetSkill()) {
			const auto skillName = ndd->getGameSettingForSkill(skill)->value.asString;
			switch (nameGMST) {
			case GMST::sEffectFortifySkill:
				sprintf_s(buffer, "%s %s", ndd->gameSettingsHandler->gameSettings[GMST::sFortify]->value.asString, skillName);
				break;
			case GMST::sEffectDrainSkill:
				sprintf_s(buffer, "%s %s", ndd->gameSettingsHandler->gameSettings[GMST::sDrain]->value.asString, skillName);
				break;
			case GMST::sEffectDamageSkill:
				sprintf_s(buffer, "%s %s", ndd->gameSettingsHandler->gameSettings[GMST::sDamage]->value.asString, skillName);
				break;
			case GMST::sEffectRestoreSkill:
				sprintf_s(buffer, "%s %s", ndd->gameSettingsHandler->gameSettings[GMST::sRestore]->value.asString, skillName);
				break;
			case GMST::sEffectAbsorbSkill:
				sprintf_s(buffer, "%s %s", ndd->gameSettingsHandler->gameSettings[GMST::sAbsorb]->value.asString, skillName);
				break;
			}

			if (string::complex_contains(buffer, needle, settings, regex)) {
				return true;
			}
		}
		else if (attribute >= 0 && getFlagTargetAttribute()) {
			const auto attributeName = ndd->getGameSettingForAttribute(attribute)->value.asString;
			switch (nameGMST) {
			case GMST::sEffectFortifyAttribute:
				sprintf_s(buffer, "%s %s", ndd->gameSettingsHandler->gameSettings[GMST::sFortify]->value.asString, attributeName);
				break;
			case GMST::sEffectDrainAttribute:
				sprintf_s(buffer, "%s %s", ndd->gameSettingsHandler->gameSettings[GMST::sDrain]->value.asString, attributeName);
				break;
			case GMST::sEffectDamageAttribute:
				sprintf_s(buffer, "%s %s", ndd->gameSettingsHandler->gameSettings[GMST::sDamage]->value.asString, attributeName);
				break;
			case GMST::sEffectRestoreAttribute:
				sprintf_s(buffer, "%s %s", ndd->gameSettingsHandler->gameSettings[GMST::sRestore]->value.asString, attributeName);
				break;
			case GMST::sEffectAbsorbAttribute:
				sprintf_s(buffer, "%s %s", ndd->gameSettingsHandler->gameSettings[GMST::sAbsorb]->value.asString, attributeName);
				break;
			}

			if (string::complex_contains(buffer, needle, settings, regex)) {
				return true;
			}
		}

		return false;
	}
}
