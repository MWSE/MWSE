#pragma once

#include "TES3MobileActorLua.h"

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForTES3MobileNPC(sol::usertype<T>& usertypeDefinition) {
			setUserdataForTES3MobileActor(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["forceJump"] = &TES3::MobileNPC::flagForceJump;
			usertypeDefinition["forceMoveJump"] = &TES3::MobileNPC::flagForceMoveJump;
			usertypeDefinition["forceRun"] = &TES3::MobileNPC::flagForceRun;
			usertypeDefinition["forceSneak"] = &TES3::MobileNPC::flagForceSneak;
			usertypeDefinition["object"] = sol::readonly_property(&TES3::MobileNPC::npcInstance);

			// Indirect bindings to unions and arrays.
			usertypeDefinition["skills"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatistics);

			// Access to other objects that need to be packaged.

			// Allow read access to movement speeds.
			usertypeDefinition["moveSpeed"] = sol::readonly_property(&TES3::MobileNPC::calculateMovementSpeedFromAnimationData);
			usertypeDefinition["walkSpeed"] = sol::readonly_property(&TES3::MobileNPC::calculateWalkSpeed);
			usertypeDefinition["runSpeed"] = sol::readonly_property(&TES3::MobileNPC::calculateRunSpeed);
			usertypeDefinition["swimSpeed"] = sol::readonly_property(&TES3::MobileNPC::calculateSwimSpeed);
			usertypeDefinition["swimRunSpeed"] = sol::readonly_property(&TES3::MobileNPC::calculateSwimRunSpeed);
			usertypeDefinition["flySpeed"] = sol::readonly_property(&TES3::MobileNPC::calculateFlySpeed);

			// Friendly access to skills.
			usertypeDefinition["acrobatics"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticAcrobatics);
			usertypeDefinition["alchemy"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticAlchemy);
			usertypeDefinition["alteration"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticAlteration);
			usertypeDefinition["armorer"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticArmorer);
			usertypeDefinition["athletics"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticAthletics);
			usertypeDefinition["axe"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticAxe);
			usertypeDefinition["block"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticBlock);
			usertypeDefinition["bluntWeapon"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticBluntWeapon);
			usertypeDefinition["conjuration"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticConjuration);
			usertypeDefinition["destruction"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticDestruction);
			usertypeDefinition["enchant"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticEnchant);
			usertypeDefinition["handToHand"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticHandToHand);
			usertypeDefinition["heavyArmor"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticHeavyArmor);
			usertypeDefinition["illusion"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticIllusion);
			usertypeDefinition["lightArmor"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticLightArmor);
			usertypeDefinition["longBlade"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticLongBlade);
			usertypeDefinition["marksman"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticMarksman);
			usertypeDefinition["mediumArmor"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticMediumArmor);
			usertypeDefinition["mercantile"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticMercantile);
			usertypeDefinition["mysticism"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticMysticism);
			usertypeDefinition["restoration"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticRestoration);
			usertypeDefinition["security"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticSecurity);
			usertypeDefinition["shortBlade"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticShortBlade);
			usertypeDefinition["sneak"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticSneak);
			usertypeDefinition["spear"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticSpear);
			usertypeDefinition["speechcraft"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticSpeechcraft);
			usertypeDefinition["unarmored"] = sol::readonly_property(&TES3::MobileNPC::getSkillStatisticUnarmored);
		}

		void bindTES3MobileNPC();
	}
}
