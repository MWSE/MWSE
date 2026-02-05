#pragma once

#include "TES3Defines.h"

#include "TES3AnimationData.h"
#include "TES3Vectors.h"

#include "NIExtraData.h"
#include "NIProperty.h"

namespace TES3 {
	struct ActorAnimationController_VirtualTable {
		void(__thiscall* destructor)(ActorAnimationController*, int); // 0x0
		void(__thiscall* update)(ActorAnimationController*, float); // 0x4
		void(__thiscall* updateProceduralAnim)(ActorAnimationController*); // 0x8
		void(__thiscall* syncReferenceRotation)(ActorAnimationController*); // 0xC
		void(__thiscall* setProperties)(ActorAnimationController*, NI::Node*); // 0x10
		Matrix33* (__thiscall* getRotation)(ActorAnimationController*, Matrix33*); // 0x14
		void(__thiscall* getAnimationDelta)(ActorAnimationController*, Vector3*); // 0x18
		void(__thiscall* reset)(ActorAnimationController*); // 0x1C
	};
	static_assert(sizeof(ActorAnimationController_VirtualTable) == 0x20, "TES3::ActorAnimationController_VirtualTable failed size validation");

	struct ActorAnimationController {
		struct BodySection {
			AnimGroupID animGroupID;
			int playbackTypeEnum;
			float attackFollowTiming;
			int loopCount;

			BodySection() = delete;
			~BodySection() = delete;
		};
		static_assert(sizeof(ActorAnimationController::BodySection) == 0x10, "TES3::ActorAnimationController::BodySection failed size validation");

		ActorAnimationController_VirtualTable* vTable; // 0x0
		unsigned char useAnimationDelta; // 0x04
		char padding_0x5[3];
		AnimGroupID animGroupAttack; // 0x08
		char padding_0x9[3];
		int playbackTypeEnum; // 0x0C
		float attackFollowTiming; // 0x10
		float remainingBlockTime; // 0x14
		float startTime; // 0x18
		float minAttackTiming; // 0x1C
		float maxAttackTiming; // 0x20
		float minHitTiming; // 0x24
		float hitTiming; // 0x28
		float strikeWhenAttackSwingReaches; // 0x2C
		float attachTiming; // 0x30
		float speedMultiplier; // 0x34
		MobileActor* mobileActor; // 0x38
		AnimationData* animationData; // 0x3C
		NI::Pointer<NI::AlphaProperty> alphaProperty; // 0x40
		NI::Pointer<NI::MaterialProperty> materialProperty; // 0x44
		NI::Pointer<NI::VertexColorProperty> vertexColorProperty; // 0x48
		Matrix33 groundPlaneRotation; // 0x4C
		Matrix33 verticalRotation; // 0x70
		int unknown_0x94;
		AnimGroupID animGroupMovement; // 0x98
		AnimGroupID animGroupIdle; // 0x99
		AnimGroupID animGroupIdle2; // 0x9A
		unsigned char patchedOverrideState; // 0x9B
		int shouldJump; // 0x9C
		float dotProductFacingAgainstWind; // 0xA0
		BodySection sectionLowerBody; // 0xA4
		BodySection sectionUpperBody; // 0xB4
		BodySection sectionLeftArm; // 0xC4

		ActorAnimationController() = delete;
		~ActorAnimationController() = delete;

		//
		// Other related this-call functions.
		//

		float calculateMovementSpeed();
		float calculateCreatureMovementSpeed();
		float calculateAttackSwing();

		float getOpacity() const;
		void setOpacity(float value);

		void startCastAnimation();
		void startAttackAnimation(float swing);

		void selectDeathAnimation();
		void selectMovementAnimAndUpdate(float deltaTime, bool flag);

		//
		// Custom functions.
		//

		void updateOpacity();	// Fixes any transparency values.

		float getWeaponAnimSpeed() const;
		void setWeaponAnimSpeed(float speed);
	};
	static_assert(sizeof(ActorAnimationController) == 0xD4, "TES3::ActorAnimationController failed size validation");
}
