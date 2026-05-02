#pragma once

#include "NITimeController.h"
#include "NIUVData.h"

namespace NI {
	struct UVController : TimeController {
		unsigned int lastUOffsetIndex; // 0x34
		unsigned int lastUTilingIndex; // 0x38
		unsigned int lastVOffsetIndex; // 0x3C
		unsigned int lastVTilingIndex; // 0x40
		unsigned short textureSet; // 0x44
		Pointer<UVData> uvData; // 0x48
		float currentUOffset; // 0x4C
		float currentVOffset; // 0x50
		float currentUTiling; // 0x54
		float currentVTiling; // 0x58

		//
		// Custom functions.
		//

		void copy(UVController* to) const;

	};
	static_assert(sizeof(UVController) == 0x5C, "NI::UVController failed size validation");
}
