#pragma once

#include "NILight.h"

#include "TES3Vectors.h"

namespace NI {
	struct DirectionalLight : Light {
		TES3::Vector3 direction; // 0xD0
	};
	static_assert(sizeof(DirectionalLight) == 0xDC, "NI::DirectionalLight failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::DirectionalLight)
