#pragma once

#include "TES3BaseObject.h"

#include "NIIteratedList.h"

namespace se::cs {
	struct Region : BaseObject {
		char id[32]; // 0x10
		char name[32]; // 0x30
		unsigned char weatherChances[10]; // 0x50
		int unknown_0x5C;
		NI::IteratedList<void*> unknown_0x60;
		int unknown_0x74;
	};
	static_assert(sizeof(Region) == 0x78, "Region failed size validation");
}
