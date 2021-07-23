#pragma once

#include "NILight.h"

namespace NI {
	struct AmbientLight : Light {

	};
	static_assert(sizeof(AmbientLight) == 0xD0, "NI::AmbientLight failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::AmbientLight)
