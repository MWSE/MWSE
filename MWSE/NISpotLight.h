#pragma once

#include "NIPointLight.h"

namespace NI {
	struct SpotLight : PointLight {
		TES3::Vector3 direction; // 0xDC
		float spotAngle; // 0xE8
		float spotExponent; // 0xEC
	};
	static_assert(sizeof(SpotLight) == 0xF0, "NI::SpotLight failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::SpotLight)
