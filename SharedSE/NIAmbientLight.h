#pragma once

#include "NILight.h"

namespace NI {
	struct AmbientLight : Light {
	};
	static_assert(sizeof(AmbientLight) == 0xD0, "NI::AmbientLight failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::AmbientLight)
#endif
