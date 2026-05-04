#pragma once

namespace NI {
	struct Vector4 {
		float x; // 0x0
		float y; // 0x4
		float z; // 0x8
		float w; // 0xC
	};
	static_assert(sizeof(Vector4) == 0x10, "NI::Vector4 failed size validation");
}
