#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	struct Activator : PhysicalObject {
		Activator();
		~Activator();

		int unknown_0x30;
		char *model; // 0x34
		char *name; // 0x38
		Script * script;
	};
	static_assert(sizeof(Activator) == 0x40, "TES3::Activator failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Activator)
