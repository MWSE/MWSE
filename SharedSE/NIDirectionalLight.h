#pragma once

#include "NILight.h"

namespace NI {
	struct DirectionalLight : Light {
		Vector3 direction; // 0xD0
	};
	static_assert(sizeof(DirectionalLight) == 0xDC, "NI::DirectionalLight failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::DirectionalLight)
#endif
