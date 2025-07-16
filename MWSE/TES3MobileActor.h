#pragma once

#include "TES3Defines.h"

#include "TES3ActionData.h"
#include "TES3AIBehavior.h"
#include "TES3Armor.h"
#include "TES3Clothing.h"
#include "TES3CrimeEvent.h"
#include "TES3Deque.h"
#include "TES3HashMap.h"
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
			Invisibility,
			Fight,
			Flee,
			Hello,
			Alarm,
			NonResistable
		};
	}

	namespace Voiceover {
		enum Voiceover {
			Hello,
			Idle,
			Intruder,
			Thief,
			Hit,
			Attack,
			Flee,

			First = Hello,
			Last = Flee
		};
	}

	struct ActiveMagicEffect {
		unsigned int magicInstanceSerial; // 0x0
		unsigned char magicInstanceEffectIndex; // 0x4
		unsigned char padding_0x5;
		short magicEffectID; // 0x6
		bool isHarmful; // 0x8
		bool isIllegalSummon; // 0x9
		unsigned short duration; // 0xA
		unsigned short unresistedMagnitude; // 0xC
		signed char skillOrAttributeID; // 0xE

		//
		// Custom functions.
		//

		MagicSourceInstance* getInstance() const;
		int getMagnitude() const;

		//
		// Lua interface functions.
		//

		ActiveMagicEffect* getNext_legacy() const;

	};
	static_assert(sizeof(ActiveMagicEffect) == 0x10, "TES3::ActiveMagicEffect failed size validation");

	struct ActiveMagicEffectLua : ActiveMagicEffect {
		MobileActor * mobile;

		ActiveMagicEffectLua(const ActiveMagicEffect& e, MobileActor * mobileActor)
			: ActiveMagicEffect(e), mobile(mobileActor) {}

		//
		// Lua interface functions.
		//

		MagicEffectInstance * getEffectInstance() const;
		bool isBoundItem() const;
		bool isSummon() const;

		ActiveMagicEffect* getFirst_legacy() const;
		ActiveMagicEffect* getNext_legacy();
	};

	struct MobileActor : MobileObject {
		IteratedList<MobileActor*> listTargetActors; // 0x80
		IteratedList<MobileActor*> listFriendlyActors; // 0x94
		float scanTimer; // 0xA8
		float scanInterval; // 0xAC
		float greetTimer; // 0xB0
		Vector3 unknown_0xB4;
		char unknown_0xC0;
		char unknown_0xC1; // Undefined.
		char unknown_0xC2; // Undefined.
		char unknown_0xC3; // Undefined.
		float timer_0xC4;
		AIPlanner * aiPlanner; // 0xC8
		ActionData actionData; // 0xCC
		ActionData actionBeforeCombat; // 0x13C
		Deque<CrimeEvent> committedCrimes; // 0x1AC
		int unknown_0x1B8;
		int unknown_0x1BC;
		CombatSession * combatSession; // 0x1C0
		Deque<ActiveMagicEffect> activeMagicEffects; // 0x1C4
		int unknown_0x1D0;
		Collision collision_1D4;
		HashMap<Spell*, double> powers; // 0x214
		char unknown_0x224;
		signed char prevAIBehaviourState; // 0x225
		char unknown_0x226;
		signed char nextActionWeight; // 0x227
		MobileActorType actorType; // 0x228
		char unknown_0x229;
		char unknown_0x22A; // Undefined.
		char unknown_0x22B; // Undefined.
		float lastGroundZ; // 0x22C
		int unknown_0x230;
		Reference* collidingReference; // 0x234
		Reference* moreCollidingReferences[2]; // 0x238
		int unknown_0x240;
		union {
			ActorAnimationController * asActor;
			PlayerAnimationController * asPlayer;
		} animationController; // 0x244
		Deque<CrimeEvent> witnessedCrimes; // 0x248
		Statistic attributes[8]; // 0x254
		Statistic health; // 0x2B4
		Statistic magicka; // 0x2C0
		Statistic encumbrance; // 0x290
		Statistic fatigue; // 0x2D8
		Statistic magickaMultiplier; // 0x2E4
		int effectAttributes[24]; // 0x2F0
		int fight; // 0x350
		int flee; // 0x354
		int hello; // 0x358
		int alarm; // 0x35C
		int barterGold; // 0x360
		unsigned short widthRescaled; // 0x364 // 11.5 fixed point
		unsigned short heightRescaled; // 0x366 // 11.5 fixed point
		short readiedAmmoCount; // 0x368
		short corpseHourstamp; // 0x36A
		short greetDuration; // 0x36C
		signed char friendlyFireHitCount; // 0x36E
		char unknown_0x36F; // Undefined.
		float holdBreathTime; // 0x370
		int unknown_0x374;
		MagicSourceCombo currentSpell; // 0x378
		EquipmentStack currentEnchantedItem; // 0x380 // Not a pointer.
		EquipmentStack * readiedWeapon; // 0x388
		EquipmentStack * readiedAmmo; // 0x38C
		EquipmentStack * readiedShield; // 0x390
		EquipmentStack * torchSlot; // 0x394
		char unknown_0x398;
		char unknown_0x399; // Undefined.
		char unknown_0x39A; // Undefined.
		char unknown_0x39B; // Undefined.
		void * arrowBone;
		int unknown_0x3A0;
		Vector3 unknown_0x3A4;

		MobileActor() = delete;
		~MobileActor() = delete;

		//
		// vTable accessor functions.
		//

		bool onActorCollision(int collisionIndex);
		bool onObjectCollision(int collisionIndex, bool flag);
		bool onTerrainCollision(int collisionIndex);
		bool onActivatorCollision(int collisionIndex);

		SkillStatistic * getSkillStatistic(int skillId);
		float getSkillValue(int skillId) const;

		float applyArmorRating(float damage, float swing, bool damageEquipment);
		float calculateArmorRating(int * armorItemCount = nullptr) const;
		float getArmorRating_lua() const;

		AnimationData* getAnimationData() const;

		//
		// Other related this-call functions.
		//

		Cell* getCell() const;

		float getFatigueTerm() const;

		float getFacing() const;
		float getViewToPoint(const Vector3* point) const;
		float getViewToPoint_lua(sol::object point) const;
		float getViewToPointWithFacing(float facing, const Vector3* point) const;
		float getViewToPointWithFacing_lua(float facing, sol::object point) const;
		float getViewToActor(const TES3::MobileActor* mobile) const;
		float getViewToActor_lua(sol::object mobile) const;

		float getBootsWeight() const;
		float getWeaponSpeed() const;
		float getAttackReach() const;
		float getEffectiveAttackDistance(const TES3::MobileActor* mobile) const;

		void startCombat(MobileActor*);
		void startCombat_lua(sol::object target);
		void stopCombat(bool);
		void stopCombat_lua(sol::optional<bool>);
		bool isDead() const;
		void onDeath();
		void kill();
		void retireMagic();
		bool applyHealthDamage(float damage, bool isPlayerAttack, bool scaleWithDifficulty, bool doNotChangeHealth);
		float applyFatigueDamage(float damage, float swing, bool alwaysPlayHitVoice = false);
		void applyJumpFatigueCost() const;
		float applyDamage_lua(sol::table params);
		float applyFatigueDamage_lua(float damage, float swing, bool alwaysPlayHitVoice = false);
		float calcEffectiveDamage_lua(sol::table params);
		bool doJump(Vector3 velocity, bool applyFatigueCost = true, bool isDefaultJump = false);
		bool doJump_lua(sol::optional<sol::table> params);

		bool isAttackingOrCasting() const;
		bool isHitStunned() const;
		bool isKnockedDown() const;
		bool isKnockedOut() const;
		bool isNotKnockedDownOrOut() const;
		bool isReadyingWeapon() const;
		bool isParalyzed() const;
		bool canAct() const;
		bool canJump(bool allowMidairJumping = false) const;
		bool canJump_lua() const;
		bool canJumpMidair_lua() const;
		bool canMove() const;

		float calculateRunSpeed();
		float calculateSwimSpeed();
		float calculateSwimRunSpeed();
		float calculateFlySpeed();
		Vector3 calculateJumpVelocity(Vector2 direction);
		Vector3 calculateJumpVelocity_lua(sol::optional<sol::table> params);
		void applyPhysicalHit(MobileActor* attacker, MobileActor* defender, float damage, float swing, MobileProjectile* projectile = nullptr, bool alwaysPlayHitVoice = false);
		void setCurrentMagicFromSpell(Spell* spell);
		void setCurrentMagicFromSourceCombo(MagicSourceCombo sourceCombo);
		void setCurrentMagicFromEquipmentStack(EquipmentStack* equipmentStack);

		void updateDerivedStatistics(Statistic * baseStatistic);
		void updateDerivedStatistics_lua(sol::optional<Statistic*> baseStatistic);

		int determineModifiedPrice(int basePrice, bool buying);

		void playVoiceover(int);
		void startDialogue();

		bool isAffectedByAlchemy(Alchemy* alchemy) const;
		bool isAffectedBySimilarAlchemy(Alchemy* alchemy) const;
		bool isAffectedByEnchantment(Enchantment* enchantment) const;
		bool isAffectedBySpell(Spell* spell) const;

		bool isDiseased() const;
		bool hasCommonDisease() const;
		bool hasBlightDisease() const;
		bool hasCorprusDisease() const;
		bool hasVampirism() const;

		SpellList* getSpellList();
		IteratedList<Spell*> * getCombatSpellList();

		bool isActive() const;
		void forceSpellCast(MobileActor * target);

		void dropItem(Object * item, ItemData * itemData = nullptr, int count = 1, bool ignoreItemData = true);

		// Always returns false for non-MACH.
		bool persuade(int random, int persuasionIndex);

		bool getIsWerewolf() const;
		void setIsWerewolf(bool set);
		void changeWerewolfState(bool isWerewolf);

		float getActionWeightFight(MobileActor* target);

		//
		// Custom functions.
		//

		bool getMobileActorFlag(MobileActorFlag::Flag) const;
		void setMobileActorFlag(MobileActorFlag::Flag, bool);

		bool getMobileActorMovementFlag(ActorMovement::Flag) const;
		void setMobileActorMovementFlag(ActorMovement::Flag, bool);

		bool isCreature() const;
		bool isPlayer() const;

		float getWidth() const;
		float getHeight() const;

		bool wearItem(Object* item, ItemData* itemData, bool addItem, bool unknown, bool useEvents);
		bool equipItem(Object* item, ItemData* itemData = nullptr, bool addItem = false, bool selectBestCondition = false, bool selectWorstCondition = false, bool useEvents = false);
		bool equip_lua(sol::object arg);
		bool unequip_lua(sol::table args);
		bool equipMagic(Object* source, ItemData* itemData = nullptr, bool equipItem = false, bool updateGUI = true);
		bool equipMagic_lua(sol::table params);
		void unequipMagic(bool unequipItem = false, bool updateGUI = true);
		void unequipMagic_lua(sol::optional<sol::table> params);

		bool getWeaponReady() const;
		void setWeaponReady(bool value);
		bool forceWeaponAttack_lua(sol::optional<sol::table> params);
		bool hitStun_lua(sol::optional<sol::table> params);
		void hitStunTestAndReportCrime(float damage, bool wasKillingBlow);

		void updateOpacity();
		void notifyActorDeadOrDestroyed(MobileActor* mobileActor);
		void removeFiredProjectiles(bool includeSpellProjectiles);
		void resurrect(bool resetState, bool moveToStartingLocation);
		void resurrect_lua(sol::table params);
		void overrideHeadTrackingThisFrame(Reference* target);

		ActorAnimationController* getAnimationController() const;
		BaseObject* getCurrentSpell() const;

		std::reference_wrapper<Statistic[8]> getAttributes();
		std::reference_wrapper<int[24]> getEffectAttributes();

		bool getFlagActiveAI() const;
		void setFlagActiveAI(bool value);
		bool getFlagAttacked() const;
		void setFlagAttacked(bool value);
		bool getFlagIdleAnim() const;
		void setFlagIdleAnim(bool value);
		bool getFlagInCombat() const;
		void setFlagInCombat(bool value);
		bool getFlagIsCrittable() const;
		void setFlagIsCrittable(bool value);
		bool getFlagIsPCDetected() const;
		void setFlagIsPCDetected(bool value);
		bool getFlagIsPCHidden() const;
		void setFlagIsPCHidden(bool value);
		bool getFlagSpellReadied() const;
		void setFlagSpellReadied(bool value);
		bool getFlagTalkedTo() const;
		void setFlagTalkedTo(bool value);
		bool getFlagUnderwater() const;
		void setFlagUnderwater(bool value);
		bool getFlagWeaponDrawn() const;
		void setFlagWeaponDrawn(bool value);
		bool getFlagWerewolf() const;
		void setFlagWerewolf(bool value);

		bool hasStatistic(const Statistic* statistic) const;

		Statistic* getAttributeAgility();
		Statistic* getAttributeEndurance();
		Statistic* getAttributeIntelligence();
		Statistic* getAttributeLuck();
		Statistic* getAttributePersonality();
		Statistic* getAttributeSpeed();
		Statistic* getAttributeStrength();
		Statistic* getAttributeWillpower();

		int getEffectAttributeAttackBonus() const;
		void setEffectAttributeAttackBonus(int value);
		int getEffectAttributeBlind() const;
		void setEffectAttributeBlind(int value);
		int getEffectAttributeChameleon() const;
		void setEffectAttributeChameleon(int value);
		int getEffectAttributeInvisibility() const;
		void setEffectAttributeInvisibility(int value);
		int getEffectAttributeJump() const;
		void setEffectAttributeJump(int value);
		int getEffectAttributeLevitate() const;
		void setEffectAttributeLevitate(int value);
		int getEffectAttributeParalyze() const;
		void setEffectAttributeParalyze(int value);
		int getEffectAttributeResistBlightDisease() const;
		void setEffectAttributeResistBlightDisease(int value);
		int getEffectAttributeResistCommonDisease() const;
		void setEffectAttributeResistCommonDisease(int value);
		int getEffectAttributeResistCorprus() const;
		void setEffectAttributeResistCorprus(int value);
		int getEffectAttributeResistFire() const;
		void setEffectAttributeResistFire(int value);
		int getEffectAttributeResistFrost() const;
		void setEffectAttributeResistFrost(int value);
		int getEffectAttributeResistMagicka() const;
		void setEffectAttributeResistMagicka(int value);
		int getEffectAttributeResistNormalWeapons() const;
		void setEffectAttributeResistNormalWeapons(int value);
		int getEffectAttributeResistParalysis() const;
		void setEffectAttributeResistParalysis(int value);
		int getEffectAttributeResistPoison() const;
		void setEffectAttributeResistPoison(int value);
		int getEffectAttributeResistShock() const;
		void setEffectAttributeResistShock(int value);
		int getEffectAttributeSanctuary() const;
		void setEffectAttributeSanctuary(int value);
		int getEffectAttributeShield() const;
		void setEffectAttributeShield(int value);
		int getEffectAttributeSilence() const;
		void setEffectAttributeSilence(int value);
		int getEffectAttributeSound() const;
		void setEffectAttributeSound(int value);
		int getEffectAttributeSwiftSwim() const;
		void setEffectAttributeSwiftSwim(int value);
		int getEffectAttributeWaterBreathing() const;
		void setEffectAttributeWaterBreathing(int value);
		int getEffectAttributeWaterWalking() const;
		void setEffectAttributeWaterWalking(int value);

		bool getMovementFlagFlying() const;
		void setMovementFlagFlying(bool value);
		bool getMovementFlagJumping() const;
		void setMovementFlagJumping(bool value);
		bool getMovementFlagBack() const;
		void setMovementFlagBack(bool value);
		bool getMovementFlagForward() const;
		void setMovementFlagForward(bool value);
		bool getMovementFlagLeft() const;
		void setMovementFlagLeft(bool value);
		bool getMovementFlagRight() const;
		void setMovementFlagRight(bool value);
		bool getMovementFlagRunning() const;
		void setMovementFlagRunning(bool value);
		bool getMovementFlagSliding() const;
		void setMovementFlagSliding(bool value);
		bool getMovementFlagSneaking() const;
		void setMovementFlagSneaking(bool value);
		bool getMovementFlagFalling() const;
		void setMovementFlagFalling(bool value);
		bool getMovementFlagSwimming() const;
		void setMovementFlagSwimming(bool value);
		bool getMovementFlagTurnLeft() const;
		void setMovementFlagTurnLeft(bool value);
		bool getMovementFlagTurnRight() const;
		void setMovementFlagTurnRight(bool value);
		bool getMovementFlagWalking() const;
		void setMovementFlagWalking(bool value);

		bool isSpeaking() const;

		bool isAffectedByObject_lua(sol::object object) const;

		bool hasSummonEffect();

		bool hasUsedPower(Spell* power) const;
		bool rechargePower(Spell* power);
		sol::optional<double> getPowerUseTimestamp(Spell* power) const;
		void setPowerUseTimestamp(Spell* power, double timestamp);

		bool getMobToMobCollision() const;
		void setMobToMobCollision(bool collide);

		sol::table getActiveMagicEffectsList_lua(sol::optional<sol::table> params);
		ActiveMagicEffect* getActiveMagicEffects_legacy() const;
		int getActiveMagicEffectCount_legacy() const;
	};
	static_assert(sizeof(MobileActor) == 0x3B0, "TES3::MobileActor failed size validation");
	static_assert(sizeof(decltype(MobileActor::powers)::Node) == 0x18, "TES3::MobileActor::powers::Node failed size validation");
	static_assert(offsetof(decltype(MobileActor::powers)::Node, value) == 0x8, "TES3::MobileActor::powers::Node::value failed offset validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3_MOBILEOBJECT(TES3::MobileActor)
