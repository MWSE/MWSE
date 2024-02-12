#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	struct Door : PhysicalObject {
		int unknown_0x30;
		char name[32]; // 0x34
		char model[32]; // 0x54
		Script * script; // 0x74
		Sound * openSound; // 0x78
		Sound * closeSound; // 0x7C

		static constexpr auto OBJECT_TYPE = ObjectType::Door;

		Door() = delete;
		~Door() = delete;
	};
	static_assert(sizeof(Door) == 0x80, "TES3::Door failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Door)
