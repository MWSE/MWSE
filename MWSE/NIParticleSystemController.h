#pragma once

#include "NITimeController.h"

#include "NIParticleModifier.h"

#include "TES3Cell.h"

namespace NI {
	struct ParticleSystemController : TimeController {
		float scaledLastTime; // 0x34
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
		float emitterDepth; // 0x88
		Pointer<Object> emitter; // 0x8C
		unsigned short spawnGenerationsCount; // 0x90
		float spawnPercentage; // 0x94
		unsigned short spawnMultiplier; // 0x98
		float spawnSpeedChaos; // 0x9C
		float spawnDirectionChaos; // 0xA0
		unsigned short particleDataCount; // 0xA4
		unsigned short activeParticleCount; // 0xA6
		unsigned short currentParticleIndex; // 0xA8
		PerParticleData* particleData; // 0xAC
		Pointer<Object> emitterModifiers; // 0xB0
		Pointer<ParticleModifier> particleModifiers; // 0xB4
		Pointer<ParticleCollider> particleColliders; // 0xB8
		bool staticBounds; // 0xBC
		bool firstTime; // 0xBD
		float lastEmit; // 0xC0

		nonstd::span<PerParticleData> getPerParticleData();
	};
	static_assert(sizeof(ParticleSystemController) == 0xC4, "NI::ParticleSystemController failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::ParticleSystemController)