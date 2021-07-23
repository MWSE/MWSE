#pragma once

#include "TES3Defines.h"

#include "TES3MobileActor.h"
#include "TES3Statistic.h"

namespace TES3 {
	struct MobileNPC : MobileActor {
		SkillStatistic skills[27]; // 0x3B0
		NPCInstance * npcInstance; // 0x560
		char unknown_0x564;
		bool flagForceSneak; // 0x565
		bool flagForceRun; // 0x566
		bool flagForceJump; // 0x567
		bool flagForceMoveJump; // 0x568
		char unknown_0x569; // Undefined.
		char unknown_0x56A; // Undefined.
		char unknown_0x56B; // Undefined.

		//
		// vTable accessor functions.
		//

		float applyArmorRating(float damage, float swing, bool damageEquipment);

		//
		// Other related this-call functions.
		//

		float calculateWalkSpeed();
		int getDisposition();

		//
		// Custom functions.
		//

		float getSkillRequirement(int skillId);

		float calculateMovementSpeedFromAnimationData();

		std::reference_wrapper<SkillStatistic[27]> getSkillStatistics();

		SkillStatistic* getSkillStatisticAcrobatics();
		SkillStatistic* getSkillStatisticAlchemy();
		SkillStatistic* getSkillStatisticAlteration();
		SkillStatistic* getSkillStatisticArmorer();
		SkillStatistic* getSkillStatisticAthletics();
		SkillStatistic* getSkillStatisticAxe();
		SkillStatistic* getSkillStatisticBlock();
		SkillStatistic* getSkillStatisticBluntWeapon();
		SkillStatistic* getSkillStatisticConjuration();
		SkillStatistic* getSkillStatisticDestruction();
		SkillStatistic* getSkillStatisticEnchant();
		SkillStatistic* getSkillStatisticHandToHand();
		SkillStatistic* getSkillStatisticHeavyArmor();
		SkillStatistic* getSkillStatisticIllusion();
		SkillStatistic* getSkillStatisticLightArmor();
		SkillStatistic* getSkillStatisticLongBlade();
		SkillStatistic* getSkillStatisticMarksman();
		SkillStatistic* getSkillStatisticMediumArmor();
		SkillStatistic* getSkillStatisticMercantile();
		SkillStatistic* getSkillStatisticMysticism();
		SkillStatistic* getSkillStatisticRestoration();
		SkillStatistic* getSkillStatisticSecurity();
		SkillStatistic* getSkillStatisticShortBlade();
		SkillStatistic* getSkillStatisticSneak();
		SkillStatistic* getSkillStatisticSpear();
		SkillStatistic* getSkillStatisticSpeechcraft();
		SkillStatistic* getSkillStatisticUnarmored();

	};
	static_assert(sizeof(MobileNPC) == 0x56C, "TES3::MobileNPC failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3_MOBILEOBJECT(TES3::MobileNPC)
