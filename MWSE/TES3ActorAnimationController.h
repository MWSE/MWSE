#pragma once

#include "TES3Defines.h"

#include "TES3AnimationData.h"
#include "TES3Vectors.h"

#include "NIExtraData.h"
#include "NIProperty.h"

namespace TES3 {
	struct KeyframeDefinition {
		const char* name; // 0x0
		NI::Sequence* sequences[3];
		AnimationGroup* animationGroup; // 0x10
		unsigned short groupCount; // 0x14
		unsigned short refCount; // 0x16
	};
	static_assert(sizeof(KeyframeDefinition) == 0x18, "TES3::KeyframeDefinition failed size validation");

	struct ActorAnimationController_VirtualTable {
		void(__thiscall* destructor)(ActorAnimationController*, int); // 0x0
		void(__thiscall* unknown_0x4)(ActorAnimationController*, float); // 0x4
		void(__thiscall* unknown_0x8)(ActorAnimationController*, int); // 0x8
		void(__thiscall* syncReferenceRotation)(ActorAnimationController*); // 0xC
		void(__thiscall* setProperties)(ActorAnimationController*, NI::Node*); // 0x10
		Matrix33* (__thiscall* getRotation)(ActorAnimationController*, Matrix33*); // 0x14
		void(__thiscall* unknown_0x18)(ActorAnimationController*, Vector3*); // 0x18
		void(__thiscall* unknown_0x1C)(ActorAnimationController*); // 0x1C
	};
	static_assert(sizeof(ActorAnimationController_VirtualTable) == 0x20, "TES3::ActorAnimationController_VirtualTable failed size validation");

	struct ActorAnimationController {
		struct Layer {
			unsigned char animGroupID;
			int playbackTypeEnum;
			float timing_8;
			int loopCount;
		};
		static_assert(sizeof(ActorAnimationController::Layer) == 0x10, "TES3::ActorAnimationController::Layer failed size validation");

		ActorAnimationController_VirtualTable* vTable; // 0x0
		unsigned char useAnimationDelta; // 0x04
		char padding_0x5[3];
		char animGroup_unknown; // 0x08
		char padding_0x9[3];
		int playbackTypeEnum; // 0x0C
		float attackFollowTiming;
		float remainingBlockTime; // 0x14
		float startTime; // 0x18
		float minAttackTiming; // 0x1C
		float maxAttackTiming; // 0x20
		float minHitTiming; // 0x24
		float hitTiming; // 0x28
		float attackSwing; // 0x2C
		float attachTiming; // 0x30
		float speedMultiplier; // 0x34
		MobileActor * mobileActor; // 0x38
		AnimationData * animationData; // 0x3C
		NI::Pointer<NI::AlphaProperty> alphaProperty; // 0x40
		NI::Pointer<NI::MaterialProperty> materialProperty; // 0x44
		NI::Pointer<NI::VertexColorProperty> vertexColorProperty; // 0x48
		Matrix33 groundPlaneRotation; // 0x4C
		Matrix33 swimmingFlyingRotation; // 0x70
		int unknown_0x94;
		unsigned char animGroupMovement; // 0x98
		unsigned char animGroupIdle; // 0x99
		unsigned char animGroupIdle2; // 0x9A
		unsigned char patchedOverrideState; // 0x9B
		int shouldJump; // 0x9C
		int unknown_0xA0;
		Layer layerLowerBody; // 0xA4
		Layer layerUpperBody; // 0xB4
		Layer layerShieldArm; // 0xC4

		//
		// Other related this-call functions.
		//

		float calculateMovementSpeed();

		float getOpacity();
		void setOpacity(float value);

		void startAttackAnimation(float swing);

		//
		// Custom functions.
		//

		// Fixes any transparency values.
		void updateOpacity();

		float getWeaponAnimSpeed() const;
		void setWeaponAnimSpeed(float speed);
		void selectMovementAnimAndUpdate(float deltaTime, bool flag);
	};
	static_assert(sizeof(ActorAnimationController) == 0xD4, "TES3::ActorAnimationController failed size validation");
}
