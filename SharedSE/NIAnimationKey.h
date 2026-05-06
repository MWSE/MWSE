#pragma once

#include "NIColor.h"
#include "NIQuaternion.h"
#include "NIVector3.h"

namespace NI {
	struct AnimationKey {
		enum class ContentType : unsigned int {
			Float,
			Position,
			Rotation,
			Color,
			Text,
			Vis,

			COUNT,
		};
		enum class KeyType : unsigned int {
			NoInterp,
			Linear,
			Bezier,
			TCB,
			Euler,

			COUNT,
		};

		float timing; // 0x0

		using FillDerivedValuesFunction = void(__cdecl*)(AnimationKey* keys, unsigned int keyCount, KeyType keyType);
		static FillDerivedValuesFunction getFillDerivedValuesFunction(ContentType content, KeyType key);
	};
	static_assert(sizeof(AnimationKey) == 0x4, "NI::AnimationKey failed size validation");
	static_assert(static_cast<unsigned int>(AnimationKey::ContentType::COUNT) == 6u, "NI::AnimationKey::ContentType failed enum count validation");
	static_assert(static_cast<unsigned int>(AnimationKey::KeyType::COUNT) == 5, "NI::AnimationKey::KeyType failed enum count validation");

	struct FloatKey : AnimationKey {
		float value; // 0x4
	};
	static_assert(sizeof(FloatKey) == 0x8, "NI::FloatKey failed size validation");

	struct BezFloatKey : FloatKey {
		float inTangent; // 0x8
		float outTangent; // 0xC
	};
	static_assert(sizeof(BezFloatKey) == 0x10, "NI::BezFloatKey failed size validation");

	struct TCBFloatKey : FloatKey {
		float tension; // 0x8
		float continuity; // 0xC
		float bias; // 0x10
		float derivedA; // 0x14
		float derivedB; // 0x18
	};
	static_assert(sizeof(TCBFloatKey) == 0x1C, "NI::TCBFloatKey failed size validation");

	union AmbiguousFloatKeyPtr {
		FloatKey* asFloatKey;
		BezFloatKey* asBezFloatKey;
		TCBFloatKey* asTCBFloatKey;
	};
	static_assert(sizeof(AmbiguousFloatKeyPtr) == sizeof(void*), "NI::AmbiguousFloatKeyPtr failed size validation");

	struct RotKey : AnimationKey {
		Quaternion value; // 0x4
	};
	static_assert(sizeof(RotKey) == 0x14, "NI::RotKey failed size validation");

	struct BezRotKey : RotKey {
		Quaternion intermediate; // 0x14
	};
	static_assert(sizeof(BezRotKey) == 0x24, "NI::BezRotKey failed size validation");

	struct TCBRotKey : RotKey {
		float tension; // 0x14
		float continuity; // 0x18
		float bias; // 0x1C
		Quaternion intermediateA; // 0x20
		Quaternion intermediateB; // 0x30
	};
	static_assert(sizeof(TCBRotKey) == 0x40, "NI::TCBRotKey failed size validation");

	struct EulerRotKey : RotKey {
		enum Order : unsigned int {
			ORDER_XYZ, ORDER_XZY, ORDER_YZX, ORDER_YXZ,
			ORDER_ZXY, ORDER_ZYX, ORDER_XYX, ORDER_YZY, ORDER_ZXZ,
			ORDER_COUNT,
		};
		unsigned int numKeys[3]; // 0x14
		AnimationKey::KeyType keyTypes[3]; // 0x20
		Order order; // 0x2C
		AmbiguousFloatKeyPtr keys[3]; // 0x30
		unsigned int lastIndices[3]; // 0x38

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		std::reference_wrapper<decltype(numKeys)> getNumKeys_lua();
		std::reference_wrapper<decltype(keyTypes)> getKeyTypes_lua();
		sol::table getKeys_lua(sol::this_state ts);
		std::reference_wrapper<decltype(lastIndices)> getLastIndices_lua();
#endif
	};
	static_assert(sizeof(EulerRotKey) == 0x48, "NI::EulerRotKey failed size validation");

	union AmbiguousRotKeyPtr {
		RotKey* asRotKey;
		BezRotKey* asBezRotKey;
		TCBRotKey* asTCBRotKey;
		EulerRotKey* asEulerRotKey;
	};
	static_assert(sizeof(AmbiguousRotKeyPtr) == sizeof(void*), "NI::AmbiguousRotKeyPtr failed size validation");

	struct PosKey : AnimationKey {
		Vector3 value; // 0x4
	};
	static_assert(sizeof(PosKey) == 0x10, "NI::PosKey failed size validation");

	struct BezPosKey : PosKey {
		Vector3 inTangent; // 0x10
		Vector3 outTangent; // 0x1C
		Vector3 intermediateA; // 0x28
		Vector3 intermediateB; // 0x34
	};
	static_assert(sizeof(BezPosKey) == 0x40, "NI::BezPosKey failed size validation");

	struct TCBPosKey : PosKey {
		float tension; // 0x10
		float continuity; // 0x14
		float bias; // 0x18
		Vector3 derivedA; // 0x1C
		Vector3 derivedB; // 0x28
		Vector3 intermediateA; // 0x34
		Vector3 intermediateB; // 0x40
	};
	static_assert(sizeof(TCBPosKey) == 0x4C, "NI::TCBPosKey failed size validation");

	union AmbiguousPosKeyPtr {
		PosKey* asPosKey;
		BezPosKey* asBezPosKey;
		TCBPosKey* asTCBPosKey;
	};
	static_assert(sizeof(AmbiguousPosKeyPtr) == sizeof(void*), "NI::AmbiguousPosKeyPtr failed size validation");

	struct ColorKey : AnimationKey {
		PackedColor color; // 0x4
	};
	static_assert(sizeof(ColorKey) == 0x8, "NI::ColorKey failed size validation");
}
