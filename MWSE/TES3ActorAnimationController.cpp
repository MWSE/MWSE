#include "TES3ActorAnimationController.h"

#include "LuaManager.h"

#include "LuaAttackStartEvent.h"
#include "LuaCalcMovementSpeedEvent.h"

#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace TES3 {
	const auto TES3_ActorAnimationController_calculateMovementSpeed = reinterpret_cast<float(__thiscall*)(ActorAnimationController*)>(0x53E1A0);
	float ActorAnimationController::calculateMovementSpeed() {
		// Call the original function to get the default movement speed value.
		float speed = TES3_ActorAnimationController_calculateMovementSpeed(this);

		// Launch our event, and overwrite the speed with what was given back to us.
		if (mwse::lua::event::CalculateMovementSpeed::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CalculateMovementSpeed(mwse::lua::event::CalculateMovementSpeed::Move, this->mobileActor, speed));
			if (eventData.valid()) {
				speed = eventData["speed"];
			}
		}

		return speed;
	}

	float ActorAnimationController::calculateCreatureMovementSpeed() {
		auto movementFlags = mobileActor->movementFlags;
		float speed = 0;

		// Call the relevant function to get the default movement speed.
		if (movementFlags & ActorMovement::Flying) {
			speed = TES3_ActorAnimationController_calculateMovementSpeed(this);
		}
		else if (movementFlags & ActorMovement::Swimming) {
			speed = mobileActor->calculateSwimSpeed();
		}
		else {
			speed = mobileActor->vTable.mobileActor->calculateWalkSpeed(mobileActor);
		}

		// Launch our event, and overwrite the speed with what was given back to us.
		if (mwse::lua::event::CalculateMovementSpeed::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CalculateMovementSpeed(mwse::lua::event::CalculateMovementSpeed::Move, this->mobileActor, speed));
			if (eventData.valid()) {
				speed = eventData["speed"];
			}
		}

		return speed;
	}

	float ActorAnimationController::calculateAttackSwing() {
		if (mobileActor->actionData.animStateAttack != AttackAnimationState::SwingUp) {
			return 0.0;
		}

		if (startTime == 0.0 || maxAttackTiming == 0.0 || startTime == maxAttackTiming)
		{
			return 1.0;
		}

		return std::clamp(mobileActor->actionData.swingTimer / (maxAttackTiming - startTime), 0.0f, 1.0f);
	}

	const auto TES3_ActorAnimationController_getOpacity = reinterpret_cast<float(__thiscall*)(const ActorAnimationController*)>(0x542130);
	float ActorAnimationController::getOpacity() const {
		return TES3_ActorAnimationController_getOpacity(this);
	}

	const auto TES3_ActorAnimationController_setOpacity = reinterpret_cast<void(__thiscall*)(ActorAnimationController*, float)>(0x542030);
	void ActorAnimationController::setOpacity(float value) {
		TES3_ActorAnimationController_setOpacity(this, value);
	}

	const auto TES3_ActorAnimController_startCastAnimation = reinterpret_cast<void(__thiscall*)(ActorAnimationController*)>(0x541A90);
	void ActorAnimationController::startCastAnimation() {
		TES3_ActorAnimController_startCastAnimation(this);
	}

	const auto TES3_ActorAnimController_startAttackAnimation = reinterpret_cast<void(__thiscall*)(ActorAnimationController*, float)>(0x5411C0);
	void ActorAnimationController::startAttackAnimation(float swing) {
		sol::optional<float> speed;

		if (mwse::lua::event::AttackStartEvent::getEventEnabled()) {
			PhysicalAttackType attack = mobileActor->actionData.physicalAttackType;
			speed = mobileActor->getWeaponSpeed();

			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::AttackStartEvent(mobileActor, attack, speed.value()));
			if (eventData.valid()) {
				// Replace attack type only if original is slash, chop, or thrust.
				PhysicalAttackType newAttack = eventData["attackType"];
				if (attack >= PhysicalAttackType::Slash && attack <= PhysicalAttackType::Thrust
					&& newAttack >= PhysicalAttackType::Slash && newAttack <= PhysicalAttackType::Thrust) {

					mobileActor->actionData.physicalAttackType = newAttack;
				}

				// Replace weapon speed.
				speed = eventData["attackSpeed"];
			}
		}

		// Call the original function.
		TES3_ActorAnimController_startAttackAnimation(this, swing);

		// Optionally override weapon animation speed.
		if (speed) {
			animationData->weaponSpeed = speed.value();
		}
	}

	void ActorAnimationController::updateOpacity() {
		const auto invisibility = mobileActor->effectAttributes[TES3::EffectAttribute::Invisibility];
		const auto chameleon = mobileActor->effectAttributes[TES3::EffectAttribute::Chameleon];

		// We care about invisibility first.
		if (invisibility > 0) {
			if (mobileActor == TES3::WorldController::get()->getMobilePlayer()) {
				setOpacity(0.25f);
			}
			else {
				setOpacity(0.05f);
			}
		}
		// Fall back to chameleon, based on magnitude.
		else if (chameleon > 0) {
			setOpacity(std::clamp(1.0f - (float(chameleon) / 100.0f), 0.25f, 0.75f));
		}
		// If all else fails we go for no Opacity.
		else {
			setOpacity(1.0f);
		}
	}

	float ActorAnimationController::getWeaponAnimSpeed() const {
		return animationData->weaponSpeed;
	}

	void ActorAnimationController::setWeaponAnimSpeed(float speed) {
		if (mobileActor->actorType == MobileActorType::Player) {
			auto macp = static_cast<MobilePlayer*>(mobileActor);
			macp->reference->getAttachedAnimationData()->weaponSpeed = speed;
			macp->firstPersonReference->getAttachedAnimationData()->weaponSpeed = speed;
		}
		else {
			animationData->weaponSpeed = speed;
		}
	}

	const auto TES3_ActorAnimController_selectMovementAnimAndUpdate = reinterpret_cast<void(__thiscall*)(TES3::ActorAnimationController*, float, bool)>(0x540A90);
	void ActorAnimationController::selectMovementAnimAndUpdate(float deltaTime, bool flag) {
		auto state = patchedOverrideState;

		if (state != 0xFF) {
			auto baseAnimGroup = animationData->currentAnimGroup[0];
			auto overrideAnimGroup = animationData->currentAnimGroup[2];

			// Check if override animation has completed.
			if (overrideAnimGroup != baseAnimGroup) {
				// Still animating. Suppress updates to the AnimationAttachment by misusing ActorAnimData internal flags.
				if (state <= 1) {
					layerUpperBody.playbackTypeEnum = 8;
				}
				if (state <= 2) {
					layerShieldArm.playbackTypeEnum = 8;
				}
			}
			else {
				// Animation has completed, and been paused at the end. Signal controller to update animations.
				if (state <= 1) {
					animationData->currentAnimGroup[1] = 0xFF;
				}
				if (state <= 2) {
					animationData->currentAnimGroup[2] = 0xFF;
				}
				// End override state.
				patchedOverrideState = 0xFF;
			}
		}
		TES3_ActorAnimController_selectMovementAnimAndUpdate(this, deltaTime, flag);
	}

}
