#pragma once

#include "TES3Lockpick.h"

namespace TES3 {
	// Probes are effectively just lockpicks.
	struct Probe : Lockpick {};
	static_assert(sizeof(Probe) == 0xB8, "TES3::Probe failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Probe)
