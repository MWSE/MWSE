#pragma once

#include "NIPointLight.h"

namespace NI {
	struct SpotLight : PointLight {
		Vector3 direction; // 0xDC
		float spotAngle; // 0xE8
		float spotExponent; // 0xEC
	};
	static_assert(sizeof(SpotLight) == 0xF0, "NI::SpotLight failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::SpotLight)
#endif
