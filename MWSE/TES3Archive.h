#pragma once

#include "TES3Stream.h"

#include "TES3CriticalSection.h"

namespace TES3 {
	struct Archive : Stream {
		char path[128]; // 0xB0
		int unknown_0x130;
		int unknown_0x134;
		int unknown_0x138;
		int unknown_0x13C;
		int unknown_0x140;
		int unknown_0x144;
		int unknown_0x148;
		int unknown_0x14C;
		Archive* nextArchive; // 0x150
		CriticalSection criticalSection; // 0x154
	};
	static_assert(sizeof(Archive) >= 0x178, "TES3::Archive failed size validation");
}
