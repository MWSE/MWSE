#include "TES3ActorAnimationControllerLua.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3ActorAnimationController.h"
#include "TES3MobileActor.h"

namespace mwse {
	namespace lua {
		void bindTES3ActorAnimationController() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ActorAnimationController>("tes3actorAnimationController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["alphaProperty"] = &TES3::ActorAnimationController::alphaProperty;
			usertypeDefinition["animGroupIdle"] = &TES3::ActorAnimationController::animGroupIdle;
			usertypeDefinition["animGroupIdle2"] = &TES3::ActorAnimationController::animGroupIdle2;
			usertypeDefinition["animGroupMovement"] = &TES3::ActorAnimationController::animGroupMovement;
			usertypeDefinition["attachTiming"] = &TES3::ActorAnimationController::attachTiming;
			usertypeDefinition["attackFollowTiming"] = &TES3::ActorAnimationController::attackFollowTiming;
			usertypeDefinition["attackSwing"] = &TES3::ActorAnimationController::attackSwing;
			usertypeDefinition["groundPlaneRotation"] = &TES3::ActorAnimationController::groundPlaneRotation;
			usertypeDefinition["hitTiming"] = &TES3::ActorAnimationController::hitTiming;
			usertypeDefinition["materialProperty"] = &TES3::ActorAnimationController::materialProperty;
			usertypeDefinition["maxAttackTiming"] = &TES3::ActorAnimationController::maxAttackTiming;
			usertypeDefinition["minAttackTiming"] = &TES3::ActorAnimationController::minAttackTiming;
			usertypeDefinition["minHitTiming"] = &TES3::ActorAnimationController::minHitTiming;
			usertypeDefinition["mobile"] = sol::readonly_property(&TES3::ActorAnimationController::mobileActor);
			usertypeDefinition["patchedOverrideState"] = &TES3::ActorAnimationController::patchedOverrideState;
			usertypeDefinition["remainingBlockTime"] = &TES3::ActorAnimationController::remainingBlockTime;
			usertypeDefinition["shouldJump"] = &TES3::ActorAnimationController::shouldJump;
			usertypeDefinition["speedMultiplier"] = &TES3::ActorAnimationController::speedMultiplier;
			usertypeDefinition["startTime"] = &TES3::ActorAnimationController::startTime;
			usertypeDefinition["swimmingFlyingRotation"] = &TES3::ActorAnimationController::swimmingFlyingRotation;
			usertypeDefinition["vertexColorProperty"] = &TES3::ActorAnimationController::vertexColorProperty;

			// Custom properties.
			usertypeDefinition["weaponSpeed"] = sol::property(&TES3::ActorAnimationController::getWeaponAnimSpeed, &TES3::ActorAnimationController::setWeaponAnimSpeed);
		}
	}
}
