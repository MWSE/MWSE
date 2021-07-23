#pragma once

#include "TES3Vectors.h"

namespace NI {
	struct PerParticleData {
		TES3::Vector3 velocity; // 0x0
		TES3::Vector3 rotationAxis; // 0xC
		float age; // 0x18
		float lifeSpan; // 0x1C
		float lastUpdate; // 0x20
		unsigned short generation; // 0x24
		unsigned short index; // 0x26
	};
	static_assert(sizeof(PerParticleData) == 0x28, "NI::PerParticleData failed size validation");
}
