#pragma once

#include "NIFile.h"

namespace se::cs {
	struct File : NI::File {
		bool readAndWrite; // 0x20
		char path[260]; // 0x21
		size_t seekPosition; // 0x128
		int unknown_0x12C;
	};
}
