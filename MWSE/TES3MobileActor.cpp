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
#include "LuaEquipEvent.h"
#include "LuaEquippedEvent.h"
#include "LuaJumpEvent.h"
#include "LuaMobileObjectCollisionEvent.h"

#include "TES3Actor.h"
#include "TES3ActorAnimationController.h"
#include "TES3Alchemy.h"
#include "TES3AudioController.h"
#include "TES3Cell.h"
#include "TES3CombatSession.h"
#include "TES3Enchantment.h"
#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3MagicEffectController.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicInstanceController.h"
#include "TES3MobManager.h"
#include "TES3MobilePlayer.h"
#include "TES3ItemData.h"
#include "TES3Race.h"
#include "TES3Spell.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"
#include "TES3UIManager.h"

#include "TES3Util.h"

namespace TES3 {
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

	MagicEffectInstance* ActiveMagicEffectLua::getEffectInstance() const {
		auto magicInstance = getInstance();
		if (magicInstance) {
			return magicInstance->getEffectInstance(magicInstanceEffectIndex, mobile->reference);
		}
		return nullptr;
	}

	bool ActiveMagicEffectLua::isBoundItem() const {
		auto effectInstance = getEffectInstance();
		return effectInstance ? effectInstance->isBoundItem() : false;
	}

	bool ActiveMagicEffectLua::isSummon() const {
		auto effectInstance = getEffectInstance();
		return effectInstance ? effectInstance->isSummon() : false;
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

	SkillStatistic* MobileActor::getSkillStatistic(int skillId) {
		return vTable.mobileActor->getSkillStatistic(this, skillId);
	}

	float MobileActor::getSkillValue(int skillId) const {
		return vTable.mobileActor->getSkillValue(this, skillId);
	}

	float MobileActor::applyArmorRating(float damage, float swing, bool damageEquipment) {
		return vTable.mobileActor->applyArmorRating(this, damage, swing, damageEquipment);
	}

	float MobileActor::calculateArmorRating(int* armorItemCount) const {
		return vTable.mobileActor->calculateArmorRating(this, armorItemCount);
	}

	float MobileActor::getArmorRating_lua() const {
		return calculateArmorRating(nullptr);
	}

	AnimationData* MobileActor::getAnimationData() const {
		return vTable.mobileActor->getAnimationAttachment(this);
	}

	const auto TES3_MobileActor_applyPhysicalHit = reinterpret_cast<void(__thiscall*)(MobileActor*, MobileActor*, MobileActor*, float, float, MobileProjectile*, bool)>(0x5568F0);
	void MobileActor::applyPhysicalHit(MobileActor* attacker, MobileActor* defender, float damage, float swing, MobileProjectile* projectile, bool alwaysPlayHitVoice) {
		// Setup damage event data.
		mwse::lua::event::DamageEvent::m_Attacker = attacker;
		mwse::lua::event::DamageEvent::m_Projectile = projectile;

		// Call original function.
		TES3_MobileActor_applyPhysicalHit(this, attacker, defender, damage, swing, projectile, alwaysPlayHitVoice);

		// Clean up damage event data.
		mwse::lua::event::DamageEvent::m_Attacker = nullptr;
		mwse::lua::event::DamageEvent::m_Projectile = nullptr;
	}

	const auto TES3_MobileActor_setCurrentMagicFromSpell = reinterpret_cast<void(__thiscall*)(MobileActor*, Spell*)>(0x52B390);
	void MobileActor::setCurrentMagicFromSpell(Spell* spell) {
		TES3_MobileActor_setCurrentMagicFromSpell(this, spell);
	}

	const auto TES3_MobileActor_setCurrentMagicFromSourceCombo = reinterpret_cast<void(__thiscall*)(MobileActor*, MagicSourceCombo)>(0x52B220);
	void MobileActor::setCurrentMagicFromSourceCombo(MagicSourceCombo sourceCombo) {
		TES3_MobileActor_setCurrentMagicFromSourceCombo(this, sourceCombo);
	}

	const auto TES3_MobileActor_setCurrentMagicFromEquipmentStack = reinterpret_cast<void(__thiscall*)(MobileActor*, EquipmentStack*)>(0x52B2A0);
	void MobileActor::setCurrentMagicFromEquipmentStack(EquipmentStack* equipmentStack) {
		TES3_MobileActor_setCurrentMagicFromEquipmentStack(this, equipmentStack);
	}

	void MobileActor::removeFiredProjectiles(bool includeSpellProjectiles) {
		TES3::WorldController::get()->mobManager->projectileManager->removeProjectilesFiredByActor(this, includeSpellProjectiles);
	}

	const auto TES3_MobileActor_resurrect = reinterpret_cast<void(__thiscall*)(MobileActor*, bool)>(0x529AF0);
	void MobileActor::resurrect(bool resetState, bool moveToStartingLocation) {
		if (resetState) {
			// Original function will resurrect, but also reset the stats, inventory, and reference of non-players.
			TES3_MobileActor_resurrect(this, moveToStartingLocation);
		}
		else if (this->isDead()) {
			// Custom resurrect logic that revives with minimal state changes.
			actionData.animStateAttack = AttackAnimationState::Idle;
			actionData.aiBehaviorState = 0;
			actionData.animGroupCurrentAction = 0xFF;
			actorFlags &= ~(MobileActorFlag::SpellReadied | MobileActorFlag::WeaponDrawn);
			movementFlags = 0;

			health.current = this->health.base;
			magicka.current = this->magicka.base;
			fatigue.current = this->fatigue.base;
			magickaMultiplier.current = this->magickaMultiplier.base;

			vTable.mobileActor->setupCollision(this);
			vTable.mobileActor->handleMovementCollision(this, true);
			WorldController::get()->mobManager->addMob(reference);
		}

		// Reset orientation that may have been altered post-death by mods.
		reference->orientation = Vector3();

		enterLeaveSimulationByDistance();
	}

	void MobileActor::resurrect_lua(sol::table params) {
		bool resetState = params.get_or("resetState", true);
		bool moveToStartingLocation = params.get_or("moveToStartingLocation", false);
		resurrect(resetState, moveToStartingLocation);
	}

	using gMaxHeadTrackingDistance = mwse::ExternalGlobal<float, 0x7C8784>;

	void MobileActor::overrideHeadTrackingThisFrame(Reference* target) {
		const auto animController = animationController.asActor;

		if (target) {
			if (isActive() && animController && reference->isInSameWorldspace(target)) {
				// Allow overrides to acquire a track target at any distance.
				float prevMaxHeadTrackingDistance = gMaxHeadTrackingDistance::get();
				gMaxHeadTrackingDistance::set(std::numeric_limits<float>::max());

				// Call vanilla code to set head target angle, including angle constraints.
				animController->animationData->headTracking(reference, target);
				// Set track distance to zero so that the AI doesn't examine other targets to track.
				animController->animationData->headLookClosestDistance = 0;

				// Restore global data.
				gMaxHeadTrackingDistance::set(prevMaxHeadTrackingDistance);
			}
		}
		else {
			// Reset head tracking distance to allow AI to control it.
			if (animController) {
				animController->animationData->headLookClosestDistance = std::numeric_limits<float>::max();
			}
		}
	}

	bool MobileActor::equipMagic(Object* source, ItemData* itemData, bool equipItem, bool updateGUI) {
		if (!source) {
			throw std::invalid_argument("Invalid 'source' parameter provided. Must not be nil.");
		}

		updateGUI = updateGUI && actorType == MobileActorType::Player;

		// Reset currently set spell and enchanted item first.
		unequipMagic(false, updateGUI);

		if (source->objectType == ObjectType::Spell) {
			Spell* spell = static_cast<Spell*>(source);

			// Ignore spell that can not be casted.
			if (spell->castType != SpellCastType::Spell && spell->castType != SpellCastType::Power) {
				throw std::invalid_argument("Invalid 'source' parameter provided. Spell must be castable.");
			}

			// Equip the spell.
			setCurrentMagicFromSpell(spell);

			if (updateGUI) {
				// Get the icon path of the spell's first effect.
				const char* iconPath = "";
				Effect* effects = spell->effects;
				if (effects) {
					TES3::MagicEffect* effect = DataHandler::get()->nonDynamicData->getMagicEffect(effects[0].effectID);
					if (effect) {
						iconPath = effect->icon;
					}
				}

				// Update the GUI.
				UI::updateCurrentMagicFromSpell(iconPath, spell->getName(), spell);
				UI::updateMagicMenuSelection();
			}
		}
		else if (source->isItem()) {
			auto item = static_cast<Item*>(source);
			EquipmentStack equipmentStack;
			EquipmentStack* equipmentStackPointer = &equipmentStack;

			// Ignore items without enchantments that can be casted.
			auto enchantment = item->getEnchantment();
			if (!enchantment || (enchantment->castType != EnchantmentCastType::OnUse && enchantment->castType != EnchantmentCastType::Once)) {
				throw std::invalid_argument("Invalid 'source' parameter provided. Item must have a castable enchantment.");
			}

			if (item->objectType == ObjectType::Book)
			{
				// Create a basic equipment stack for enchanted scrolls.
				equipmentStack.object = source;
			}
			else {
				if (equipItem) {
					Actor* actor = static_cast<Actor*>(reference->baseObject);

					// Check if the actor possesses the desired item.
					ItemStack* stack = actor->inventory.findItemStack(item);
					if (!stack) {
						throw std::runtime_error("Actor must possess item if 'equipItem' is true.");
					}

					// Get the equipment stack from the actor.
					if (itemData) {
						equipmentStackPointer = actor->getEquippedItemExact(item, itemData);
					}
					else {
						equipmentStackPointer = actor->getEquippedItem(item);
					}


					if (!equipmentStackPointer) {
						// Equip the item if desired.
						this->equipItem(item, itemData);
						equipmentStackPointer = actor->getEquippedItem(item);

						// Return if the item could not be equipped.
						if (!equipmentStackPointer) {
							return false;
						}
					}
				}
				else {
					if (!itemData) {
						throw std::invalid_argument("Invalid 'itemData' parameter provided. Must not be nil for equippable items if 'equipItem' is false.");
					}

					// Create an equipment stack for items that should not be equipped automatically.
					equipmentStack.object = source;
					equipmentStack.itemData = itemData;
				}
			}

			// Equip the item enchantment.
			setCurrentMagicFromEquipmentStack(equipmentStackPointer);

			// Update the GUI if desired.
			if (updateGUI) {
				UI::updateCurrentMagicFromEquipmentStack(nullptr, nullptr, equipmentStackPointer);
				UI::updateMagicMenuSelection();
			}
		}
		else {
			throw std::invalid_argument("Invalid 'source' parameter provided. Must be a spell or item.");
		}

		return true;
	}

	void MobileActor::unequipMagic(bool unequipItem, bool updateGUI) {
		// Unequip the item of the currently equipped magic if desired.
		if (unequipItem && currentEnchantedItem.object) {
			Actor* actor = static_cast<Actor*>(reference->baseObject);
			if (actor->getEquippedItem(currentEnchantedItem.object)) {
				actor->unequipItem(currentEnchantedItem.object, true, this, false, currentEnchantedItem.itemData);
				actor->postUnequipUIRefresh(this);
			}
		}

		// Remove currently set spell and enchanted item.
		setCurrentMagicFromSourceCombo(nullptr);
		setCurrentMagicFromEquipmentStack(nullptr);

		// Update the GUI if desired.
		if (updateGUI && actorType == MobileActorType::Player) {
			UI::updateCurrentMagicFromSpell("", "", nullptr);
			UI::updateMagicMenuSelection();
		}
	}

	const auto TES3_MobileActor_getCell = reinterpret_cast<Cell* (__thiscall*)(const MobileActor*)>(0x521630);
	Cell* MobileActor::getCell() const {
		return TES3_MobileActor_getCell(this);
	}

	const auto TES3_MobileActor_getFatigueTerm = reinterpret_cast<float(__thiscall*)(const MobileActor*)>(0x527610);
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

	float MobileActor::getViewToPoint_lua(sol::object point) const {
		if (!point.is<const Vector3*>()) {
			throw std::invalid_argument("Invalid 'point' parameter.");
		}
		return getViewToPoint(point.as<const Vector3*>());
	}

	const auto TES3_MobileActor_getViewToPointWithFacing = reinterpret_cast<float(__thiscall*)(const MobileActor*, float, const Vector3*)>(0x5264C0);
	float MobileActor::getViewToPointWithFacing(float facing, const Vector3* point) const {
		return TES3_MobileActor_getViewToPointWithFacing(this, facing, point);
	}

	float MobileActor::getViewToPointWithFacing_lua(float facing, sol::object point) const {
		if (!point.is<const Vector3*>()) {
			throw std::invalid_argument("Invalid 'point' parameter.");
		}
		return getViewToPointWithFacing(facing, point.as<const Vector3*>());
	}

	float MobileActor::getViewToActor(const TES3::MobileActor* mobile) const {
		return getViewToPointWithFacing(getFacing(), &mobile->reference->position);
	}

	float MobileActor::getViewToActor_lua(sol::object mobile) const {
		if (!mobile.is<const TES3::MobileActor*>()) {
			throw std::invalid_argument("Invalid 'mobile' parameter.");
		}
		return getViewToActor(mobile.as<const TES3::MobileActor*>());
	}

	const auto TES3_MobileActor_getBootsWeight = reinterpret_cast<float(__thiscall*)(const MobileActor*)>(0x526F30);
	float MobileActor::getBootsWeight() const {
		return TES3_MobileActor_getBootsWeight(this);
	}

	const auto TES3_MobileActor_getWeaponSpeed = reinterpret_cast<float(__thiscall*)(const MobileActor*)>(0x556580);
	float MobileActor::getWeaponSpeed() const {
		return TES3_MobileActor_getWeaponSpeed(this);
	}

	const auto TES3_MobileActor_getAttackReach = reinterpret_cast<float(__thiscall*)(const MobileActor*)>(0x5584C0);
	float MobileActor::getAttackReach() const {
		return TES3_MobileActor_getAttackReach(this);
	}

	const auto TES3_MobileActor_getEffectiveAttackDistance = reinterpret_cast<float(__cdecl*)(const MobileActor*, const MobileActor*, bool)>(0x53ADD0);
	float MobileActor::getEffectiveAttackDistance(const TES3::MobileActor* mobile) const {
		return TES3_MobileActor_getEffectiveAttackDistance(this, mobile, true);
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

	void MobileActor::startCombat_lua(sol::object target) {
		if (target.is<MobileActor>()) {
			auto targetActor = target.as<MobileActor*>();

			// Make sure that disabled or dead NPCs can't start combat.
			if (!getFlagActiveAI() || isDead()) {
				return;
			}

			// Make sure that disabled or dead NPCs can't be the target of combat.
			if (targetActor->reference->getDisabled() || !targetActor->getFlagActiveAI() || targetActor->isDead()) {
				return;
			}

			startCombat(targetActor);
			return;
		}
		throw std::invalid_argument("Invalid 'target' parameter provided. Must be a tes3mobileActor.");
	}

	const auto TES3_MobileActor_stopCombat = reinterpret_cast<void(__thiscall*)(MobileActor*, bool)>(0x558720);
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
		TES3_MobileActor_stopCombat(this, something);

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

	const auto TES3_MobileActor_onDeath = reinterpret_cast<void(__thiscall*)(MobileActor*)>(0x523AA0);
	void MobileActor::onDeath() {
		TES3_MobileActor_onDeath(this);

		// Trigger death event.
		if (mwse::lua::event::DeathEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::DeathEvent(this));
		}
	}

	void MobileActor::kill() {
		health.setCurrentCapped(0.0f, false);
	}

	const auto TES3_MobileActor_notifyActorDeadOrDestroyed = reinterpret_cast<void(__thiscall*)(MobileActor*, MobileActor*)>(0x51FEB0);
	void MobileActor::notifyActorDeadOrDestroyed(MobileActor* mobileActor) {
		TES3_MobileActor_notifyActorDeadOrDestroyed(this, mobileActor);
	}

	const auto TES3_MobileActor_retireMagic = reinterpret_cast<void(__thiscall*)(MobileActor*)>(0x52C990);
	void MobileActor::retireMagic() {
		TES3_MobileActor_retireMagic(this);
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

		// Call original function.
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

	const auto TES3_MobileActor_applyFatigueDamage = reinterpret_cast<float(__thiscall*)(MobileActor*, float, float, bool)>(0x5581B0);
	float MobileActor::applyFatigueDamage(float fatigueDamage, float swing, bool alwaysPlayHitVoice) {
		bool miss = fatigueDamage <= 0.001f;

		// Invoke our pre-damage event and check if it is blocked.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		if (!miss && mwse::lua::event::DamageHandToHandEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::DamageHandToHandEvent(this, fatigueDamage));
			if (eventData.valid()) {
				if (eventData.get_or("block", false)) {
					return 0;
				}

				fatigueDamage = eventData["fatigueDamage"];
			}
		}

		// Call original function. Should run on both hits and misses.
		float result = TES3_MobileActor_applyFatigueDamage(this, fatigueDamage, swing, alwaysPlayHitVoice);

		// Do our post-damage event.
		if (!miss && mwse::lua::event::DamagedHandToHandEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::DamagedHandToHandEvent(this, fatigueDamage));
		}
		return result;
	}

	float MobileActor::applyDamage_lua(sol::table params) {
		auto applyArmor = params.get_or("applyArmor", false);
		auto applyDifficulty = params.get_or("applyDifficulty", false);
		sol::optional<float> damage = params["damage"];
		auto doNotChangeHealth = params.get_or("doNotChangeHealth", false);
		sol::optional<bool> playerAttack = params["playerAttack"];
		auto resistIndex = params.get_or("resistAttribute", -1);

		if (applyDifficulty && !playerAttack) {
			throw std::invalid_argument("'applyDifficulty' requires a 'playerAttack' parameter.");
		}
		if (!damage) {
			throw std::invalid_argument("Invalid 'damage' parameter provided.");
		}

		auto adjustedDamage = damage.value();

		// Apply armor mitigation. Includes damaging armor condition, hit sounds, and player armor skill experience.
		if (applyArmor) {
			adjustedDamage = applyArmorRating(adjustedDamage, 1.0f, true);
		}

		// Effect attribute based resistance/weakness.
		if (resistIndex >= TES3::MagicEffectAttribute::AttackBonus && resistIndex <= TES3::MagicEffectAttribute::Invisibility) {
			adjustedDamage *= std::max(0, 100 - effectAttributes[resistIndex]) / 100.0f;
		}

		auto prevSource = mwse::lua::event::DamageEvent::m_Source;
		mwse::lua::event::DamageEvent::m_Source = "script";
		applyHealthDamage(adjustedDamage, playerAttack.value_or(false), applyDifficulty, doNotChangeHealth);
		mwse::lua::event::DamageEvent::m_Source = prevSource;

		return adjustedDamage;
	}

	float MobileActor::applyFatigueDamage_lua(float fatigueDamage, float swing, bool alwaysPlayHitVoice) {
		auto prevSource = mwse::lua::event::DamageHandToHandEvent::m_Source;
		mwse::lua::event::DamageHandToHandEvent::m_Source = "script";
		float result = applyFatigueDamage(fatigueDamage, swing, alwaysPlayHitVoice);
		mwse::lua::event::DamageHandToHandEvent::m_Source = prevSource;

		return result;
	}

	float MobileActor::calcEffectiveDamage_lua(sol::table params) {
		auto applyArmor = params.get_or("applyArmor", false);
		sol::optional<float> damage = params["damage"];
		auto resistIndex = params.get_or("resistAttribute", -1);

		if (!damage) {
			throw std::invalid_argument("Invalid 'damage' parameter provided.");
		}

		// Emulate armor mitigation.
		auto adjustedDamage = damage.value();
		if (applyArmor) {
			if (adjustedDamage > 0.001f) {
				auto fCombatArmorMinMult = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fCombatArmorMinMult]->value.asFloat;
				auto armor = calculateArmorRating();
				auto reducer = std::max(fCombatArmorMinMult, adjustedDamage / (armor + adjustedDamage));
				adjustedDamage = std::max(1.0f, reducer * adjustedDamage);
			}
			else {
				adjustedDamage = 0;
			}
		}

		// Effect attribute based resistance/weakness.
		if (resistIndex >= TES3::MagicEffectAttribute::AttackBonus && resistIndex <= TES3::MagicEffectAttribute::Invisibility) {
			adjustedDamage *= std::max(0, 100 - effectAttributes[resistIndex]) / 100.0f;
		}

		return adjustedDamage;
	}

	const auto TES3_MobileActor_applyJumpFatigueCost = reinterpret_cast<float(__thiscall*)(const MobileActor*)>(0x527A60);
	void MobileActor::applyJumpFatigueCost() const {
		TES3_MobileActor_applyJumpFatigueCost(this);
	}

	bool MobileActor::doJump(Vector3 velocity, bool applyFatigueCost, bool isDefaultJump) {
		// Allow the event to override the velocity or block the jump.
		if (mwse::lua::event::JumpEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::JumpEvent(this, velocity, applyFatigueCost, isDefaultJump));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				if (eventData.get_or("block", false)) {
					return false;
				}
				velocity = mwse::lua::getOptionalParam<Vector3>(eventData, "velocity", velocity);
				applyFatigueCost = mwse::lua::getOptionalParam<bool>(eventData, "applyFatigueCost", applyFatigueCost);
			}
		}

		// Execute the actual jumping logic.
		Vector3 zeroVector;
		setInstantVelocity(&zeroVector);
		updateConstantVelocity(&velocity);
		vTable.mobileActor->setJumping(this, true);
		if (applyFatigueCost) {
			applyJumpFatigueCost();
		}
		return true;
	}

	bool MobileActor::doJump_lua(sol::optional<sol::table> params) {
		sol::optional<Vector3> velocity = mwse::lua::getOptionalParamVector3(params, "velocity");
		bool applyFatigueCost = mwse::lua::getOptionalParam(params, "applyFatigueCost", true);
		bool allowMidairJumping = mwse::lua::getOptionalParam(params, "allowMidairJumping", false);
		bool isDefaultJump = false;

		// Prevent jumping if it would break logic.
		if (!canJump(allowMidairJumping)) {
			return false;
		}

		// Get the default jumping velocity if none has been provided.
		bool isVelocityValid = velocity.has_value();
		if (!isVelocityValid) {
			Vector2 zeroVector;
			velocity = calculateJumpVelocity(zeroVector);
			isDefaultJump = applyFatigueCost && !allowMidairJumping;
		}

		return doJump(velocity.value(), applyFatigueCost, isDefaultJump);
	}

	const auto TES3_MobileActor_isNotKnockedDownOrOut = reinterpret_cast<bool(__thiscall*)(const MobileActor*)>(0x527580);
	bool MobileActor::isNotKnockedDownOrOut() const {
		return TES3_MobileActor_isNotKnockedDownOrOut(this);
	}

	const auto TES3_MobileActor_isKnockedDown = reinterpret_cast<bool(__thiscall*)(const MobileActor*)>(0x5275D0);
	bool MobileActor::isKnockedDown() const {
		return TES3_MobileActor_isKnockedDown(this);
	}

	const auto TES3_MobileActor_isKnockedOut = reinterpret_cast<bool(__thiscall*)(const MobileActor*)>(0x5275F0);
	bool MobileActor::isKnockedOut() const {
		return TES3_MobileActor_isKnockedOut(this);
	}

	const auto TES3_MobileActor_isAttackingOrCasting = reinterpret_cast<bool(__thiscall*)(const MobileActor*)>(0x5567D0);
	bool MobileActor::isAttackingOrCasting() const {
		return TES3_MobileActor_isAttackingOrCasting(this);
	}

	bool MobileActor::isReadyingWeapon() const {
		return actionData.animStateAttack == AttackAnimationState::ReadyingWeap ||
			actionData.animStateAttack == AttackAnimationState::UnreadyWeap;
	}

	bool MobileActor::isParalyzed() const {
		return getEffectAttributeParalyze() > 0;
	}

	bool MobileActor::isHitStunned() const {
		if (animationController.asActor) {
			auto animData = animationController.asActor->animationData;
			if (animData) {
				// Check for hit stun animations.
				const unsigned char hit1 = 19, swimHit3 = 26;
				auto bodyAnimGroup = animData->currentAnimGroup[0];
				return bodyAnimGroup >= hit1 && bodyAnimGroup <= swimHit3;
			}
		}
		return false;
	}

	bool MobileActor::canAct() const {
		return !isDead() &&
			isNotKnockedDownOrOut() &&
			!isAttackingOrCasting() &&
			!isParalyzed() &&
			!isHitStunned() &&
			!isReadyingWeapon();
	}

	bool MobileActor::canMove() const {
		return !isDead() &&
			isNotKnockedDownOrOut() &&
			!isParalyzed() &&
			!isHitStunned();
	}

	bool MobileActor::canJump(bool allowMidairJumping) const {
		return WorldController::get()->collisionEnabled &&
			!isDead() &&
			isNotKnockedDownOrOut() &&
			!isParalyzed() &&
			!getMovementFlagSwimming() &&
			!getMovementFlagFlying() &&
			(allowMidairJumping || (!getMovementFlagJumping() && !getMovementFlagFalling())) &&
			(!getMovementFlagSliding() || thisFrameDeltaPosition.length() <= 0.0099999998);
	}

	bool MobileActor::canJump_lua() const {
		return canJump(false);
	}

	bool MobileActor::canJumpMidair_lua() const {
		return canJump(true);
	}

	const auto TES3_MobileActor_calculateRunSpeed = reinterpret_cast<float(__thiscall*)(MobileActor*)>(0x527050);
	float MobileActor::calculateRunSpeed() {
		// Call the original function to get the default movement speed value.
		float speed = TES3_MobileActor_calculateRunSpeed(this);

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

	const auto TES3_MobileActor_calculateSwimSpeed = reinterpret_cast<float(__thiscall*)(MobileActor*)>(0x5270B0);
	float MobileActor::calculateSwimSpeed() {
		// Call the original function to get the default movement speed value.
		float speed = TES3_MobileActor_calculateSwimSpeed(this);

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

	const auto TES3_MobileActor_calculateFlySpeed = reinterpret_cast<float(__thiscall*)(MobileActor*)>(0x5271F0);
	float MobileActor::calculateFlySpeed() {
		// Call the original function to get the default movement speed value.
		float speed = TES3_MobileActor_calculateFlySpeed(this);

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

	Vector3 MobileActor::calculateJumpVelocity(Vector2 direction) {
		GameSetting** GMSTs = DataHandler::get()->nonDynamicData->GMSTs;
		float fJumpEncumbranceBase = GMSTs[TES3::GMST::fJumpEncumbranceBase]->value.asFloat;
		float fJumpEncumbranceMultiplier = GMSTs[TES3::GMST::fJumpEncumbranceMultiplier]->value.asFloat;
		float fJumpAcrobaticsBase = GMSTs[TES3::GMST::fJumpAcrobaticsBase]->value.asFloat;
		float fJumpAcroMultiplier = GMSTs[TES3::GMST::fJumpAcroMultiplier]->value.asFloat;
		float fJumpRunMultiplier = GMSTs[TES3::GMST::fJumpRunMultiplier]->value.asFloat;
		float acrobaticsSkill = getSkillValue(TES3::SkillID::Acrobatics);
		int jumpEffectMagnitude = getEffectAttributeJump();

		// Calculate acrobatics jump height coefficients.
		float lowCoefficient = acrobaticsSkill;
		float highCoefficient = 0.0f;
		if (acrobaticsSkill > 50.0f) {
			lowCoefficient = 50.0f;
			highCoefficient = acrobaticsSkill - 50.0f;
		}

		// Calculate the impact of encumbrance on jump height.
		float encumbranceTerm = fJumpEncumbranceBase + fJumpEncumbranceMultiplier * (1 - encumbrance.getNormalized());

		// Calculate the jump height.
		float jumpHeight = fJumpAcrobaticsBase + pow(lowCoefficient / 15.0f, fJumpAcroMultiplier);
		jumpHeight += 3.0f * highCoefficient * fJumpAcroMultiplier;
		jumpHeight += jumpEffectMagnitude * 64;
		jumpHeight *= encumbranceTerm;
		if (getMovementFlagRunning()) {
			jumpHeight *= fJumpRunMultiplier;
		}
		jumpHeight *= getFatigueTerm();
		jumpHeight -= WorldController::get()->mobManager->gravity.z;
		jumpHeight /= 3.0f;

		// Velocity is unavailable outside of the physics loop.
		// For now this is solved by letting the caller provide a direction vector.
		// The code snippet below is the actual logic and velocity that is required to automatically determine the direction.
		/*if (getMovementFlagWalking() || getMovementFlagRunning()) {
			Vector3 localVelocity = reference->sceneNode->getLocalVelocity();
			Vector3 direction = Vector3(localVelocity.x, localVelocity.y, 0).normalized();*/
		if (direction.length() > 0) {
			direction.normalize();
			return Vector3(direction.x, direction.y, 1.0f) * jumpHeight * 0.707f;
		}
		else
		{
			return Vector3(0.0f, 0.0f, jumpHeight);
		}
	}

	Vector3 MobileActor::calculateJumpVelocity_lua(sol::optional<sol::table> params) {
		Vector2 direction = mwse::lua::getOptionalParam<Vector2>(params, "direction", Vector2());
		return calculateJumpVelocity(direction);
	}

	const auto TES3_MobileActor_calcDerivedStats = reinterpret_cast<void(__thiscall*)(const MobileActor*, Statistic*)>(0x527BC0);
	void MobileActor::updateDerivedStatistics(Statistic * baseStatistic) {
		TES3_MobileActor_calcDerivedStats(this, baseStatistic);
	}

	void MobileActor::updateDerivedStatistics_lua(sol::optional<Statistic*> baseStatistic) {
		TES3_MobileActor_calcDerivedStats(this, baseStatistic.value_or(nullptr));
	}

	const auto TES3_MobileActor_determineModifiedPrice = reinterpret_cast<int(__thiscall*)(const MobileActor*, int, int)>(0x52AA50);
	int MobileActor::determineModifiedPrice(int basePrice, bool buying) {
		return TES3_MobileActor_determineModifiedPrice(this, basePrice, buying);
	}

	const auto TES3_MobileActor_playVoiceover = reinterpret_cast<void(__thiscall*)(const MobileActor*, int)>(0x528F80);
	void MobileActor::playVoiceover(int voiceover) {
		TES3_MobileActor_playVoiceover(this, voiceover);
	}

	const auto TES3_MobileActor_startDialogue = reinterpret_cast<void(__thiscall*)(const MobileActor*)>(0x529260);
	void MobileActor::startDialogue() {
		TES3_MobileActor_startDialogue(this);
	}

	const auto TES3_MobileActor_isAffectedByAlchemy = reinterpret_cast<bool(__thiscall*)(const MobileActor*, Alchemy*)>(0x52D1A0);
	bool MobileActor::isAffectedByAlchemy(Alchemy* alchemy) const {
		return TES3_MobileActor_isAffectedByAlchemy(this, alchemy);
	}

	bool MobileActor::isAffectedBySimilarAlchemy(Alchemy* alchemy) const {
		for (auto& ame : activeMagicEffects) {
			auto sourceInstance = ame.getInstance();

			if (sourceInstance && sourceInstance->sourceCombo.sourceType == TES3::MagicSourceType::Alchemy) {
				auto activeAlch = sourceInstance->sourceCombo.source.asAlchemy;

				if (alchemy->effectsMatchWith(activeAlch)
					&& _strnicmp(alchemy->name, activeAlch->name, 32) == 0) {

					return true;
				}
			}
		}
		return false;
	}

	const auto TES3_MobileActor_isAffectedByEnchantment = reinterpret_cast<bool(__thiscall*)(const MobileActor*, Enchantment*)>(0x52D140);
	bool MobileActor::isAffectedByEnchantment(Enchantment* enchantment) const {
		return TES3_MobileActor_isAffectedByEnchantment(this, enchantment);
	}

	const auto TES3_MobileActor_isAffectedBySpell = reinterpret_cast<bool(__thiscall*)(const MobileActor*, Spell*)>(0x52D0E0);
	bool MobileActor::isAffectedBySpell(Spell* spell) const {
		return TES3_MobileActor_isAffectedBySpell(this, spell);
	}

	const auto TES3_MobileActor_isDiseased = reinterpret_cast<bool(__thiscall*)(const MobileActor*)>(0x54DB80);
	bool MobileActor::isDiseased() const {
		return TES3_MobileActor_isDiseased(this);
	}

	bool MobileActor::hasCommonDisease() const {
		for (const auto& effect : activeMagicEffects) {
			const auto instance = effect.getInstance();
			if (instance == nullptr) {
				continue;
			}

			const auto& combo = instance->sourceCombo;
			if (combo.sourceType == MagicSourceType::Spell && combo.source.asSpell->isCommonDisease()) {
				return true;
			}
		}

		return false;
	}

	bool MobileActor::hasBlightDisease() const {
		for (const auto& effect : activeMagicEffects) {
			const auto instance = effect.getInstance();
			if (instance == nullptr) {
				continue;
			}

			const auto& combo = instance->sourceCombo;
			if (combo.sourceType == MagicSourceType::Spell && combo.source.asSpell->isBlightDisease()) {
				return true;
			}
		}

		return false;
	}

	bool MobileActor::hasCorprusDisease() const {
		for (const auto& effect : activeMagicEffects) {
			if (effect.magicEffectID == EffectID::Corprus) {
				return true;
			}
		}

		return false;
	}

	bool MobileActor::hasVampirism() const {
		for (const auto& effect : activeMagicEffects) {
			if (effect.magicEffectID == EffectID::Vampirism) {
				return true;
			}
		}

		return false;
	}

	const auto TES3_MobileActor_getSpellList = reinterpret_cast<SpellList * (__thiscall*)(const MobileActor*)>(0x52B3D0);
	SpellList* MobileActor::getSpellList() {
		return TES3_MobileActor_getSpellList(this);
	}

	const auto TES3_MobileActor_getCombatSpellList = reinterpret_cast<IteratedList<Spell*> *(__thiscall*)(const MobileActor*)>(0x52B3C0);
	IteratedList<Spell*> * MobileActor::getCombatSpellList() {
		return TES3_MobileActor_getCombatSpellList(this);
	}

	const auto TES3_MobileActor_isActive = reinterpret_cast<bool(__thiscall*)(const MobileActor*)>(0x50F5F0);
	bool MobileActor::isActive() const {
		return TES3_MobileActor_isActive(this);
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

	bool MobileActor::getIsWerewolf() const {
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

	float MobileActor::getWidth() const {
		return reference->getScale() * float(widthRescaled) / 32.0f;
	}

	float MobileActor::getHeight() const {
		return reference->getScale() * float(heightRescaled) / 32.0f;
	}

	const auto TES3_MobileActor_wearItem = reinterpret_cast<void(__thiscall*)(MobileActor*, Object*, ItemData*, bool, bool)>(0x52C770);
	bool MobileActor::wearItem(Object * item, ItemData * itemData, bool addItem, bool unknown, bool useEvents) {
		if (useEvents && mwse::lua::event::EquipEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::EquipEvent(reference, item, itemData));
			if (eventData.valid() && eventData.get_or("block", false)) {
				return false;
			}
		}

		TES3_MobileActor_wearItem(this, item, itemData, addItem, unknown);

		if (useEvents && mwse::lua::event::EquippedEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			stateHandle.triggerEvent(new mwse::lua::event::EquippedEvent(static_cast<TES3::Actor*>(reference->baseObject), this, item, itemData));
		}

		return true;
	}

	bool MobileActor::equipItem(Object * item, ItemData * itemData, bool addItem, bool selectBestCondition, bool selectWorstCondition, bool useEvents) {
		Actor* actor = static_cast<Actor*>(reference->baseObject);

		// Equipping weapons while they are in use breaks animations and AI.
		if (item->objectType == ObjectType::Weapon && isAttackingOrCasting()) {
			return false;
		}

		// Prevent equipping incompatible wearables on non-player beast races.
		// For players, execution continues to later display the correct warning message.
		if (actorType == MobileActorType::NPC && actor->getRace()->getIsBeast()) {
			if (item->objectType == ObjectType::Armor && !static_cast<Armor*>(item)->isUsableByBeasts()) {
				return false;
			}
			if (item->objectType == ObjectType::Clothing && !static_cast<Clothing*>(item)->isUsableByBeasts()) {
				return false;
			}
		}

		// Check if item exists in the inventory.
		ItemStack* s = actor->inventory.findItemStack(item);
		if (!s) {
			if (addItem) {
				actor->addItem(static_cast<Item*>(item), 1);
				if (actorType == MobileActorType::Player) {
					UI::forcePlayerInventoryUpdate();
				}
				wearItem(item, itemData, false, false, useEvents);
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
			if (equipped && ItemData::isItemDataStackable(equipped->itemData, static_cast<TES3::Item*>(equipped->object))) {
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

		wearItem(item, itemData, false, false, useEvents);
		return true;
	}

	bool MobileActor::equip_lua(sol::object arg) {
		using mwse::lua::getOptionalParam;
		using mwse::lua::getOptionalParamObject;

		if (arg.is<Item>()) {
			return equipItem(arg.as<Item*>());
		}
		else if (arg.is<sol::table>()) {
			sol::table params = arg;

			Item* item = getOptionalParamObject<Item>(params, "item");
			if (item == nullptr) {
				return false;
			}

			auto itemData = getOptionalParam<ItemData*>(params, "itemData", nullptr);
			auto addItem = getOptionalParam<bool>(params, "addItem", false);
			auto selectBestCondition = getOptionalParam<bool>(params, "selectBestCondition", false);
			auto selectWorstCondition = getOptionalParam<bool>(params, "selectWorstCondition", false);
			TES3::WorldController::ItemUpDownSoundBlocker soundBlocker = !getOptionalParam<bool>(params, "playSound", true);

			return equipItem(item, itemData, addItem, selectBestCondition, selectWorstCondition);
		}

		throw std::invalid_argument("Invalid parameter provided: must be a tes3item or table.");
	}

	bool MobileActor::unequip_lua(sol::table args) {
		using mwse::lua::getOptionalParam;
		using mwse::lua::getOptionalParamObject;

		Actor* actor = static_cast<TES3::Actor*>(reference->baseObject);
		EquipmentStack* s = nullptr;
		auto item = mwse::lua::getOptionalParamObject<Item>(args, "item");
		int type = args.get_or("type", 0);
		int armourSlot = args.get_or("armorSlot", -1);
		int clothingSlot = args.get_or("clothingSlot", -1);

		if (item) {
			// Match by item.
			auto itemData = getOptionalParam<ItemData*>(args, "itemData", nullptr);
			if (itemData) {
				s = actor->getEquippedItemExact(item, itemData);
			}
			else {
				s = actor->getEquippedItem(item);
			}
		}
		else if (armourSlot != -1) {
			// Match by slot.
			s = actor->getEquippedArmorBySlot(armourSlot);
		}
		else if (clothingSlot != -1) {
			// Match by slot.
			s = actor->getEquippedClothingBySlot(clothingSlot);
		}
		if (s) {
			// Warning: Unequipping lights during menumode with updateGUI=true triggers a Morrowind crash.
			// UI update has been moved to a separate function.
			actor->unequipItem(s->object, true, this, false, s->itemData);
			actor->postUnequipUIRefresh(this);
			return true;
		}

		if (type) {
			// Match all equipped items by objectType.
			std::vector<TES3::EquipmentStack*> matches;
			for (auto it = actor->equipment.head; it; it = it->next) {
				if (it->data->object->objectType == type) {
					matches.push_back(it->data);
				}
			}
			// Warning: Unequipping lights during menumode with updateGUI=true triggers a Morrowind crash.
			// UI update has been moved to a separate function.
			for (auto it : matches) {
				s = it;
				actor->unequipItem(s->object, true, this, false, s->itemData);
			}
			actor->postUnequipUIRefresh(this);
		}
		return bool(s);
	}

	bool MobileActor::equipMagic_lua(sol::table params) {
		Object* object = mwse::lua::getOptionalParamObject<Object>(params, "source");
		ItemData* itemData = mwse::lua::getOptionalParam<ItemData*>(params, "itemData", nullptr);
		bool equipItem = mwse::lua::getOptionalParam(params, "equipItem", false);
		bool updateGUI = mwse::lua::getOptionalParam(params, "updateGUI", true);
		return equipMagic(object, itemData, equipItem, updateGUI);
	}

	void MobileActor::unequipMagic_lua(sol::optional<sol::table> params) {
		bool unequipItem = mwse::lua::getOptionalParam(params, "unequipItem", false);
		bool updateGUI = mwse::lua::getOptionalParam(params, "updateGUI", true);
		unequipMagic(unequipItem, updateGUI);
	}

	bool MobileActor::getWeaponReady() const {
		return getMobileActorFlag(MobileActorFlag::WeaponDrawn) || actionData.animStateAttack == AttackAnimationState::ReadyingWeap;
	}

	void MobileActor::setWeaponReady(bool value) {
		if (isDead()) {
			return;
		}

		if (value && !getWeaponReady()) {
			// Update equipped weapon, as this->readiedWeapon may have been cleared by an unequip.
			const auto TES3_MobileActor_setReadiedWeapon = reinterpret_cast<void(__thiscall*)(MobileActor*, EquipmentStack*)>(0x52CB70);
			auto weapon = static_cast<Actor*>(reference->baseObject)->getEquippedWeapon();
			TES3_MobileActor_setReadiedWeapon(this, weapon);

			// Start readying action.
			actionData.animStateAttack = AttackAnimationState::ReadyingWeap;
		}
		else if (!value && getWeaponReady()) {
			// Start unreadying action.
			actionData.animStateAttack = AttackAnimationState::UnreadyWeap;
		}
	}

	bool MobileActor::forceWeaponAttack_lua(sol::optional<sol::table> params) {
		auto attackType = mwse::lua::getOptionalParam<int>(params, "attackType");
		auto swing = mwse::lua::getOptionalParam<float>(params, "swing");

		// This function is based on CombatSession::checkAttackWithWeapon.
		// It is modified to start an attack without reach or timer checks.
		const auto isWeaponAnimNotCreatureAttack = reinterpret_cast<bool(__thiscall*)(MobileActor*)>(0x528050);
		if (actionData.target == nullptr) {
			return false;
		}
		if (isWeaponAnimNotCreatureAttack(this) && !getFlagWeaponDrawn()) {
			return false;
		}
		if (readiedWeapon && readiedWeapon->object->getSceneGraphNode() == nullptr) {
			return false;
		}

		if (actionData.animStateAttack == AttackAnimationState::Wait) {
			actionData.animStateAttack = AttackAnimationState::Idle;
		}

		auto animState = actionData.animStateAttack;
		auto animGroup = actionData.animGroupCurrentAction;
		const unsigned char Hit1 = 0x13, SwimHit3 = 0x1A;

		if (animState == AttackAnimationState::Idle
			|| (animState == AttackAnimationState::ReadyingWeap && animGroup >= Hit1 && animGroup <= SwimHit3)) {
			auto data_animGroupNoteClass = reinterpret_cast<int*>(0x78B0A8);
			auto weaponAnimGroup = this->vTable.mobileActor->getReadiedWeaponAnimationGroup(this);

			switch (data_animGroupNoteClass[weaponAnimGroup]) {
			case 2: // Creature attack
			{
				int roll100 = mwse::tes3::rand() % 100;
				if (roll100 < 33) {
					actionData.physicalAttackType = PhysicalAttackType::Creature1;
				}
				else if (roll100 < 66) {
					actionData.physicalAttackType = PhysicalAttackType::Creature2;
				}
				else {
					actionData.physicalAttackType = PhysicalAttackType::Creature3;
				}
				break;
			}
			case 3: // Projectile weapon
				if (!getMobileActorMovementFlag(ActorMovement::Swimming)) {
					actionData.physicalAttackType = PhysicalAttackType::Projectile;
				}
				break;
			case 7: // Melee weapon
				const auto getWeightedRandomAttackDirection = reinterpret_cast<PhysicalAttackType(__thiscall*)(CombatSession*)>(0x5373B0);
				if (attackType) {
					actionData.physicalAttackType = (PhysicalAttackType)attackType.value();
				}
				else if (combatSession) {
					actionData.physicalAttackType = getWeightedRandomAttackDirection(combatSession);
				}
				else {
					actionData.physicalAttackType = PhysicalAttackType::Slash;
				}
				break;
			}

			float attackSwingReleaseAt;
			if (swing) {
				attackSwingReleaseAt = swing.value();
			}
			else {
				// Vanilla randomized swing.
				attackSwingReleaseAt = 0.1f + 0.01f * (mwse::tes3::rand() % 100);
				attackSwingReleaseAt = std::min(1.0f, attackSwingReleaseAt);
			}
			if (combatSession) {
				combatSession->combatDelayTimer = 0;
			}
			animationController.asActor->startAttackAnimation(attackSwingReleaseAt);
			return true;
		}
		return false;
	}

	static MobileActor* overrideActorsNextHitStunTest = nullptr;

	bool MobileActor::hitStun_lua(sol::optional<sol::table> params) {
		auto cancel = mwse::lua::getOptionalParam<bool>(params, "cancel", false);
		auto knockDown = mwse::lua::getOptionalParam<bool>(params, "knockDown", false);

		const auto animData = this->getAnimationData();
		if (!animData) {
			return false;
		}

		if (cancel) {
			// Attempt to cancel hit stun or knockdown.
			if (actionData.animGroupStunEffect != 255) {
				actionData.animGroupStunEffect = 255;
			}
			else if (actionData.animStateAttack == AttackAnimationState::Knockdown) {
				actionData.animStateAttack = AttackAnimationState::Ready;
			}
			else {
				// Set up cancellation of the next hitstun.
				// This feature allows affecting hitstun logic that runs after the 'damaged' event.
				overrideActorsNextHitStunTest = this;
			}
			return true;
		}

		// Conditionals matching the hit stun mechanics.
		auto animGroup = animData->currentAnimGroup[0];
		const unsigned char Hit1 = 0x13, SwimHit3 = 0x1A;
		if (!isNotKnockedDownOrOut() || (animGroup >= Hit1 && animGroup <= SwimHit3)) {
			return false;
		}
			
		if (knockDown) {
			// Knockdown, heavy stun. When the character falls to their knees and takes seconds to recover.
			WorldController::get()->magicInstanceController->interruptCasting(reference);
			actionData.animStateAttack = AttackAnimationState::Knockdown;
			return true;
		}
		else {
			// Hit stun. When attacking or casting is interruped and the recovery animation is short.
			// Note that this specific animation ID is detected and later converted to a randomized animation (Swim)Hit1-3.

			// Conditionals matching the hit stun mechanics. Creatures are less interruptible.
			auto animStateAttack = actionData.animStateAttack;
			if (this->actorType != TES3::MobileActorType::Creature
				|| (animStateAttack != TES3::AttackAnimationState::SwingUp
				&& animStateAttack != TES3::AttackAnimationState::SwingDown
				&& animStateAttack != TES3::AttackAnimationState::SwingHit
				&& animStateAttack != TES3::AttackAnimationState::SwingFollowLight
				&& animStateAttack != TES3::AttackAnimationState::SwingFollowMed
				&& animStateAttack != TES3::AttackAnimationState::SwingFollowHeavy
				&& animStateAttack != TES3::AttackAnimationState::Casting
				&& animStateAttack != TES3::AttackAnimationState::CastingFollow
				&& animStateAttack != TES3::AttackAnimationState::PickingProbing))
			{
				const unsigned char knockDownAnim = 0x22;
				actionData.animGroupStunEffect = knockDownAnim;
				return true;
			}
		}
		return false;
	}

	const auto TES3_MobileActor_hitStunTestAndReportCrime = reinterpret_cast<void(__thiscall*)(MobileActor*, float, bool)>(0x557840);
	void MobileActor::hitStunTestAndReportCrime(float damage, bool wasKillingBlow) {
		// Additional check to support the hitstun cancelling code.
		if (this == overrideActorsNextHitStunTest) {
			// Prevent hitstun by zeroing the reported damage.
			overrideActorsNextHitStunTest = nullptr;
			damage = 0.0f;
		}

		TES3_MobileActor_hitStunTestAndReportCrime(this, damage, wasKillingBlow);
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

	bool MobileActor::getMovementFlagSliding() const {
		return getMobileActorMovementFlag(ActorMovement::Sliding);
	}

	void MobileActor::setMovementFlagSliding(bool value) {
		setMobileActorMovementFlag(TES3::ActorMovement::Sliding, value);
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

	bool MobileActor::isSpeaking() const {
		TES3::AnimationData* animData = nullptr;

		if (!animationController.asActor) {
			return false;
		}

		if (actorType == TES3::MobileActorType::Player) {
			// Always inspect third-person animation data for the player.
			animData = reference->getAttachedAnimationData();
		}
		else {
			animData = animationController.asActor->animationData;
		}

		if (animData && animData->lipsyncLevel != -1) {
			return true;
		}

		return false;
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
			if (activeEffect.isIllegalSummon) {
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

	sol::table MobileActor::getActiveMagicEffectsList_lua(sol::optional<sol::table> params) {
		auto effectID = mwse::lua::getOptionalParam<int>(params, "effect");
		auto serial = mwse::lua::getOptionalParam<unsigned int>(params, "serial");

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
