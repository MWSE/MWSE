#pragma once

#include "TES3MobileObjectLua.h"

#include "TES3ActorAnimationController.h"
#include "TES3AIData.h"
#include "TES3Cell.h"
#include "TES3CombatSession.h"

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForTES3MobileActor(sol::usertype<T>& usertypeDefinition) {
			setUserdataForTES3MobileObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["actionBeforeCombat"] = sol::readonly_property(&TES3::MobileActor::actionBeforeCombat);
			usertypeDefinition["actionData"] = sol::readonly_property(&TES3::MobileActor::actionData);
			usertypeDefinition["actorType"] = sol::readonly_property(&TES3::MobileActor::actorType);
			usertypeDefinition["aiPlanner"] = sol::readonly_property(&TES3::MobileActor::aiPlanner);
			usertypeDefinition["alarm"] = &TES3::MobileActor::alarm;
			usertypeDefinition["barterGold"] = &TES3::MobileActor::barterGold;
			usertypeDefinition["collidingReference"] = sol::readonly_property(&TES3::MobileActor::collidingReference);
			usertypeDefinition["combatSession"] = sol::readonly_property(&TES3::MobileActor::combatSession);
			usertypeDefinition["corpseHourstamp"] = &TES3::MobileActor::corpseHourstamp;
			usertypeDefinition["currentEnchantedItem"] = sol::readonly_property(&TES3::MobileActor::currentEnchantedItem);
			usertypeDefinition["encumbrance"] = sol::readonly_property(&TES3::MobileActor::encumbrance);
			usertypeDefinition["facing"] = sol::readonly_property(&TES3::MobileActor::getFacing);
			usertypeDefinition["fatigue"] = sol::readonly_property(&TES3::MobileActor::fatigue);
			usertypeDefinition["fight"] = &TES3::MobileActor::fight;
			usertypeDefinition["flee"] = &TES3::MobileActor::flee;
			usertypeDefinition["friendlyActors"] = sol::readonly_property(&TES3::MobileActor::listFriendlyActors);
			usertypeDefinition["greetDuration"] = &TES3::MobileActor::greetDuration;
			usertypeDefinition["greetTimer"] = sol::readonly_property(&TES3::MobileActor::greetTimer);
			usertypeDefinition["health"] = sol::readonly_property(&TES3::MobileActor::health);
			usertypeDefinition["hello"] = &TES3::MobileActor::hello;
			usertypeDefinition["holdBreathTime"] = &TES3::MobileActor::holdBreathTime;
			usertypeDefinition["hostileActors"] = sol::readonly_property(&TES3::MobileActor::listTargetActors);
			usertypeDefinition["isDead"] = sol::readonly_property(&TES3::MobileActor::isDead);
			usertypeDefinition["lastGroundZ"] = &TES3::MobileActor::lastGroundZ;
			usertypeDefinition["magicka"] = sol::readonly_property(&TES3::MobileActor::magicka);
			usertypeDefinition["magickaMultiplier"] = sol::readonly_property(&TES3::MobileActor::magickaMultiplier);
			usertypeDefinition["nextActionWeight"] = sol::readonly_property(&TES3::MobileActor::nextActionWeight);
			//usertypeDefinition["powers"] = sol::readonly_property(&TES3::MobileActor::powers);
			usertypeDefinition["readiedAmmo"] = &TES3::MobileActor::readiedAmmo;
			usertypeDefinition["readiedAmmoCount"] = &TES3::MobileActor::readiedAmmoCount;
			usertypeDefinition["readiedShield"] = &TES3::MobileActor::readiedShield;
			usertypeDefinition["readiedWeapon"] = &TES3::MobileActor::readiedWeapon;
			usertypeDefinition["scanInterval"] = sol::readonly_property(&TES3::MobileActor::scanInterval);
			usertypeDefinition["scanTimer"] = sol::readonly_property(&TES3::MobileActor::scanTimer);
			usertypeDefinition["torchSlot"] = &TES3::MobileActor::torchSlot;
			usertypeDefinition["width"] = sol::readonly_property(&TES3::MobileActor::widthInUnits);

			// Indirect bindings to unions and arrays.
			usertypeDefinition["activeMagicEffectList"] = sol::readonly_property(&TES3::MobileActor::getActiveMagicEffectsList_lua);
			usertypeDefinition["animationController"] = sol::readonly_property(&TES3::MobileActor::getAnimationController);
			usertypeDefinition["attributes"] = sol::readonly_property(&TES3::MobileActor::getAttributes);
			usertypeDefinition["currentSpell"] = sol::readonly_property(&TES3::MobileActor::getCurrentSpell);
			usertypeDefinition["effectAttributes"] = sol::readonly_property(&TES3::MobileActor::getEffectAttributes);

			// Friendly access to actor flags.
			usertypeDefinition["activeAI"] = sol::property(&TES3::MobileActor::getFlagActiveAI, &TES3::MobileActor::setFlagActiveAI);
			usertypeDefinition["attacked"] = sol::property(&TES3::MobileActor::getFlagAttacked, &TES3::MobileActor::setFlagAttacked);
			usertypeDefinition["idleAnim"] = sol::property(&TES3::MobileActor::getFlagIdleAnim, &TES3::MobileActor::setFlagIdleAnim);
			usertypeDefinition["inCombat"] = sol::property(&TES3::MobileActor::getFlagInCombat, &TES3::MobileActor::setFlagInCombat);
			usertypeDefinition["isCrittable"] = sol::property(&TES3::MobileActor::getFlagIsCrittable, &TES3::MobileActor::setFlagIsCrittable);
			usertypeDefinition["isPlayerDetected"] = sol::property(&TES3::MobileActor::getFlagIsPCDetected, &TES3::MobileActor::setFlagIsPCDetected);
			usertypeDefinition["isPlayerHidden"] = sol::property(&TES3::MobileActor::getFlagIsPCHidden, &TES3::MobileActor::setFlagIsPCHidden);
			usertypeDefinition["spellReadied"] = sol::property(&TES3::MobileActor::getFlagSpellReadied, &TES3::MobileActor::setFlagSpellReadied);
			usertypeDefinition["talkedTo"] = sol::property(&TES3::MobileActor::getFlagTalkedTo, &TES3::MobileActor::setFlagTalkedTo);
			usertypeDefinition["underwater"] = sol::property(&TES3::MobileActor::getFlagUnderwater, &TES3::MobileActor::setFlagUnderwater);
			usertypeDefinition["weaponDrawn"] = sol::property(&TES3::MobileActor::getFlagWeaponDrawn, &TES3::MobileActor::setFlagWeaponDrawn);
			usertypeDefinition["werewolf"] = sol::property(&TES3::MobileActor::getFlagWerewolf, &TES3::MobileActor::setFlagWerewolf);

			// Provide some friendly exposure to attributes by name.
			usertypeDefinition["agility"] = sol::readonly_property(&TES3::MobileActor::getAttributeAgility);
			usertypeDefinition["endurance"] = sol::readonly_property(&TES3::MobileActor::getAttributeEndurance);
			usertypeDefinition["intelligence"] = sol::readonly_property(&TES3::MobileActor::getAttributeIntelligence);
			usertypeDefinition["luck"] = sol::readonly_property(&TES3::MobileActor::getAttributeLuck);
			usertypeDefinition["personality"] = sol::readonly_property(&TES3::MobileActor::getAttributePersonality);
			usertypeDefinition["speed"] = sol::readonly_property(&TES3::MobileActor::getAttributeSpeed);
			usertypeDefinition["strength"] = sol::readonly_property(&TES3::MobileActor::getAttributeStrength);
			usertypeDefinition["willpower"] = sol::readonly_property(&TES3::MobileActor::getAttributeWillpower);

			// Provide some friendly exposure to effect attributes.
			usertypeDefinition["attackBonus"] = sol::property(&TES3::MobileActor::getEffectAttributeAttackBonus, &TES3::MobileActor::setEffectAttributeAttackBonus);
			usertypeDefinition["blind"] = sol::property(&TES3::MobileActor::getEffectAttributeBlind, &TES3::MobileActor::setEffectAttributeBlind);
			usertypeDefinition["chameleon"] = sol::property(&TES3::MobileActor::getEffectAttributeChameleon, &TES3::MobileActor::setEffectAttributeChameleon);
			usertypeDefinition["invisibility"] = sol::property(&TES3::MobileActor::getEffectAttributeInvisibility, &TES3::MobileActor::setEffectAttributeInvisibility);
			usertypeDefinition["jump"] = sol::property(&TES3::MobileActor::getEffectAttributeJump, &TES3::MobileActor::setEffectAttributeJump);
			usertypeDefinition["levitate"] = sol::property(&TES3::MobileActor::getEffectAttributeLevitate, &TES3::MobileActor::setEffectAttributeLevitate);
			usertypeDefinition["paralyze"] = sol::property(&TES3::MobileActor::getEffectAttributeParalyze, &TES3::MobileActor::setEffectAttributeParalyze);
			usertypeDefinition["resistBlightDisease"] = sol::property(&TES3::MobileActor::getEffectAttributeResistBlightDisease, &TES3::MobileActor::setEffectAttributeResistBlightDisease);
			usertypeDefinition["resistCommonDisease"] = sol::property(&TES3::MobileActor::getEffectAttributeResistCommonDisease, &TES3::MobileActor::setEffectAttributeResistCommonDisease);
			usertypeDefinition["resistCorprus"] = sol::property(&TES3::MobileActor::getEffectAttributeResistCorprus, &TES3::MobileActor::setEffectAttributeResistCorprus);
			usertypeDefinition["resistFire"] = sol::property(&TES3::MobileActor::getEffectAttributeResistFire, &TES3::MobileActor::setEffectAttributeResistFire);
			usertypeDefinition["resistFrost"] = sol::property(&TES3::MobileActor::getEffectAttributeResistFrost, &TES3::MobileActor::setEffectAttributeResistFrost);
			usertypeDefinition["resistMagicka"] = sol::property(&TES3::MobileActor::getEffectAttributeResistMagicka, &TES3::MobileActor::setEffectAttributeResistMagicka);
			usertypeDefinition["resistNormalWeapons"] = sol::property(&TES3::MobileActor::getEffectAttributeResistNormalWeapons, &TES3::MobileActor::setEffectAttributeResistNormalWeapons);
			usertypeDefinition["resistParalysis"] = sol::property(&TES3::MobileActor::getEffectAttributeResistParalysis, &TES3::MobileActor::setEffectAttributeResistParalysis);
			usertypeDefinition["resistPoison"] = sol::property(&TES3::MobileActor::getEffectAttributeResistPoison, &TES3::MobileActor::setEffectAttributeResistPoison);
			usertypeDefinition["resistShock"] = sol::property(&TES3::MobileActor::getEffectAttributeResistShock, &TES3::MobileActor::setEffectAttributeResistShock);
			usertypeDefinition["sanctuary"] = sol::property(&TES3::MobileActor::getEffectAttributeSanctuary, &TES3::MobileActor::setEffectAttributeSanctuary);
			usertypeDefinition["shield"] = sol::property(&TES3::MobileActor::getEffectAttributeShield, &TES3::MobileActor::setEffectAttributeShield);
			usertypeDefinition["silence"] = sol::property(&TES3::MobileActor::getEffectAttributeSilence, &TES3::MobileActor::setEffectAttributeSilence);
			usertypeDefinition["sound"] = sol::property(&TES3::MobileActor::getEffectAttributeSound, &TES3::MobileActor::setEffectAttributeSound);
			usertypeDefinition["swiftSwim"] = sol::property(&TES3::MobileActor::getEffectAttributeSwiftSwim, &TES3::MobileActor::setEffectAttributeSwiftSwim);
			usertypeDefinition["waterBreathing"] = sol::property(&TES3::MobileActor::getEffectAttributeWaterBreathing, &TES3::MobileActor::setEffectAttributeWaterBreathing);
			usertypeDefinition["waterWalking"] = sol::property(&TES3::MobileActor::getEffectAttributeWaterWalking, &TES3::MobileActor::setEffectAttributeWaterWalking);

			// Provide some friendly exposure for movement flags.
			usertypeDefinition["isFlying"] = sol::property(&TES3::MobileActor::getMovementFlagFlying, &TES3::MobileActor::setMovementFlagFlying);
			usertypeDefinition["isJumping"] = sol::property(&TES3::MobileActor::getMovementFlagJumping, &TES3::MobileActor::setMovementFlagJumping);
			usertypeDefinition["isMovingBack"] = sol::property(&TES3::MobileActor::getMovementFlagBack, &TES3::MobileActor::setMovementFlagBack);
			usertypeDefinition["isMovingForward"] = sol::property(&TES3::MobileActor::getMovementFlagForward, &TES3::MobileActor::setMovementFlagForward);
			usertypeDefinition["isMovingLeft"] = sol::property(&TES3::MobileActor::getMovementFlagLeft, &TES3::MobileActor::setMovementFlagLeft);
			usertypeDefinition["isMovingRight"] = sol::property(&TES3::MobileActor::getMovementFlagRight, &TES3::MobileActor::setMovementFlagRight);
			usertypeDefinition["isRunning"] = sol::property(&TES3::MobileActor::getMovementFlagRunning, &TES3::MobileActor::setMovementFlagRunning);
			usertypeDefinition["isSneaking"] = sol::property(&TES3::MobileActor::getMovementFlagSneaking, &TES3::MobileActor::setMovementFlagSneaking);
			usertypeDefinition["isFalling"] = sol::property(&TES3::MobileActor::getMovementFlagFalling, &TES3::MobileActor::setMovementFlagFalling);
			usertypeDefinition["isSwimming"] = sol::property(&TES3::MobileActor::getMovementFlagSwimming, &TES3::MobileActor::setMovementFlagSwimming);
			usertypeDefinition["isTurningLeft"] = sol::property(&TES3::MobileActor::getMovementFlagTurnLeft, &TES3::MobileActor::setMovementFlagTurnLeft);
			usertypeDefinition["isTurningRight"] = sol::property(&TES3::MobileActor::getMovementFlagTurnRight, &TES3::MobileActor::setMovementFlagTurnRight);
			usertypeDefinition["isWalking"] = sol::property(&TES3::MobileActor::getMovementFlagWalking, &TES3::MobileActor::setMovementFlagWalking);

			// Basic function binding.
			usertypeDefinition["applyHealthDamage"] = &TES3::MobileActor::applyHealthDamage;
			usertypeDefinition["applyFatigueDamage"] = &TES3::MobileActor::applyFatigueDamage;
			usertypeDefinition["applyDamage"] = &TES3::MobileActor::applyDamage_lua;
			usertypeDefinition["calcEffectiveDamage"] = &TES3::MobileActor::calcEffectiveDamage_lua;
			usertypeDefinition["getActiveMagicEffects"] = &TES3::MobileActor::getActiveMagicEffectsList_lua;
			usertypeDefinition["getBootsWeight"] = &TES3::MobileActor::getBootsWeight;
			usertypeDefinition["getViewToActor"] = &TES3::MobileActor::getViewToActor;
			usertypeDefinition["getViewToPoint"] = &TES3::MobileActor::getViewToPoint;
			usertypeDefinition["getViewToPointWithFacing"] = &TES3::MobileActor::getViewToPointWithFacing;
			usertypeDefinition["getFatigueTerm"] = &TES3::MobileActor::getFatigueTerm;
			usertypeDefinition["getPowerUseTimestamp"] = &TES3::MobileActor::getPowerUseTimestamp;
			usertypeDefinition["getSkillStatistic"] = &TES3::MobileActor::getSkillStatistic;
			usertypeDefinition["getSkillValue"] = &TES3::MobileActor::getSkillValue;
			usertypeDefinition["getWeaponSpeed"] = &TES3::MobileActor::getWeaponSpeed;
			usertypeDefinition["hasUsedPower"] = &TES3::MobileActor::hasUsedPower;
			usertypeDefinition["rechargePower"] = &TES3::MobileActor::rechargePower;
			usertypeDefinition["setPowerUseTimestamp"] = &TES3::MobileActor::setPowerUseTimestamp;
			usertypeDefinition["startCombat"] = &TES3::MobileActor::startCombat;
			usertypeDefinition["startDialogue"] = &TES3::MobileActor::startDialogue;
			usertypeDefinition["stopCombat"] = &TES3::MobileActor::stopCombat_lua;
			usertypeDefinition["updateDerivedStatistics"] = &TES3::MobileActor::updateDerivedStatistics_lua;
			usertypeDefinition["updateOpacity"] = &TES3::MobileActor::updateOpacity;

			// Provide single function for isAffectedByAlchemy, etc.
			usertypeDefinition["isAffectedByObject"] = &TES3::MobileActor::isAffectedByObject_lua;

			// Functions exposed as properties.
			usertypeDefinition["armorRating"] = sol::property(&TES3::MobileActor::getArmorRating_lua);
			usertypeDefinition["cell"] = sol::property(&TES3::MobileActor::getCell);
			usertypeDefinition["hasFreeAction"] = sol::property(&TES3::MobileActor::hasFreeAction);
			usertypeDefinition["mobToMobCollision"] = sol::property(&TES3::MobileActor::getMobToMobCollision, &TES3::MobileActor::setMobToMobCollision);

			// Legacy support for old structures.
			usertypeDefinition["activeMagicEffects"] = sol::readonly_property(&TES3::MobileActor::getActiveMagicEffects_legacy);
			usertypeDefinition["activeMagicEffectCount"] = sol::readonly_property(&TES3::MobileActor::getActiveMagicEffectCount_legacy);
		}

		void bindTES3MobileActor();
	}
}
