#pragma once

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1

// In MWSE context, NI::Vector4 IS TES3::Vector4.
#include "TES3Vectors.h"
namespace NI {
	using Vector4 = TES3::Vector4;
}

#else

namespace NI {
	struct Vector4 {
		float x; // 0x0
		float y; // 0x4
		float z; // 0x8
		float w; // 0xC
	};
	static_assert(sizeof(Vector4) == 0x10, "NI::Vector4 failed size validation");
}

#endif
