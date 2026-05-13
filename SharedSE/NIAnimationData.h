#pragma once

#include "NIAnimationKey.h"
#include "NIObject.h"

namespace NI {
	struct FloatData : Object {
		unsigned int keyCount; // 0x8
		AmbiguousFloatKeyPtr keys; // 0xC
		AnimationKey::KeyType keyType; // 0x10

		unsigned int getKeyDataSize() const;
	};
	static_assert(sizeof(FloatData) == 0x14, "NI::FloatData failed size validation");

	struct PosData : Object {
		unsigned int keyCount; // 0x8
		AmbiguousPosKeyPtr keys; // 0xC
		AnimationKey::KeyType keyType; // 0x10

		unsigned int getKeyDataSize() const;
	};
	static_assert(sizeof(PosData) == 0x14, "NI::PosData failed size validation");

	struct ColorData : Object {
		unsigned int keyCount; // 0x8
		ColorKey** keys; // 0xC
		AnimationKey::KeyType keyType; // 0x10

		nonstd::span<ColorKey*> getKeys() const;
	};
	static_assert(sizeof(ColorData) == 0x14, "NI::ColorData failed size validation");
}
