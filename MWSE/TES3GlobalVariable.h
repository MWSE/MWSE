#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	struct GlobalVariable : BaseObject {
		char name[32]; // 0x10
		char valueType; // 0x30
		char unknown_0x31;
		char unknown_0x32;
		char unknown_0x33;
		float value; // 0x34

		static constexpr auto OBJECT_TYPE = ObjectType::Global;

		GlobalVariable() = delete;
		~GlobalVariable() = delete;

		//
		// Custom functions
		//

		float getValue() const;
		double getValue_lua() const;
		void setValue_lua(double value);

	};
	static_assert(sizeof(GlobalVariable) == 0x38, "TES3::GlobalVariable failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::GlobalVariable)
