#include "TES3PlayerAnimationControllerLua.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "NICamera.h"

#include "TES3PlayerAnimationController.h"

namespace mwse::lua {
	void bindTES3PlayerAnimationController() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::PlayerAnimationController>("tes3playerAnimationController");
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		usertypeDefinition["allowVerticalAirControl"] = &TES3::PlayerAnimationController::allowVerticalAirControl;
		usertypeDefinition["cameraOffset"] = &TES3::PlayerAnimationController::cameraOffset;
		usertypeDefinition["firstPersonHeadCameraNode"] = sol::readonly_property(&TES3::PlayerAnimationController::firstPersonHeadCameraNode);
		usertypeDefinition["is3rdPerson"] = sol::readonly_property(&TES3::PlayerAnimationController::is3rdPerson);
		usertypeDefinition["pickData"] = sol::readonly_property(&TES3::PlayerAnimationController::pickData);
		usertypeDefinition["shadowCameraMatrix"] = sol::readonly_property(&TES3::PlayerAnimationController::shadowCameraMatrix);
		usertypeDefinition["switchPOVMode"] = &TES3::PlayerAnimationController::switchPOVMode;
		usertypeDefinition["useThirdPersonAfterVanityCameraDone"] = &TES3::PlayerAnimationController::useThirdPersonAfterVanityCameraDone;
		usertypeDefinition["vanityCamera"] = sol::readonly_property(&TES3::PlayerAnimationController::vanityCamera);
		usertypeDefinition["vanityCameraAngle"] = sol::property(&TES3::PlayerAnimationController::getVanityCameraAngle, &TES3::PlayerAnimationController::setVanityCameraAngle);
		usertypeDefinition["vanityCameraDistance"] = &TES3::PlayerAnimationController::vanityCameraDistance;
		usertypeDefinition["vanityCameraEnabled"] = &TES3::PlayerAnimationController::vanityCameraEnabled;
		usertypeDefinition["vanityCameraZ"] = &TES3::PlayerAnimationController::vanityCameraZ;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::ActorAnimationController>();
	}
}
