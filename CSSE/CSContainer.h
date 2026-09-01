#pragma once

#include "CSActor.h"

#include "TES3ContainerFlags.h"

namespace se::cs {
	struct Container : Actor {
		int unknown_0x7C;
		int unknown_0x80;
		const char* name; // 0x84
		const char* model; // 0x88
		Script* script; // 0x8C
		float capacity; // 0x90

		bool getIsOrganic() const;
	};
	static_assert(sizeof(Container) == 0x94, "Container failed size validation");
}
