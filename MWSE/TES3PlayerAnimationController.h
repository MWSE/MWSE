#pragma once

#include "TES3ActorAnimationController.h"

#include "NIPick.h"
#include "TES3Vectors.h"

namespace TES3 {
	struct PlayerAnimationController : ActorAnimationController {
		NI::Camera * firstPersonHeadCameraNode; // 0xD4 
		TES3::Vector3 cameraOffset; // 0xD8
		NI::Pick * pickData; // 0xE4
		bool is3rdPerson; // 0xE8
		unsigned char switchPOVMode; // 0xE9
		bool allowVerticalAirControl; // 0xEA
		int vanityCameraEnabled; // 0xEC
		bool useThirdPersonAfterVanityCameraDone; // 0xF0
		float vanityCameraZ; // 0xF4
		int vanityCameraDistance; // 0xF8
		NI::Camera * vanityCamera; // 0xFC
		Matrix33 shadowCameraMatrix; // 0x100

		static TES3::Transform previousCameraTransform;
		static TES3::Transform previousArmCameraTransform;

		PlayerAnimationController() = delete;
		~PlayerAnimationController() = delete;

		//
		// vTable overrides
		//

		void syncRotation();

		//
		// Custom functions
		//

		bool force1stPerson();
		bool force3rdPerson();

		float getVanityCameraAngle() const;
		void setVanityCameraAngle(float angle);
	};
	static_assert(sizeof(PlayerAnimationController) == 0x124, "TES3::PlayerAnimationController failed size validation");
}
