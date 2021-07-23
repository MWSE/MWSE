#pragma once

#include "TES3Object.h"

namespace TES3 {
	namespace EffectID {
		enum EffectID {
			None = -1,
			WaterBreathing = 0,
			SwiftSwim = 1,
			WaterWalking = 2,
			Shield = 3,
			FireShield = 4,
			LightningShield = 5,
			FrostShield = 6,
			Burden = 7,
			Feather = 8,
			Jump = 9,
			Levitate = 10,
			SlowFall = 11,
			Lock = 12,
			Open = 13,
			FireDamage = 14,
			ShockDamage = 15,
			FrostDamage = 16,
			DrainAttribute = 17,
			DrainHealth = 18,
			DrainMagicka = 19,
			DrainFatigue = 20,
			DrainSkill = 21,
			DamageAttribute = 22,
			DamageHealth = 23,
			DamageMagicka = 24,
			DamageFatigue = 25,
			DamageSkill = 26,
			Poison = 27,
			WeaknessToFire = 28,
			WeaknessToFrost = 29,
			WeaknessToShock = 30,
			WeaknessToMagicka = 31,
			WeaknessToCommonDisease = 32,
			WeaknessToBlightDisease = 33,
			WeaknessToCorprus = 34,
			WeaknessToPoison = 35,
			WeaknessToNormalWeapons = 36,
			DisintegrateWeapon = 37,
			DisintegrateArmor = 38,
			Invisibility = 39,
			Chameleon = 40,
			Light = 41,
			Sanctuary = 42,
			NightEye = 43,
			Charm = 44,
			Paralyze = 45,
			Silence = 46,
			Blind = 47,
			Sound = 48,
			CalmHumanoid = 49,
			CalmCreature = 50,
			FrenzyHumanoid = 51,
			FrenzyCreature = 52,
			DemoralizeHumanoid = 53,
			DemoralizeCreature = 54,
			RallyHumanoid = 55,
			RallyCreature = 56,
			Dispel = 57,
			SoulTrap = 58,
			Telekinesis = 59,
			Mark = 60,
			Recall = 61,
			DivineIntervention = 62,
			AlmsiviIntervention = 63,
			DetectAnimal = 64,
			DetectEnchantment = 65,
			DetectKey = 66,
			SpellAbsorption = 67,
			Reflect = 68,
			CureCommonDisease = 69,
			CureBlightDisease = 70,
			CureCorprus = 71,
			CurePoison = 72,
			CureParalyzation = 73,
			RestoreAttribute = 74,
			RestoreHealth = 75,
			RestoreMagicka = 76,
			RestoreFatigue = 77,
			RestoreSkill = 78,
			FortifyAttribute = 79,
			FortifyHealth = 80,
			FortifyMagicka = 81,
			FortifyFatigue = 82,
			FortifySkill = 83,
			FortifyMagickaMultiplier = 84,
			AbsorbAttribute = 85,
			AbsorbHealth = 86,
			AbsorbMagicka = 87,
			AbsorbFatigue = 88,
			AbsorbSkill = 89,
			ResistFire = 90,
			ResistFrost = 91,
			ResistShock = 92,
			ResistMagicka = 93,
			ResistCommonDisease = 94,
			ResistBlightDisease = 95,
			ResistCorprus = 96,
			ResistPoison = 97,
			ResistNormalWeapons = 98,
			ResistParalysis = 99,
			RemoveCurse = 100,
			TurnUndead = 101,
			SummonScamp = 102,
			SummonClannfear = 103,
			SummonDaedroth = 104,
			SummonDremora = 105,
			SummonGhost = 106,
			SummonSkeleton = 107,
			SummonLeastBonewalker = 108,
			SummonGreaterBonewalker = 109,
			SummonBonelord = 110,
			SummonTwilight = 111,
			SummonHunger = 112,
			SummonGoldenSaint = 113,
			SummonFlameAtronach = 114,
			SummonFrostAtronach = 115,
			SummonStormAtronach = 116,
			FortifyAttackBonus = 117,
			CommandCreature = 118,
			CommandHumanoid = 119,
			BoundDagger = 120,
			BoundLongsword = 121,
			BoundMace = 122,
			BoundBattleAxe = 123,
			BoundSpear = 124,
			BoundLongbow = 125,
			ExtraSpell = 126,
			BoundCuirass = 127,
			BoundHelm = 128,
			BoundBoots = 129,
			BoundShield = 130,
			BoundGloves = 131,
			Corprus = 132,
			Vampirism = 133,
			SummonCenturionSphere = 134,
			SunDamage = 135,
			StuntedMagicka = 136,
			SummonFabricant = 137,
			SummonWolf = 138,
			SummonBear = 139,
			SummonBoneWolf = 140,
			Summon04 = 141,
			Summon05 = 142,
			FirstEffect = WaterBreathing,
			LastEffect = Summon05
		};
	}

	namespace EffectFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			TargetSkill = 0x1,
			TargetAttribute = 0x2,
			NoDuration = 0x4,
			NoMagnitude = 0x8,
			Harmful = 0x10,
			ContinuousVFX = 0x20,
			CanCastSelf = 0x40,
			CanCastTouch = 0x80,
			CanCastTarget = 0x100,
			AllowSpellmaking = 0x200,
			AllowEnchanting = 0x400,
			NegativeLighting = 0x800,
			AppliedOnce = 0x1000,
			UnknownChameleon = 0x2000,
			NonRecastable = 0x4000,
			IllegalDaedra = 0x8000,
			Unreflectable = 0x10000,
			CasterLinked = 0x20000
		};

		enum FlagBit {
			TargetSkillBit = 0,
			TargetAttributeBit = 1,
			NoDurationBit = 2,
			NoMagnitudeBit = 3,
			HarmfulBit = 4,
			ContinuousVFXBit = 5,
			CanCastSelfBit = 6,
			CanCastTouchBit = 7,
			CanCastTargetBit = 8,
			AllowSpellmakingBit = 9,
			AllowEnchantingBit = 10,
			NegativeLightingBit = 11,
			AppliedOnceBit = 12,
			UnknownChameleonBit = 13,
			NonRecastableBit = 14,
			IllegalDaedraBit = 15,
			UnreflectableBit = 16,
			CasterLinkedBit = 17
		};
	}

	enum class EffectRange : unsigned char {
		Self,
		Touch,
		Target
	};

	namespace MagicSchool {
		enum MagicSchool {
			Alteration,
			Conjuration,
			Destruction,
			Illusion,
			Mysticism,
			Restoration,
			None,
			FirstMagicSchool = Alteration,
			LastMagicSchool = Restoration
		};
	}

	struct MagicEffect : BaseObject {
		int id; // 0x10
		char * description; // 0x14
		int descriptionFileOffset; // 0x18
		char icon[32]; // 0x1C
		char particleTexture[32]; // 0x3C
								  // Only a few effects have data in their sound effect strings.
								  // Maybe it's inferred from school if missing?
		char castSoundEffect[32]; // 0x5C
		char boltSoundEffect[32]; // 0x7C
		char hitSoundEffect[32]; // 0x9C
		char areaSoundEffect[32]; // 0xBC
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
		int getNameGMST() const;
		void setDescription( const char *value );
		const char* getDescription() const noexcept;

		const char* getIcon() const;
		void setIcon(const char* path);

		const char* getParticleTexture() const;
		void setParticleTexture(const char* path);
		const char* getCastSoundEffect() const;
		void setCastSoundEffect(const char* path);
		const char* getBoltSoundEffect() const;
		void setBoltSoundEffect(const char* path);
		const char* getHitSoundEffect() const;
		void setHitSoundEffect(const char* path);
		const char* getAreaSoundEffect() const;
		void setAreaSoundEffect(const char* path);

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

	};
	static_assert(sizeof(MagicEffect) == 0x0110, "TES3::EffectID:: failed size validation");

	struct Effect {
		short effectID; // 0x0
		signed char skillID; // 0x2
		signed char attributeID; // 0x3
		EffectRange rangeType; // 0x4
		int radius; // 0x8
		int duration; // 0xC
		int magnitudeMin; // 0x10
		int magnitudeMax; // 0x14

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
