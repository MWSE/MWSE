#include "TES3Race.h"

#include "BitUtil.h"

namespace TES3 {
	char * Race::getObjectID() {
		return id;
	}

	const auto TES3_Race_getBodyPart = reinterpret_cast<BodyPart * (__thiscall*)(const Race*, int, int, Race::PartIndex)>(0x4A7790);
	BodyPart* Race::getBodyPart(bool isFemale, bool isVampire, PartIndex index) const {
		return TES3_Race_getBodyPart(this, isFemale, isVampire, index);
	}

	const auto TES3_Race_loadDescription = reinterpret_cast<char* (__thiscall*)(Race*)>(0x4A7A90);
	char* Race::loadDescription() {
		return TES3_Race_loadDescription(this);
	}

	const auto TES3_Race_freeDescription = reinterpret_cast<void(__thiscall*)(Race*)>(0x4A7C90);
	void Race::freeDescription() {
		return TES3_Race_freeDescription(this);
	}

	const char* Race::getName() const {
		return name;
	}

	bool Race::getIsPlayable() const {
		return BIT_TEST(flags, PlayableBit);
	}

	void Race::setIsPlayable(bool value) {
		BIT_SET(flags, PlayableBit, value);
	}

	bool Race::getIsBeast() const {
		return BIT_TEST(flags, BeastBit);
	}

	void Race::setIsBeast(bool value) {
		BIT_SET(flags, BeastBit, value);
	}

	sol::optional<std::string> Race::getAndLoadDescription() {
		// If the description is already loaded, just return it.
		if (description) {
			return description;
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

	std::reference_wrapper<Race::BaseAttribute[8]> Race::getBaseAttributes() {
		return std::ref(baseAttributes);
	}

	std::reference_wrapper<Race::SkillBonus[7]> Race::getSkillBonuses() {
		return std::ref(skillBonuses);
	}

}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Race)
