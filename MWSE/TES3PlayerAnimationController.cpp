#include "TES3PlayerAnimationController.h"

#include "LuaManager.h"
#include "LuaUtil.h"
#include "MemoryUtil.h"
#include "NINode.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

#include "LuaCameraControlEvent.h"

namespace TES3 {
	TES3::Transform PlayerAnimationController::previousCameraTransform;
	TES3::Transform PlayerAnimationController::previousArmCameraTransform;

	using gVanityCameraAngle = mwse::ExternalGlobal<float, 0x7D00C8>;

	bool PlayerAnimationController::force1stPerson() {
		if (is3rdPerson) {
			switchPOVMode = 1;
			return true;
		}
		return false;
	}

	bool PlayerAnimationController::force3rdPerson() {
		if (!is3rdPerson) {
			switchPOVMode = 1;
			return true;
		}
		return false;
	}

	float PlayerAnimationController::getVanityCameraAngle() const {
		return gVanityCameraAngle::get();
	}

	void PlayerAnimationController::setVanityCameraAngle(float angle) {
		gVanityCameraAngle::set(angle);
	}

	const auto TES3_PlayerAnimationController_syncRotation = reinterpret_cast<void(__thiscall*)(PlayerAnimationController*)>(0x5438F0);
	void PlayerAnimationController::syncRotation() {
		const auto worldController = WorldController::get();

		// Save camera transforms for the cameraControl event.
		previousCameraTransform = worldController->worldCamera.cameraRoot->getTransforms();
		previousArmCameraTransform = worldController->armCamera.cameraRoot->getTransforms();

		// Call the original function.
		TES3_PlayerAnimationController_syncRotation(this);
	}
}
