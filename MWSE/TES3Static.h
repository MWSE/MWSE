#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	struct Static : PhysicalObject {
		Static();
		~Static();

		static constexpr auto OBJECT_TYPE = ObjectType::Static;

		char * model;
	};
	static_assert(sizeof(Static) == 0x34, "TES3::Static failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Static)
