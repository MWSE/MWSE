#pragma once

#include "NITimeController.h"

#include "NIParticleModifier.h"

#include "TES3Cell.h"

namespace NI {
	struct ParticleSystemController : TimeController {
		int unknown_0x34;
		float speed; // 0x38
		float speedVariation; // 0x3C
		float declinationAngle; // 0x40
		float declinationAngleVariation; // 0x44
		float planarAngle; // 0x48
		float planarAngleVariation; // 0x4C
		TES3::Vector3 initialNormal; // 0x50
		PackedColor initialColor; // 0x5C
		float initialSize; // 0x60
		float emitStartTime; // 0x64
		float emitStopTime; // 0x68
		bool resetParticleSystem; // 0x6C
		float birthRate; // 0x70
		float lifespan; // 0x74
		float lifespanVariance; // 0x78
		bool useBirthRate; // 0x7C
		bool spawnOnDeath; // 0x7D
		float emitterWidth; // 0x80
		float emitterHeight; // 0x84
		float emitterDepth; // 0x8C
		int unknown_0x8C;
		unsigned short generationCount; // 0x90
		float spawnPercentage; // 0x94
		unsigned short spawnMultiplier; // 0x98
		float speedChaos; // 0x9C
		float directionChaos; // 0xA0
		unsigned short particleDataCount; // 0xA4
		unsigned short activeParticleCount; // 0xA6
		short unknown_0xA8;
		PerParticleData* particleData; // 0xAC
		Pointer<Object> emitterModifier; // 0xB0
		Pointer<ParticleModifier> particleModifier; // 0xB4
		Pointer<ParticleCollider> particleCollider; // 0xB8
		bool computeDynamicBoundingVolume; // 0xBC
		unsigned char unknown_0xBD;
		int unknown_0xC0;
	};
	static_assert(sizeof(ParticleSystemController) == 0xC4, "NI::ParticleSystemController failed size validation");
}
