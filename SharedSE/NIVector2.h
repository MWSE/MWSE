#pragma once

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1

// In MWSE context, NI::Vector2 IS TES3::Vector2.
#include "TES3Vectors.h"
namespace NI {
	using Vector2 = TES3::Vector2;
}

#else

namespace NI {
	struct Vector2 {
		float x;
		float y;
	};
}

#endif
