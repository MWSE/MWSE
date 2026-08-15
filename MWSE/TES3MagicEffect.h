#pragma once

#include "TES3EffectFlags.h"
#include "TES3Object.h"

namespace TES3 {
	struct MagicEffectExtendedData;

	struct MagicEffect : BaseObject {
		int id; // 0x10
		char * description; // 0x14
		int descriptionFileOffset; // 0x18
		char icon[32]; // 0x1C
		char particleTexture[32]; // 0x3C
								  // Only a few effects have data in their sound effect strings.
								  // Maybe it's inferred from school if missing?
		char castSoundEffectID[32]; // 0x5C
		char boltSoundEffectID[32]; // 0x7C
		char hitSoundEffectID[32]; // 0x9C
		char areaSoundEffectID[32]; // 0xBC
		PhysicalObject * castEffect; // 0xDC
		PhysicalObject * boltEffect; // 0xE0
		PhysicalObject * hitEffect; // 0xE4
		PhysicalObject * areaEffect; // 0xE8
		int school; // 0xEC
		float baseMagickaCost; // 0xF0
		unsigned int flags; // 0xF4
		int lightingRed; // 0xF8
		int lightingGreen; // 0xFC
		int lightingBlue; // 0x0100
		float size; // 0x0104
		float speed; // 0x0108
		float sizeCap; // 0x010C

		static constexpr auto OBJECT_TYPE = ObjectType::MagicEffect;

		MagicEffect();
		MagicEffect(int id);
		~MagicEffect();

		//
		// Other related this-call functions.
		//

		void resolveLinks(NonDynamicData * nonDynamicData);
		void clearData();

		//
		// Custom functions
		//

		const char* getName() const;
		std::string getComplexName(int attribute = -1, int skill = -1) const;
		int getNameGMST() const;
		void setDescription(const char* value);
		const char* getDescription() const noexcept;

		const char* getIcon() const;
		void setIcon(const char* path);
		std::string getBigIcon() const;
		const char* getParticleTexture() const;
		void setParticleTexture(const char* path);

		Sound* getCastSoundEffect() const;
		void setCastSoundEffect(Sound* sound);
		Sound* getBoltSoundEffect() const;
		void setBoltSoundEffect(Sound* sound);
		Sound* getHitSoundEffect() const;
		void setHitSoundEffect(Sound* sound);
		Sound* getAreaSoundEffect() const;
		void setAreaSoundEffect(Sound* sound);
		Sound* getSpellFailureSoundEffect() const;

		unsigned int getEffectFlags() const;
		void setEffectFlags(unsigned int flags) const;
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

		int getSkillForSchool() const;

		MagicEffectExtendedData* getExtendedData() const;

		bool getHasActorLighting() const;

	};
	static_assert(sizeof(MagicEffect) == 0x0110, "TES3::EffectID:: failed size validation");

	struct MagicEffectExtendedData {
		std::string name;
		std::string magnitudeType;
		std::string magnitudeTypePlural;
		sol::protected_function tickFunction;
		sol::protected_function collisionFunction;
		bool hasActorLighting;

		MagicEffectExtendedData();

		bool hasName() const;
		bool hasMagnitudeType() const;
		std::string_view getMagnitudeType(bool plural) const;
	};

	struct Effect {
		short effectID; // 0x0
		signed char skillID; // 0x2
		signed char attributeID; // 0x3
		EffectRange rangeType; // 0x4
		int radius; // 0x8
		int duration; // 0xC
		int magnitudeMin; // 0x10
		int magnitudeMax; // 0x14

		Effect();
		Effect(const Effect& from);
		Effect(const sol::table& from);

		Effect& operator=(const Effect& vector);
		Effect& operator=(const sol::table& table);
		Effect& operator=(const sol::object& object);

		bool operator==(const Effect& vector) const;
		bool operator!=(const Effect& vector) const;

		//
		// Other related this-call functions.
		//

		float calculateCost() const;

		//
		// Custom functions
		//

		MagicEffect * getEffectData() const;
		bool matchesEffectsWith(const Effect *) const;

		signed char getSkillID() const;
		void setSkillID(signed char id);
		void setSkillID_lua(sol::optional<signed char> id);

		signed char geAttributeID() const;
		void seAttributeID(signed char id);
		void seAttributeID_lua(sol::optional<signed char> id);

		sol::optional<std::string> toString() const;

	};
	static_assert(sizeof(Effect) == 0x18, "TES3::Effect failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::MagicEffect)
