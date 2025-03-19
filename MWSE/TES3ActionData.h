#pragma once

#include "TES3Defines.h"

#include "TES3Vectors.h"

namespace TES3 {
	enum AttackAnimationState : signed char {
		Idle = 0x0,
		Ready = 0x1,
		SwingUp = 0x2,
		SwingDown = 0x3,
		SwingHit = 0x4,
		SwingFollowLight = 0x5,
		SwingFollowMed = 0x6,
		SwingFollowHeavy = 0x7,
		ReadyingWeap = 0x8,
		UnreadyWeap = 0x9,
		Casting = 0xA,
		CastingFollow = 0xB,
		ReadyingMagic = 0xC,
		UnreadyMagic = 0xD,
		Knockdown = 0xE,
		KnockedOut = 0xF,
		PickingProbing = 0x10,
		Wait = 0x11,
		Dying = 0x12,
		Dead = 0x13,
	};

	enum class PhysicalAttackType : unsigned char {
		None = 0,
		Slash = 1,
		Chop = 2,
		Thrust = 3,
		Projectile = 4,
		Creature1 = 5,
		Creature2 = 6,
		Creature3 = 7
	};

	struct ActionData {
		short fightAttrCombatChange; // 0x0
		short dispositionCombatChange; // 0x2
		float attackSwing; // 0x4
		float swingTimer; // 0x8
		float physicalDamage; // 0xC
		signed char aiBehaviorState; // 0x10
		AttackAnimationState animStateAttack; // 0x11
		unsigned char blockingState; // 0x12
		unsigned char animGroupStunEffect; // 0x13
		PhysicalAttackType physicalAttackType; // 0x14
		unsigned char animSectionCurrentAction; // 0x15
		unsigned char animGroupCurrentAction; // 0x16
		unsigned char animGroupBlocking; // 0x17
		short quantizedHitAngleX; // 0x18
		short quantizedHitAngleY; // 0x1A
		short quantizedHitAngleZ; // 0x1C
		short rescaledFacingBeforeCombat; // 0x1E
		MobileActor * target; // 0x20
		MobileActor * hitTarget; // 0x24
		unsigned short lastBarterHoursPassed; // 0x28
		short padding_0x2A;
		BaseObject * stolenFromFactionOrNPC; // 0x2C
		bool attackWasBlocked; // 0x30
		char padding_0x31[3];
		MobileProjectile * nockedProjectile; // 0x34
		short countAIPackages; // 0x38
		Vector3 unknown_0x3C;
		Vector3 unknown_0x48;
		Vector3 lastPositionBeforeCombat; // 0x54
		Vector3 walkDestination; // 0x60
		float lastWitnessedCrimeTimestamp; // 0x6C

		ActionData() = delete;
		~ActionData() = delete;

	};
	static_assert(sizeof(ActionData) == 0x70, "TES3::ActionData failed size validation");
}
