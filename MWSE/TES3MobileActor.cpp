#include "TES3MobileActor.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaCalcMovementSpeedEvent.h"
#include "LuaCombatStartedEvent.h"
#include "LuaCombatStartEvent.h"
#include "LuaCombatStopEvent.h"
#include "LuaCombatStoppedEvent.h"
#include "LuaDamagedEvent.h"
#include "LuaDamagedHandToHandEvent.h"
#include "LuaDamageEvent.h"
#include "LuaDamageHandToHandEvent.h"
#include "LuaDeathEvent.h"
#include "LuaMobileObjectCollisionEvent.h"

#include "TES3Actor.h"
#include "TES3ActorAnimationController.h"
#include "TES3Alchemy.h"
#include "TES3AudioController.h"
#include "TES3Enchantment.h"
#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3MagicEffectController.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicInstanceController.h"
#include "TES3MobController.h"
#include "TES3MobilePlayer.h"
#include "TES3ItemData.h"
#include "TES3Spell.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

#include "TES3Util.h"

#define TES3_MobileActor_getCell 0x521630
#define TES3_MobileActor_stopCombat 0x558720
#define TES3_MobileActor_onDeath 0x523AA0
#define TES3_MobileActor_hasFreeAction 0x527580
#define TES3_MobileActor_calculateRunSpeed 0x527050
#define TES3_MobileActor_calculateSwimSpeed 0x5270B0
#define TES3_MobileActor_calculateSwimRunSpeed 0x527130
#define TES3_MobileActor_calculateFlySpeed 0x5271F0

namespace TES3 {
	const auto TES3_MobileActor_isInAttackAnim = reinterpret_cast<bool (__thiscall*)(const MobileActor*)>(0x5567D0);
	const auto TES3_MobileActor_wearItem = reinterpret_cast<void (__thiscall*)(MobileActor*, Object*, ItemData*, bool, bool)>(0x52C770);
	const auto TES3_MobileActor_calcDerivedStats = reinterpret_cast<void(__thiscall*)(const MobileActor*, Statistic*)>(0x527BC0);
	const auto TES3_MobileActor_determineModifiedPrice = reinterpret_cast<int(__thiscall*)(const MobileActor*, int, int)>(0x52AA50);
	const auto TES3_MobileActor_playVoiceover = reinterpret_cast<void(__thiscall*)(const MobileActor*, int)>(0x528F80);

	MagicSourceInstance* ActiveMagicEffect::getInstance() const {
		return WorldController::get()->magicInstanceController->getInstanceFromSerial(magicInstanceSerial);
	}

	int ActiveMagicEffect::getMagnitude() const {
		return unresistedMagnitude;
	}

	static_assert(offsetof(Deque<ActiveMagicEffect>::Node, data) == 0x8);
	ActiveMagicEffect* ActiveMagicEffect::getNext_legacy() const {
		auto node = reinterpret_cast<const Deque<ActiveMagicEffect>::Node*>(DWORD(this) - offsetof(Deque<ActiveMagicEffect>::Node, data));
		return &node->next->data;
	}

	MagicEffectInstance * ActiveMagicEffectLua::getEffectInstance() const {
		auto magicInstance = getInstance();
		if (magicInstance) {
			return magicInstance->getEffectInstance(magicInstanceEffectIndex, mobile->reference);
		}
		return nullptr;
	}

	ActiveMagicEffect* ActiveMagicEffectLua::getFirst_legacy() const {
		return &mobile->activeMagicEffects.front();
	}

	ActiveMagicEffect* ActiveMagicEffectLua::getNext_legacy() {
		// This is a bad solution that only works for the legacy codebase we have to support.
		// Due to how ActiveMagicEffect::getNext_legacy above functions, and how ActiveMagicEffectLua
		// is copied when pushed to lua, its possible for this copy to break. We have to overwrite it
		// otherwise it will crash.
		return this;
	}

	const auto TES3_MobileActor_onActorCollision = reinterpret_cast<bool(__thiscall*)(MobileActor*, int)>(0x5234A0);
	bool MobileActor::onActorCollision(int collisionIndex) {
		// Grab the hit reference now, it won't be available after calling the main function.
		TES3::Reference* hitReference = arrayCollisionResults[collisionIndex].colliderRef;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		bool result = TES3_MobileActor_onActorCollision(this, collisionIndex);

		// Fire off our hit event.
		if (mwse::lua::event::MobileObjectCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectCollisionEvent(this, hitReference));
		}

		return result;
	}

	const auto TES3_MobileActor_onObjectCollision = reinterpret_cast<bool(__thiscall*)(MobileActor*, int, bool)>(0x5233B0);
	bool MobileActor::onObjectCollision(int collisionIndex, bool flag) {
		// Grab the hit reference now, it won't be available after calling the main function.
		TES3::Reference* hitReference = arrayCollisionResults[collisionIndex].colliderRef;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		bool result = TES3_MobileActor_onObjectCollision(this, collisionIndex, flag);

		// Fire off our hit event.
		if (mwse::lua::event::MobileObjectCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectCollisionEvent(this, hitReference));
		}

		return result;
	}

	const auto TES3_MobileActor_onTerrainCollision = reinterpret_cast<bool(__thiscall*)(MobileActor*, int)>(0x523310);
	bool MobileActor::onTerrainCollision(int collisionIndex) {
		// Grab the hit reference now, it won't be available after calling the main function.
		TES3::Reference* hitReference = arrayCollisionResults[collisionIndex].colliderRef;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		bool result = TES3_MobileActor_onTerrainCollision(this, collisionIndex);

		// Fire off our hit event.
		if (mwse::lua::event::MobileObjectCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectCollisionEvent(this, hitReference));
		}

		return result;
	}

	const auto TES3_MobileActor_onActivatorCollision = reinterpret_cast<bool(__thiscall*)(MobileActor*, int)>(0x523590);
	bool MobileActor::onActivatorCollision(int collisionIndex) {
		// Grab the hit reference now, it won't be available after calling the main function.
		TES3::Reference* hitReference = arrayCollisionResults[collisionIndex].colliderRef;

		// Call the original function. We can't invoke the vtable here because we overwrite it.
		bool result = TES3_MobileActor_onActivatorCollision(this, collisionIndex);

		// Fire off our hit event.
		if (mwse::lua::event::MobileObjectCollisionEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectCollisionEvent(this, hitReference));
		}

		return result;
	}

	SkillStatistic * MobileActor::getSkillStatistic(int skillId) {
		return vTable.mobileActor->getSkillStatistic(this, skillId);
	}

	float MobileActor::getSkillValue(int skillId) const {
		return vTable.mobileActor->getSkillValue(this, skillId);
	}

	float MobileActor::applyArmorRating(float damage, float swing, bool damageEquipment) {
		return vTable.mobileActor->applyArmorRating(this, damage, swing, damageEquipment);
	}

	float MobileActor::calculateArmorRating(int * armorItemCount) const {
		return vTable.mobileActor->calculateArmorRating(this, armorItemCount);
	}

	float MobileActor::getArmorRating_lua() const {
		return calculateArmorRating(nullptr);
	}

	const auto TES3_MobileActor_applyPhysicalHit = reinterpret_cast<void(__thiscall *)(MobileActor*, MobileActor*, MobileActor*, float, float, MobileProjectile*, bool)>(0x5568F0);
	void MobileActor::applyPhysicalHit(MobileActor * attacker, MobileActor * defender, float damage, float swing, MobileProjectile * projectile, bool alwaysPlayHitVoice) {
		// Setup damage event data.
		mwse::lua::event::DamageEvent::m_Attacker = attacker;
		mwse::lua::event::DamageEvent::m_Projectile = projectile;

		// Call original function.
		TES3_MobileActor_applyPhysicalHit(this, attacker, defender, damage, swing, projectile, alwaysPlayHitVoice);

		// Clean up damage event data.
		mwse::lua::event::DamageEvent::m_Attacker = nullptr;
		mwse::lua::event::DamageEvent::m_Projectile = nullptr;
	}

	const auto TES3_MobileActor_setCurrentSpell = reinterpret_cast<void(__thiscall*)(MobileActor*, const Spell*)>(0x52B390);
	void MobileActor::setCurrentSpell(const Spell* spell) {
		TES3_MobileActor_setCurrentSpell(this, spell);
	}

	Cell* MobileActor::getCell() {
		return reinterpret_cast<Cell*(__thiscall *)(MobileActor*)>(TES3_MobileActor_getCell)(this);
	}

	const auto TES3_MobileActor_getFatigueTerm = reinterpret_cast<float(__thiscall *)(const MobileActor*)>(0x527610);
	float MobileActor::getFatigueTerm() const {
		return TES3_MobileActor_getFatigueTerm(this);
	}

	float MobileActor::getFacing() const {
		if (reference) {
			return reference->orientation.z;
		}
		return 0.0f;
	}

	float MobileActor::getViewToPoint(const Vector3* point) const {
		return getViewToPointWithFacing(getFacing(), point);
	}

	const auto TES3_MobileActor_getViewToPointWithFacing = reinterpret_cast<float(__thiscall*)(const MobileActor*, float, const Vector3*)>(0x5264C0);
	float MobileActor::getViewToPointWithFacing(float facing, const Vector3* point) const {
		return TES3_MobileActor_getViewToPointWithFacing(this, facing, point);
	}

	float MobileActor::getViewToActor(const TES3::MobileActor* mobile) const {
		return getViewToPointWithFacing(getFacing(), &mobile->reference->position);
	}

	const auto TES3_MobileActor_getBootsWeight = reinterpret_cast<float(__thiscall*)(const MobileActor*)>(0x526F30);
	float MobileActor::getBootsWeight() const {
		return TES3_MobileActor_getBootsWeight(this);
	}

	const auto TES3_MobileActor_getWeaponSpeed = reinterpret_cast<float(__thiscall*)(const MobileActor*)>(0x556580);
	float MobileActor::getWeaponSpeed() const {
		return TES3_MobileActor_getWeaponSpeed(this);
	}

	const auto TES3_MobileActor_startCombat = reinterpret_cast<void(__thiscall*)(MobileActor*, MobileActor*)>(0x530470);
	void MobileActor::startCombat(MobileActor* target) {
		// Patch: Make sure that disabled NPCs can't start combat.
		if (reference->getDisabled()) {
			return;
		}

		// Invoke our first event and check if it is blocked.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		if (mwse::lua::event::CombatStartEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CombatStartEvent(this, target));
			if (eventData.valid() && eventData.get_or("block", false)) {
				return;
			}
		}

		// Call original function.
		TES3_MobileActor_startCombat(this, target);

		// Do our follow up started event.
		if (mwse::lua::event::CombatStartedEvent::getEventEnabled()) {
			luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::CombatStartedEvent(this, target));
		}
	}

	void MobileActor::stopCombat(bool something) {
		// Invoke our combat stop event and check if it is blocked.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		if (mwse::lua::event::CombatStopEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CombatStopEvent(this));
			if (eventData.valid() && eventData.get_or("block", false)) {
				return;
			}
		}

		// Call original function.
		reinterpret_cast<void(__thiscall *)(MobileActor*, bool)>(TES3_MobileActor_stopCombat)(this, something);

		// Do our follow up stopped event.
		if (mwse::lua::event::CombatStoppedEvent::getEventEnabled()) {
			luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::CombatStoppedEvent(this));
		}
	}

	void MobileActor::stopCombat_lua(sol::optional<bool> something) {
		stopCombat(something.value_or(false));
	}

	bool MobileActor::isDead() const {
		return actionData.animStateAttack == AttackAnimationState::Dead || actionData.animStateAttack == AttackAnimationState::Dying;
	}

	void MobileActor::onDeath() {
		reinterpret_cast<void(__thiscall *)(MobileActor*)>(TES3_MobileActor_onDeath)(this);

		// Trigger death event.
		if (mwse::lua::event::DeathEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::DeathEvent(this));
		}
	}

	const auto TES3_MobileActor_applyHealthDamage = reinterpret_cast<bool(__thiscall*)(MobileActor*, float, bool, bool, bool)>(0x557CF0);
	bool MobileActor::applyHealthDamage(float damage, bool isPlayerAttack, bool scaleWithDifficulty, bool doNotChangeHealth) {
		// Invoke our pre-damage event and check if it is blocked.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		if (mwse::lua::event::DamageEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::DamageEvent(this, damage));
			if (eventData.valid()) {
				if (eventData.get_or("block", false)) {
					return false;
				}

				damage = eventData["damage"];
			}
		}

		bool killingBlow = TES3_MobileActor_applyHealthDamage(this, damage, isPlayerAttack, scaleWithDifficulty, doNotChangeHealth);

		// Do our post-damage event.
		if (mwse::lua::event::DamagedEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::DamagedEvent(this, damage, killingBlow));
			if (eventData.valid()) {
				killingBlow = eventData["killingBlow"];
			}
		}

		return killingBlow;
	}

	float MobileActor::applyDamage_lua(sol::table params) {
		bool applyArmor = params["applyArmor"].get_or(false);
		bool applyDifficulty = params["applyDifficulty"].get_or(false);
		sol::optional<float> damage = params["damage"];
		bool doNotChangeHealth = params["doNotChangeHealth"].get_or(false);
		sol::optional<bool> playerAttack = params["playerAttack"];
		sol::optional<int> resistAttribute = params["resistAttribute"];
		int resistIndex = resistAttribute.value_or(-1);

		if (applyDifficulty && !playerAttack) {
			throw std::invalid_argument("'applyDifficulty' requires a 'playerAttack' parameter.");
		}
		if (!damage) {
			throw std::invalid_argument("Invalid 'damage' parameter provided.");
		}
		if (resistAttribute) {
			if (resistIndex < TES3::MagicEffectAttribute::AttackBonus || resistIndex > TES3::MagicEffectAttribute::Invisibility) {
				// Disable resistAttribute for effect attributes that are > Invisibility.
				resistAttribute.reset();
			}
		}

		float adjustedDamage = damage.value();
		// Apply armor mitigation. Includes damaging armor condition, hit sounds, and player armor skill experience.
		if (applyArmor) {
			adjustedDamage = applyArmorRating(adjustedDamage, 1.0f, true);
		}
		// Effect attribute based resistance/weakness.
		if (resistAttribute) {
			adjustedDamage *= std::max(0, 100 - this->effectAttributes[resistIndex]) / 100.0f;
		}

		auto prevSource = mwse::lua::event::DamageEvent::m_Source;
		mwse::lua::event::DamageEvent::m_Source = "script";
		applyHealthDamage(adjustedDamage, playerAttack.value_or(false), applyDifficulty, doNotChangeHealth);
		mwse::lua::event::DamageEvent::m_Source = prevSource;

		return adjustedDamage;
	}

	float MobileActor::calcEffectiveDamage_lua(sol::table params) {
		bool applyArmor = params["applyArmor"].get_or(false);
		sol::optional<float> damage = params["damage"];
		sol::optional<int> resistAttribute = params["resistAttribute"];
		int resistIndex = resistAttribute.value_or(-1);

		if (!damage) {
			throw std::invalid_argument("Invalid 'damage' parameter provided.");
		}
		if (resistAttribute) {
			if (resistIndex < TES3::MagicEffectAttribute::AttackBonus || resistIndex > TES3::MagicEffectAttribute::Invisibility) {
				// Disable resistAttribute for effect attributes that are > Invisibility.
				resistAttribute.reset();
			}
		}

		// Emulate armor mitigation.
		float adjustedDamage = damage.value();
		if (applyArmor) {
			if (adjustedDamage > 0.001f) {
				float fCombatArmorMinMult = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fCombatArmorMinMult]->value.asFloat;
				float armor = calculateArmorRating();
				float reducer = std::max(fCombatArmorMinMult, adjustedDamage / (armor + adjustedDamage));
				adjustedDamage = std::max(1.0f, reducer * adjustedDamage);
			}
			else {
				adjustedDamage = 0;
			}
		}
		// Effect attribute based resistance/weakness.
		if (resistAttribute) {
			adjustedDamage *= std::max(0, 100 - this->effectAttributes[resistIndex]) / 100.0f;
		}

		return adjustedDamage;
	}

	const auto TES3_MobileActor_applyFatigueDamage = reinterpret_cast<float(__thiscall*)(MobileActor*, float, float, bool)>(0x5581B0);
	float MobileActor::applyFatigueDamage(float fatigueDamage, float swing, bool alwaysPlayHitVoice) {
		// Invoke our pre-damage event and check if it is blocked.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		if (mwse::lua::event::DamageHandToHandEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::DamageHandToHandEvent(this, fatigueDamage));
			if (eventData.valid()) {
				if (eventData.get_or("block", false)) {
					return 0;
				}

				fatigueDamage = eventData["fatigueDamage"];
			}
		}

		float result = TES3_MobileActor_applyFatigueDamage(this, fatigueDamage, swing, alwaysPlayHitVoice);

		// Do our post-damage event.
		if (mwse::lua::event::DamagedHandToHandEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::DamagedHandToHandEvent(this, fatigueDamage));
		}
		return result;
	}

	bool MobileActor::hasFreeAction() const {
		return reinterpret_cast<bool(__thiscall *)(const MobileActor*)>(TES3_MobileActor_hasFreeAction)(this);
	}

	float MobileActor::calculateRunSpeed() {
		// Call the original function to get the default movement speed value.
		float speed = reinterpret_cast<float(__thiscall *)(MobileActor*)>(TES3_MobileActor_calculateRunSpeed)(this);

		// Launch our event, and overwrite the speed with what was given back to us.
		if (mwse::lua::event::CalculateMovementSpeed::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CalculateMovementSpeed(mwse::lua::event::CalculateMovementSpeed::Run, this, speed));
			if (eventData.valid()) {
				speed = eventData["speed"];
			}
		}

		return speed;
	}

	float MobileActor::calculateSwimSpeed() {
		// Call the original function to get the default movement speed value.
		float speed = reinterpret_cast<float(__thiscall *)(MobileActor*)>(TES3_MobileActor_calculateSwimSpeed)(this);

		// Launch our event, and overwrite the speed with what was given back to us.
		if (mwse::lua::event::CalculateMovementSpeed::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CalculateMovementSpeed(mwse::lua::event::CalculateMovementSpeed::Swim, this, speed));
			if (eventData.valid()) {
				speed = eventData["speed"];
			}
		}

		return speed;
	}

	float MobileActor::calculateSwimRunSpeed() {
		// In this case, we don't want to call the original function. We want to redo it so that it calls
		// the calculateSwimSpeed() function.
		TES3::NonDynamicData * nonDynamicData = TES3::DataHandler::get()->nonDynamicData;
		float runMultiplier = nonDynamicData->GMSTs[GMST::fBaseRunMultiplier]->value.asFloat +
			nonDynamicData->GMSTs[GMST::fAthleticsRunBonus]->value.asFloat / 100.0f * getSkillValue(SkillID::Athletics);
		float speed = calculateSwimSpeed() * runMultiplier;

		// Launch our event, and overwrite the speed with what was given back to us.
		if (mwse::lua::event::CalculateMovementSpeed::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CalculateMovementSpeed(mwse::lua::event::CalculateMovementSpeed::SwimRun, this, speed));
			if (eventData.valid()) {
				speed = eventData["speed"];
			}
		}

		return speed;
	}

	float MobileActor::calculateFlySpeed() {
		// Call the original function to get the default movement speed value.
		float speed = reinterpret_cast<float(__thiscall *)(MobileActor*)>(TES3_MobileActor_calculateFlySpeed)(this);

		// Launch our event, and overwrite the speed with what was given back to us.
		if (mwse::lua::event::CalculateMovementSpeed::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CalculateMovementSpeed(mwse::lua::event::CalculateMovementSpeed::Fly, this, speed));
			if (eventData.valid()) {
				speed = eventData["speed"];
			}
		}

		return speed;
	}

	void MobileActor::updateDerivedStatistics(Statistic * baseStatistic) {
		TES3_MobileActor_calcDerivedStats(this, baseStatistic);
	}

	void MobileActor::updateDerivedStatistics_lua(sol::optional<Statistic*> baseStatistic) {
		TES3_MobileActor_calcDerivedStats(this, baseStatistic.value_or(nullptr));
	}

	int MobileActor::determineModifiedPrice(int basePrice, bool buying) {
		return TES3_MobileActor_determineModifiedPrice(this, basePrice, buying);
	}

	void MobileActor::playVoiceover(int voiceover) {
		TES3_MobileActor_playVoiceover(this, voiceover);
	}

	const auto TES3_MobileActor_startDialogue = reinterpret_cast<void(__thiscall*)(const MobileActor*)>(0x529260);
	void MobileActor::startDialogue() {
		TES3_MobileActor_startDialogue(this);
	}

	const auto TES3_MobileActor_isAffectedByAlchemy = reinterpret_cast<bool(__thiscall*)(const MobileActor*, Alchemy*)>(0x52D1A0);
	bool MobileActor::isAffectedByAlchemy(Alchemy * alchemy) const {
		return TES3_MobileActor_isAffectedByAlchemy(this, alchemy);
	}

	const auto TES3_MobileActor_isAffectedByEnchantment = reinterpret_cast<bool(__thiscall*)(const MobileActor*, Enchantment*)>(0x52D140);
	bool MobileActor::isAffectedByEnchantment(Enchantment * enchantment) const {
		return TES3_MobileActor_isAffectedByEnchantment(this, enchantment);
	}

	const auto TES3_MobileActor_isAffectedBySpell = reinterpret_cast<bool(__thiscall*)(const MobileActor*, Spell*)>(0x52D0E0);
	bool MobileActor::isAffectedBySpell(Spell * spell) const {
		return TES3_MobileActor_isAffectedBySpell(this, spell);
	}

	const auto TES3_MobileActor_getCombatSpellList = reinterpret_cast<IteratedList<Spell*> *(__thiscall*)(const MobileActor*)>(0x52B3C0);
	IteratedList<Spell*> * MobileActor::getCombatSpellList() {
		return TES3_MobileActor_getCombatSpellList(this);
	}

	const auto TES3_MobileActor_isActive = reinterpret_cast<bool(__thiscall*)(const MobileActor*)>(0x50F5F0);
	bool MobileActor::isActive() {
		return TES3_MobileActor_isActive(this);
	}

	const auto TES3_MobileActor_setCurrentMagicSourceFiltered = reinterpret_cast<bool(__thiscall*)(MobileActor*, Object*, int)>(0x52B220);
	void MobileActor::setCurrentMagicSourceFiltered(Object * magic) {
		TES3_MobileActor_setCurrentMagicSourceFiltered(this, magic, 0);
	}

	const auto TES3_MobileActor_forceSpellCast = reinterpret_cast<bool(__thiscall*)(MobileActor*, MobileActor*)>(0x52F790);
	void MobileActor::forceSpellCast(MobileActor * target) {
		TES3_MobileActor_forceSpellCast(this, target);
	}

	const auto TES3_MobileActor_dropItem = reinterpret_cast<void (__thiscall*)(MobileActor*, Object*, ItemData*, int, bool)>(0x52C460);
	void MobileActor::dropItem(Object * item, ItemData * itemData, int count, bool ignoreItemData) {
		return TES3_MobileActor_dropItem(this, item, itemData, count, ignoreItemData);
	}

	const auto TES3_MobileActor_persuade = reinterpret_cast<bool(__thiscall*)(MobileActor*, int, int)>(0x529D10);
	bool MobileActor::persuade(int random, int persuasionIndex) {
		return TES3_MobileActor_persuade(this, random, persuasionIndex);
	}

	bool MobileActor::getIsWerewolf() {
		return getMobileActorFlag(TES3::MobileActorFlag::Werewolf);
	}

	void MobileActor::setIsWerewolf(bool set) {
		setMobileActorFlag(TES3::MobileActorFlag::Werewolf, set);
	}
	
	void MobileActor::changeWerewolfState(bool isWerewolf) {
		vTable.mobileActor->changeWerewolf(this, isWerewolf);
	}

	const auto TES3_MobileActor_getActionWeightFight = reinterpret_cast<float(__thiscall*)(MobileActor*, MobileActor*)>(0x530A20);
	float MobileActor::getActionWeightFight(MobileActor* target) {
		return TES3_MobileActor_getActionWeightFight(this, target);
	}

	bool MobileActor::getMobileActorFlag(MobileActorFlag::Flag flag) const {
		return (actorFlags & flag) != 0;
	}

	void MobileActor::setMobileActorFlag(MobileActorFlag::Flag flag, bool set) {
		if (set) {
			actorFlags |= flag;
		}
		else {
			actorFlags &= ~flag;
		}
	}

	bool MobileActor::getMobileActorMovementFlag(ActorMovement::Flag flag) const {
		return (movementFlags & flag) != 0;
	}

	void MobileActor::setMobileActorMovementFlag(ActorMovement::Flag flag, bool set) {
		if (set) {
			movementFlags |= flag;
		}
		else {
			movementFlags &= ~flag;
		}
	}

	bool MobileActor::equipItem(Object* item, ItemData* itemData, bool addItem, bool selectBestCondition, bool selectWorstCondition) {
		Actor* actor = static_cast<Actor*>(reference->baseObject);

		// Equipping weapons while they are in use breaks animations and AI.
		if (item->objectType == ObjectType::Weapon && TES3_MobileActor_isInAttackAnim(this)) {
			return false;
		}

		// Check if item exists in the inventory.
		ItemStack* s = actor->inventory.findItemStack(item);
		if (!s) {
			if (addItem) {
				TES3_MobileActor_wearItem(this, item, itemData, addItem, false);
				return true;
			}
			return false;
		}
		else if (itemData && !(s->variables && s->variables->contains(itemData))) {
			return false;
		}

		// Prefer an already equipped item when selecting an item data match.
		EquipmentStack* equipped = actor->getEquippedItem(item);

		// Select item based on best/worst condition.
		if (selectBestCondition) {
			// Use already equipped item if it has full condition.
			if (equipped && ItemData::isFullyRepaired(equipped->variables, static_cast<TES3::Item*>(equipped->object))) {
				return true;
			}

			// Scan for best item condition.
			itemData = nullptr;

			// Only scan if necessary; fully repaired items exist if stack count is greater than variables count.
			if (s->variables && s->count == s->variables->filledCount) {
				int bestCondition = -1;
				float bestCharge = -1.0f;

				for (auto it = s->variables->cbegin(), end = it + s->variables->endIndex; it != end; ++it) {
					// Select best condition, secondarily select best charge.
					TES3::ItemData* i = *it;
					if (i == nullptr) {
						// nullptr condition represents a fully repaired item.
						itemData = nullptr;
						break;
					}
					if (i->condition > bestCondition || (i->condition == bestCondition && i->charge > bestCharge)) {
						itemData = i;
						bestCondition = i->condition;
						bestCharge = i->charge;
					}
				}
			}
		}
		else if (selectWorstCondition) {
			// Scan for worst item condition.
			itemData = nullptr;

			if (s->variables) {
				const auto firstVar = s->variables->at(0);
				int worstCondition = firstVar->condition;
				float worstCharge = firstVar->charge;
				itemData = firstVar;

				for (auto it = s->variables->cbegin(), end = it + s->variables->endIndex; it != end; ++it) {
					// Select worst condition, secondarily select worst charge.
					TES3::ItemData* i = *it;
					if (i == nullptr) {
						continue;
					}
					if (i->condition < worstCondition || (i->condition == worstCondition && i->charge < worstCharge)) {
						itemData = i;
						worstCondition = i->condition;
						worstCharge = i->charge;
					}
				}
			}
		}
		// Get any matching item if no itemdata and no flags are set.
		else if (itemData == nullptr) {
			// Prefer to match an already equipped item.
			if (equipped) {
				return true;
			}
			// Get first available itemdata if one isn't provided.
			else if (s->variables != nullptr) {
				itemData = s->variables->at(0);
			}
		}

		// Check if selected item is already equipped, to avoid a Morrowind crash.
		// wearItem unequips the slot first, and unequipping a fully repaired item may deallocate the existing itemData.
		if (actor->getEquippedItemExact(item, itemData)) {
			return true;
		}

		TES3_MobileActor_wearItem(this, item, itemData, false, false);
		return true;
	}

	void MobileActor::updateOpacity() {
		if (animationController.asActor) {
			animationController.asActor->updateOpacity();
		}
	}

	ActorAnimationController* MobileActor::getAnimationController() const {
		return animationController.asActor;
	}

	BaseObject* MobileActor::getCurrentSpell() const {
		return currentSpell.source.asGeneric;
	}

	std::reference_wrapper<Statistic[8]> MobileActor::getAttributes() {
		return std::ref(attributes);
	}

	std::reference_wrapper<int[24]> MobileActor::getEffectAttributes() {
		return std::ref(effectAttributes);
	}

	bool MobileActor::getFlagActiveAI() const {
		return getMobileActorFlag(MobileActorFlag::ActiveInSimulation);
	}

	void MobileActor::setFlagActiveAI(bool value) {
		setMobileActorFlag(MobileActorFlag::ActiveInSimulation, value);
	}

	bool MobileActor::getFlagAttacked() const {
		return getMobileActorFlag(MobileActorFlag::Attacked);
	}

	void MobileActor::setFlagAttacked(bool value) {
		setMobileActorFlag(MobileActorFlag::Attacked, value);
	}

	bool MobileActor::getFlagIdleAnim() const {
		return getMobileActorFlag(MobileActorFlag::IdleAnim);
	}

	void MobileActor::setFlagIdleAnim(bool value) {
		setMobileActorFlag(MobileActorFlag::IdleAnim, value);
	}

	bool MobileActor::getFlagInCombat() const {
		return getMobileActorFlag(MobileActorFlag::InCombat);
	}

	void MobileActor::setFlagInCombat(bool value) {
		setMobileActorFlag(MobileActorFlag::InCombat, value);
	}

	bool MobileActor::getFlagIsCrittable() const {
		return getMobileActorFlag(MobileActorFlag::IsCrittable);
	}

	void MobileActor::setFlagIsCrittable(bool value) {
		setMobileActorFlag(MobileActorFlag::IsCrittable, value);
	}

	bool MobileActor::getFlagIsPCDetected() const {
		return getMobileActorFlag(MobileActorFlag::PCDetected);
	}

	void MobileActor::setFlagIsPCDetected(bool value) {
		setMobileActorFlag(MobileActorFlag::PCDetected, value);
	}

	bool MobileActor::getFlagIsPCHidden() const {
		return getMobileActorFlag(MobileActorFlag::PCHidden);
	}

	void MobileActor::setFlagIsPCHidden(bool value) {
		setMobileActorFlag(MobileActorFlag::PCHidden, value);
	}

	bool MobileActor::getFlagSpellReadied() const {
		return getMobileActorFlag(MobileActorFlag::SpellReadied);
	}

	void MobileActor::setFlagSpellReadied(bool value) {
		setMobileActorFlag(MobileActorFlag::SpellReadied, value);
	}

	bool MobileActor::getFlagTalkedTo() const {
		return getMobileActorFlag(MobileActorFlag::TalkedTo);
	}

	void MobileActor::setFlagTalkedTo(bool value) {
		setMobileActorFlag(MobileActorFlag::TalkedTo, value);
	}

	bool MobileActor::getFlagUnderwater() const {
		return getMobileActorFlag(MobileActorFlag::Underwater);
	}

	void MobileActor::setFlagUnderwater(bool value) {
		setMobileActorFlag(MobileActorFlag::Underwater, value);
	}

	bool MobileActor::getFlagWeaponDrawn() const {
		return getMobileActorFlag(MobileActorFlag::WeaponDrawn);
	}

	void MobileActor::setFlagWeaponDrawn(bool value) {
		setMobileActorFlag(MobileActorFlag::WeaponDrawn, value);
	}

	bool MobileActor::getFlagWerewolf() const {
		return getMobileActorFlag(MobileActorFlag::Werewolf);
	}

	void MobileActor::setFlagWerewolf(bool value) {
		setMobileActorFlag(MobileActorFlag::Werewolf, value);
	}

	Statistic* MobileActor::getAttributeAgility() {
		return &attributes[TES3::Attribute::Agility];
	}

	Statistic* MobileActor::getAttributeEndurance() {
		return &attributes[TES3::Attribute::Endurance];
	}

	Statistic* MobileActor::getAttributeIntelligence() {
		return &attributes[TES3::Attribute::Intelligence];
	}

	Statistic* MobileActor::getAttributeLuck() {
		return &attributes[TES3::Attribute::Luck];
	}

	Statistic* MobileActor::getAttributePersonality() {
		return &attributes[TES3::Attribute::Personality];
	}

	Statistic* MobileActor::getAttributeSpeed() {
		return &attributes[TES3::Attribute::Speed];
	}

	Statistic* MobileActor::getAttributeStrength() {
		return &attributes[TES3::Attribute::Strength];
	}

	Statistic* MobileActor::getAttributeWillpower() {
		return &attributes[TES3::Attribute::Willpower];
	}

	int MobileActor::getEffectAttributeAttackBonus() const {
		return effectAttributes[EffectAttribute::AttackBonus];
	}

	void MobileActor::setEffectAttributeAttackBonus(int value) {
		effectAttributes[EffectAttribute::AttackBonus] = value;
	}

	int MobileActor::getEffectAttributeBlind() const {
		return effectAttributes[EffectAttribute::Blind];
	}

	void MobileActor::setEffectAttributeBlind(int value) {
		effectAttributes[EffectAttribute::Blind] = value;
	}

	int MobileActor::getEffectAttributeChameleon() const {
		return effectAttributes[EffectAttribute::Chameleon];
	}

	void MobileActor::setEffectAttributeChameleon(int value) {
		effectAttributes[EffectAttribute::Chameleon] = value;
	}

	int MobileActor::getEffectAttributeInvisibility() const {
		return effectAttributes[EffectAttribute::Invisibility];
	}

	void MobileActor::setEffectAttributeInvisibility(int value) {
		effectAttributes[EffectAttribute::Invisibility] = value;
	}

	int MobileActor::getEffectAttributeJump() const {
		return effectAttributes[EffectAttribute::Jump];
	}

	void MobileActor::setEffectAttributeJump(int value) {
		effectAttributes[EffectAttribute::Jump] = value;
	}

	int MobileActor::getEffectAttributeLevitate() const {
		return effectAttributes[EffectAttribute::Levitate];
	}

	void MobileActor::setEffectAttributeLevitate(int value) {
		effectAttributes[EffectAttribute::Levitate] = value;
	}

	int MobileActor::getEffectAttributeParalyze() const {
		return effectAttributes[EffectAttribute::Paralyze];
	}

	void MobileActor::setEffectAttributeParalyze(int value) {
		effectAttributes[EffectAttribute::Paralyze] = value;
	}

	int MobileActor::getEffectAttributeResistBlightDisease() const {
		return effectAttributes[EffectAttribute::ResistBlightDisease];
	}

	void MobileActor::setEffectAttributeResistBlightDisease(int value) {
		effectAttributes[EffectAttribute::ResistBlightDisease] = value;
	}

	int MobileActor::getEffectAttributeResistCommonDisease() const {
		return effectAttributes[EffectAttribute::ResistCommonDisease];
	}

	void MobileActor::setEffectAttributeResistCommonDisease(int value) {
		effectAttributes[EffectAttribute::ResistCommonDisease] = value;
	}

	int MobileActor::getEffectAttributeResistCorprus() const {
		return effectAttributes[EffectAttribute::ResistCorprus];
	}

	void MobileActor::setEffectAttributeResistCorprus(int value) {
		effectAttributes[EffectAttribute::ResistCorprus] = value;
	}

	int MobileActor::getEffectAttributeResistFire() const {
		return effectAttributes[EffectAttribute::ResistFire];
	}

	void MobileActor::setEffectAttributeResistFire(int value) {
		effectAttributes[EffectAttribute::ResistFire] = value;
	}

	int MobileActor::getEffectAttributeResistFrost() const {
		return effectAttributes[EffectAttribute::ResistFrost];
	}

	void MobileActor::setEffectAttributeResistFrost(int value) {
		effectAttributes[EffectAttribute::ResistFrost] = value;
	}

	int MobileActor::getEffectAttributeResistMagicka() const {
		return effectAttributes[EffectAttribute::ResistMagicka];
	}

	void MobileActor::setEffectAttributeResistMagicka(int value) {
		effectAttributes[EffectAttribute::ResistMagicka] = value;
	}

	int MobileActor::getEffectAttributeResistNormalWeapons() const {
		return effectAttributes[EffectAttribute::ResistNormalWeapons];
	}

	void MobileActor::setEffectAttributeResistNormalWeapons(int value) {
		effectAttributes[EffectAttribute::ResistNormalWeapons] = value;
	}

	int MobileActor::getEffectAttributeResistParalysis() const {
		return effectAttributes[EffectAttribute::ResistParalysis];
	}

	void MobileActor::setEffectAttributeResistParalysis(int value) {
		effectAttributes[EffectAttribute::ResistParalysis] = value;
	}

	int MobileActor::getEffectAttributeResistPoison() const {
		return effectAttributes[EffectAttribute::ResistPoison];
	}

	void MobileActor::setEffectAttributeResistPoison(int value) {
		effectAttributes[EffectAttribute::ResistPoison] = value;
	}

	int MobileActor::getEffectAttributeResistShock() const {
		return effectAttributes[EffectAttribute::ResistShock];
	}

	void MobileActor::setEffectAttributeResistShock(int value) {
		effectAttributes[EffectAttribute::ResistShock] = value;
	}

	int MobileActor::getEffectAttributeSanctuary() const {
		return effectAttributes[EffectAttribute::Sanctuary];
	}

	void MobileActor::setEffectAttributeSanctuary(int value) {
		effectAttributes[EffectAttribute::Sanctuary] = value;
	}

	int MobileActor::getEffectAttributeShield() const {
		return effectAttributes[EffectAttribute::Shield];
	}

	void MobileActor::setEffectAttributeShield(int value) {
		effectAttributes[EffectAttribute::Shield] = value;
	}

	int MobileActor::getEffectAttributeSilence() const {
		return effectAttributes[EffectAttribute::Silence];
	}

	void MobileActor::setEffectAttributeSilence(int value) {
		effectAttributes[EffectAttribute::Silence] = value;
	}

	int MobileActor::getEffectAttributeSound() const {
		return effectAttributes[EffectAttribute::Sound];
	}

	void MobileActor::setEffectAttributeSound(int value) {
		effectAttributes[EffectAttribute::Sound] = value;
	}

	int MobileActor::getEffectAttributeSwiftSwim() const {
		return effectAttributes[EffectAttribute::SwiftSwim];
	}

	void MobileActor::setEffectAttributeSwiftSwim(int value) {
		effectAttributes[EffectAttribute::SwiftSwim] = value;
	}

	int MobileActor::getEffectAttributeWaterBreathing() const {
		return effectAttributes[EffectAttribute::WaterBreathing];
	}

	void MobileActor::setEffectAttributeWaterBreathing(int value) {
		effectAttributes[EffectAttribute::WaterBreathing] = value;
	}

	int MobileActor::getEffectAttributeWaterWalking() const {
		return effectAttributes[EffectAttribute::WaterWalking];
	}

	void MobileActor::setEffectAttributeWaterWalking(int value) {
		effectAttributes[EffectAttribute::WaterWalking] = value;
	}

	bool MobileActor::getMovementFlagFlying() const {
		return getMobileActorMovementFlag(ActorMovement::Flying);
	}

	void MobileActor::setMovementFlagFlying(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Flying, value);
	}

	bool MobileActor::getMovementFlagJumping() const {
		return getMobileActorMovementFlag(ActorMovement::Jumping);
	}

	void MobileActor::setMovementFlagJumping(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Jumping, value);
	}

	bool MobileActor::getMovementFlagBack() const {
		return getMobileActorMovementFlag(ActorMovement::Back);
	}

	void MobileActor::setMovementFlagBack(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Back, value);
	}

	bool MobileActor::getMovementFlagForward() const {
		return getMobileActorMovementFlag(ActorMovement::Forward);
	}

	void MobileActor::setMovementFlagForward(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Forward, value);
	}

	bool MobileActor::getMovementFlagLeft() const {
		return getMobileActorMovementFlag(ActorMovement::Left);
	}

	void MobileActor::setMovementFlagLeft(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Left, value);
	}

	bool MobileActor::getMovementFlagRight() const {
		return getMobileActorMovementFlag(ActorMovement::Right);
	}

	void MobileActor::setMovementFlagRight(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Right, value);
	}

	bool MobileActor::getMovementFlagRunning() const {
		return getMobileActorMovementFlag(ActorMovement::Running);
	}

	void MobileActor::setMovementFlagRunning(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Running, value);
	}

	bool MobileActor::getMovementFlagSneaking() const {
		return getMobileActorMovementFlag(ActorMovement::Sneaking);
	}

	void MobileActor::setMovementFlagSneaking(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Sneaking, value);
	}

	bool MobileActor::getMovementFlagFalling() const {
		return getMobileActorMovementFlag(ActorMovement::Falling);
	}

	void MobileActor::setMovementFlagFalling(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Falling, value);
	}

	bool MobileActor::getMovementFlagSwimming() const {
		return getMobileActorMovementFlag(ActorMovement::Swimming);
	}

	void MobileActor::setMovementFlagSwimming(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Swimming, value);
	}

	bool MobileActor::getMovementFlagTurnLeft() const {
		return getMobileActorMovementFlag(ActorMovement::TurnLeft);
	}

	void MobileActor::setMovementFlagTurnLeft(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::TurnLeft, value);
	}

	bool MobileActor::getMovementFlagTurnRight() const {
		return getMobileActorMovementFlag(ActorMovement::TurnRight);
	}

	void MobileActor::setMovementFlagTurnRight(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::TurnRight, value);
	}

	bool MobileActor::getMovementFlagWalking() const {
		return getMobileActorMovementFlag(ActorMovement::Walking);
	}

	void MobileActor::setMovementFlagWalking(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Walking, value);
	}

	bool MobileActor::isAffectedByObject_lua(sol::object object) const {
		if (object.is<TES3::Alchemy>()) {
			return isAffectedByAlchemy(object.as<TES3::Alchemy*>());
		}
		else if (object.is<TES3::Enchantment>()) {
			return isAffectedByEnchantment(object.as<TES3::Enchantment*>());
		}
		else if (object.is<TES3::Spell>()) {
			return isAffectedBySpell(object.as<TES3::Spell*>());
		}

		return false;
	}

	bool MobileActor::hasSummonEffect() {
		for (auto& activeEffect : activeMagicEffects) {
			if (activeEffect.isSummon) {
				return true;
			}
		}
		return false;
	}

	bool MobileActor::hasUsedPower(Spell* power) const {
		return powers.containsKey(power);
	}

	bool MobileActor::rechargePower(Spell* power) {
		return powers.eraseKey(power);
	}

	sol::optional<double> MobileActor::getPowerUseTimestamp(Spell* power) const {
		auto node = powers.getNode(power);
		if (!node) {
			return {};
		}

		return node->value;
	}

	void MobileActor::setPowerUseTimestamp(Spell* power, double timestamp) {
		powers.addKey(power, timestamp);
	}

	bool MobileActor::getMobToMobCollision() const {
		if (actorFlags & TES3::MobileActorFlag::ActiveInSimulation) {
			auto mobController = TES3::WorldController::get()->mobController;
			return mobController->hasMobileCollision(this);
		}
		return false;
	}

	void MobileActor::setMobToMobCollision(bool collide) {
		if (actorFlags & TES3::MobileActorFlag::ActiveInSimulation) {
			auto mobController = TES3::WorldController::get()->mobController;
			if (collide) {
				mobController->enableMobileCollision(this);
			}
			else {
				mobController->disableMobileCollision(this);
			}
		}
	}

	sol::table MobileActor::getActiveMagicEffectsList_lua(sol::table params) {
		sol::optional<int> effectID = params["effect"];
		sol::optional<unsigned int> serial = params["serial"];

		auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		sol::table effectsList = stateHandle.state.create_table();
		int index = 1;

		for (auto& activeEffect : activeMagicEffects) {
			if (effectID && effectID.value() != activeEffect.magicEffectID) {
				continue;
			}
			if (serial && serial.value() != activeEffect.magicInstanceSerial) {
				continue;
			}

			effectsList[index] = ActiveMagicEffectLua(activeEffect, this);
			index++;
		}

		return effectsList;
	}

	ActiveMagicEffect* MobileActor::getActiveMagicEffects_legacy() const {
		return &activeMagicEffects.sentinel->data;
	}

	int MobileActor::getActiveMagicEffectCount_legacy() const {
		return activeMagicEffects.size();
	}

}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3_MOBILEOBJECT(TES3::MobileActor)
