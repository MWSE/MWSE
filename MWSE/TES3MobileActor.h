#pragma once

#include "TES3Defines.h"

#include "TES3ActionData.h"
#include "TES3Armor.h"
#include "TES3Clothing.h"
#include "TES3Inventory.h"
#include "TES3MagicSourceInstance.h"
#include "TES3MobileObject.h"
#include "TES3Statistic.h"
#include "TES3Vectors.h"

namespace TES3 {
	enum class MobileActorType : char {
		Creature = 0,
		NPC = 1,
		Player = 2
	};

	namespace Attribute {
		enum Attribute {
			Invalid = -1,
			Strength = 0,
			Intelligence = 1,
			Willpower = 2,
			Agility = 3,
			Speed = 4,
			Endurance = 5,
			Personality = 6,
			Luck = 7,
			FirstAttribute = Strength,
			LastAttribute = Luck
		};
	}

	namespace EffectAttribute {
		enum EffectAttribute {
			AttackBonus,
			Sanctuary,
			ResistMagicka,
			ResistFire,
			ResistFrost,
			ResistShock,
			ResistCommonDisease,
			ResistBlightDisease,
			ResistCorprus,
			ResistPoison,
			ResistParalysis,
			Chameleon,
			ResistNormalWeapons,
			WaterBreathing,
			WaterWalking,
			SwiftSwim,
			Jump,
			Levitate,
			Shield,
			Sound,
			Silence,
			Blind,
			Paralyze,
			Invisibility
		};
	}

	struct MobileActor : MobileObject {
		struct ActiveMagicEffect {
			ActiveMagicEffect* next;
			ActiveMagicEffect* prev;
			unsigned int magicInstanceSerial;
			short magicInstanceEffectIndex;
			short magicEffectID;
			bool isHarmful;
			unsigned short duration;
			unsigned short magnitudeMin;
			unsigned char skillOrAttributeID;
		};

		void * unknown_0x60;
		float thisFrameDistanceMoved;
		Vector3 thisFrameDeltaPosition;
		float unknown_0x74;
		int unknown_0x78;
		char unknown_0x7C;
		Iterator<MobileActor> listTargetActors; // 0x80
		Iterator<MobileActor> listFriendlyActors; // 0x94
		float scanTimer; // 0xA8
		int scanInterval; // 0xAC
		int greetTimer; // B0
		Vector3 unknown_0xB4;
		char unknown_0xC0;
		char unknown_0xC1; // Undefined.
		char unknown_0xC2; // Undefined.
		char unknown_0xC3; // Undefined.
		float unknown_0xC4;
		void * aiPackageData; // 0xC8
		ActionData actionData; // 0xCC
		ActionData actionBeforeCombat; // 0x13C
		int unknown_0x1AC;
		CrimeTree * unknown_0x1B0;
		int unknown_0x1B4;
		int unknown_0x1B8;
		int unknown_0x1BC;
		CombatSession * combatSession; // 0x1C0
		char unknown_0x1C4;
		char unknown_0x1C5;
		char unknown_0x1C6;
		char unknown_0x1C7;
		ActiveMagicEffect* activeMagicEffects; // 0x1C8
		int activeMagicEffectCount; // 0x1CC
		int unknown_0x1D0;
		char unknown_0x1D4;
		char unknown_0x1D5; // Undefined.
		char unknown_0x1D6; // Undefined.
		char unknown_0x1D7; // Undefined.
		float unknown_0x1D8;
		Vector3 unknown_0x1DC;
		Vector3 unknown_0x1E8;
		Vector3 unknown_0x1F4;
		int unknown_0x200;
		int unknown_0x204;
		int unknown_0x208;
		short unknown_0x20C;
		short unknown_0x20E;
		short unknown_0x210;
		char unknown_0x212;
		char unknown_0x213;
		HashMap powers;
		char unknown_0x224;
		char unknown_0x225;
		char unknown_0x226;
		signed char nextActionWeight; // 0x227
		MobileActorType actorType; // 0x228
		char unknown_0x229;
		char unknown_0x22A; // Undefined.
		char unknown_0x22B; // Undefined.
		int lastGroundZ; // 0x22C
		int unknown_0x230;
		Reference * collidingReference; // 0x234
		int unknown_0x238;
		int unknown_0x23C;
		int unknown_0x240;
		union {
			ActorAnimationData * asActor;
			PlayerAnimationData * asPlayer;
		} animationData; // 0x244
		char unknown_0x248;
		char unknown_0x249; // Undefined.
		char unknown_0x24A; // Undefined.
		char unknown_0x24B; // Undefined.
		void * unknown_0x24C;
		int unknown_0x250;
		Statistic attributes[8];
		Statistic health;
		Statistic magicka;
		Statistic encumbrance;
		Statistic fatigue;
		Statistic magickaMultiplier;
		int effectAttributes[24];
		int fight;
		int flee;
		int hello;
		int alarm;
		int barterGold;
		short widthInUnits;
		short unknown_0x366;
		short readiedAmmoCount;
		short corpseHourstamp;
		short greetDuration;
		char unknown_0x36E;
		char unknown_0x36F; // Undefined.
		float holdBreathTime;
		int unknown_0x374;
		MagicSourceCombo currentSpell;
		EquipmentStack currentEnchantedItem; // Not a pointer.
		EquipmentStack * readiedWeapon;
		EquipmentStack * readiedAmmo;
		EquipmentStack * readiedShield;
		EquipmentStack * torchSlot;
		char unknown_0x398;
		char unknown_0x399; // Undefined.
		char unknown_0x39A; // Undefined.
		char unknown_0x39B; // Undefined.
		void * arrowBone;
		int unknown_0x3A0;
		Vector3 unknown_0x3A4;

		//
		// vTable accessor functions.
		//

		signed char onActorCollision(int hitReferenceIndex);
		signed char onObjectCollision(int hitReferenceIndex, signed char flag);
		signed char onTerrainCollision(int hitReferenceIndex);
		signed char onActivatorCollision(int hitReferenceIndex);

		SkillStatistic * getSkillStatistic(int skillId);
		float getSkillValue(int skillId);

		//
		// Other related this-call functions.
		//

		Cell* getCell();

		void startCombat(MobileActor*);
		void stopCombat(bool);
		void onDeath();
		bool applyHealthDamage(float, bool, bool, bool);
		bool hasFreeAction();
		float calculateRunSpeed();
		float calculateSwimSpeed();
		float calculateSwimRunSpeed();
		float calculateFlySpeed();

		void updateDerivedStatistics(Statistic * baseStatistic);

		//
		// Custom functions.
		//

		bool getMobileActorFlag(MobileActorFlag::Flag);
		void setMobileActorFlag(MobileActorFlag::Flag, bool);

		bool equipItem(Object* item);
	};
	static_assert(sizeof(MobileActor::ActiveMagicEffect) == 0x18, "TES3::MobileActor::ActiveMagicEffect failed size validation");
	static_assert(sizeof(MobileActor) == 0x3B0, "TES3::MobileActor failed size validation");
}
