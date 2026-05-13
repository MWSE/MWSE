#pragma once

#include "NIObject.h"

namespace NI {
	struct UVData : Object {
		struct KeyData {
			unsigned int numKeys; // 0x0
			void* keys; // 0x4
			unsigned int type; // 0x8
		};
		KeyData UOffsetData; // 0x8
		KeyData VOffsetData; // 0x14
		KeyData UTilingData; // 0x20
		KeyData VTilingData; // 0x2C
	};
	static_assert(sizeof(UVData) == 0x38, "NI::UVData failed size validation");
}
