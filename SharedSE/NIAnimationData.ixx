module;

#include "NIObject.h"

export module NIAnimationData;

import NIAnimationKey;

namespace NI {
	export struct FloatData : Object {
		unsigned int keyCount; // 0x8
		AmbiguousFloatKeyPtr keys; // 0xC
		AnimationKey::KeyType keyType; // 0x10

		unsigned int getKeyDataSize() const;
	};
	static_assert(sizeof(FloatData) == 0x14, "NI::FloatData failed size validation");

	export struct PosData : Object {
		unsigned int keyCount; // 0x8
		AmbiguousPosKeyPtr keys; // 0xC
		AnimationKey::KeyType keyType; // 0x10

		unsigned int getKeyDataSize() const;
	};
	static_assert(sizeof(PosData) == 0x14, "NI::PosData failed size validation");

	export struct ColorData : Object {
		unsigned int keyCount; // 0x8
		ColorKey** keys; // 0xC
		AnimationKey::KeyType keyType; // 0x10

		std::span<ColorKey*> getKeys() const;
	};
	static_assert(sizeof(ColorData) == 0x14, "NI::ColorData failed size validation");
}
