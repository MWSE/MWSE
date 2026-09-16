#pragma once

#include "CSObject.h"

#include "TES3EffectFlags.h"

namespace se::cs {
	struct MagicEffect : BaseObject {
		int id; // 0x10
		char* description; // 0x14
		char icon[32]; // 0x18
		char particleTexture[32]; // 0x38
		// Only a few effects have data in their sound effect strings.
		// Maybe it's inferred from school if missing?
		char castSoundEffectID[32]; // 0x58
		char boltSoundEffectID[32]; // 0x78
		char hitSoundEffectID[32]; // 0x98
		char areaSoundEffectID[32]; // 0xB8
		PhysicalObject* castEffect; // 0xD8
		PhysicalObject* boltEffect; // 0xDC
		PhysicalObject* hitEffect; // 0xE0
		PhysicalObject* areaEffect; // 0xE4
		int school; // 0xE8
		float baseMagickaCost; // 0xEC
		unsigned int flags; // 0xF0
		int lightingRed; // 0xF4
		int lightingGreen; // 0xF8
		int lightingBlue; // 0xFC
		float size; // 0x100
		float speed; // 0x104
		float sizeCap; // 0x108

		//
		// Custom functions
		//

		const char* getName() const;
		std::string getComplexName(int attribute = -1, int skill = -1) const;
		int getNameGMST() const;

		unsigned int getEffectFlags() const;
		void setEffectFlags(unsigned int flags) const;
		bool getEffectFlag(unsigned int flag) const;
		void setEffectFlag(unsigned int flag, bool value) const;
		bool getFlagTargetSkill() const;
		void setFlagTargetSkill(bool value) const;
		bool getFlagTargetAttribute() const;
		void setFlagTargetAttribute(bool value) const;
		bool getFlagNoDuration() const;
		void setFlagNoDuration(bool value) const;
		bool getFlagNoMagnitude() const;
		void setFlagNoMagnitude(bool value) const;
		bool getFlagHarmful() const;
		void setFlagHarmful(bool value) const;
		bool getFlagContinuousVFX() const;
		void setFlagContinuousVFX(bool value) const;
		bool getFlagCanCastSelf() const;
		void setFlagCanCastSelf(bool value) const;
		bool getFlagCanCastTouch() const;
		void setFlagCanCastTouch(bool value) const;
		bool getFlagCanCastTarget() const;
		void setFlagCanCastTarget(bool value) const;
		bool getFlagNegativeLighting() const;
		void setFlagNegativeLighting(bool value) const;
		bool getFlagAppliedOnce() const;
		void setFlagAppliedOnce(bool value) const;
		bool getFlagNonRecastable() const;
		void setFlagNonRecastable(bool value) const;
		bool getFlagIllegalDaedra() const;
		void setFlagIllegalDaedra(bool value) const;
		bool getFlagUnreflectable() const;
		void setFlagUnreflectable(bool value) const;
		bool getFlagCasterLinked() const;
		void setFlagCasterLinked(bool value) const;

		bool getAllowSpellmaking() const;
		void setAllowSpellmaking(bool value);
		bool getAllowEnchanting() const;
		void setAllowEnchanting(bool value);

		bool search(std::string_view needle, const BaseObject::SearchSettings& settings, std::regex* regex = nullptr, int attribute = -1, int skill = -1) const;
	};
	static_assert(sizeof(MagicEffect) == 0x10C, "TES3::MagicEffect failed size validation");
}
