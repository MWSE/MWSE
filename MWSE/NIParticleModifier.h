#pragma once

#include "NIObject.h"

#include "TES3Vectors.h"

namespace NI {
	struct ParticleModifier : Object {
		ParticleModifier* nextModifier; // 0x8
		Object* controller; // 0xC
	};
	static_assert(sizeof(ParticleModifier) == 0x10, "NI::ParticleModifier failed size validation");

	struct Gravity : ParticleModifier {
		enum class ForceType : unsigned int {
			Planar = 0,
			Spherical = 1,
		};

		float decay; // 0x10
		float force; // 0x14
		ForceType forceType; // 0x18
		TES3::Vector3 position; // 0x1C
		TES3::Vector3 direction; // 0x28
	};
	static_assert(sizeof(Gravity) == 0x34, "NI::Gravity failed size validation");

	struct ParticleBomb : ParticleModifier {
		enum class DecayType : unsigned int {
			None = 0,
			Linear = 1,
			Exponential = 2,
		};
		enum class SymmetryType : unsigned int {
			Spherical = 0,
			Cylindrical = 1,
			Planar = 2,
		};

		float decay; // 0x10
		float duration; // 0x14
		float deltaV; // 0x18
		float start; // 0x1C
		DecayType decayType; // 0x20
		SymmetryType symmetryType; // 0x24
		TES3::Vector3 position; // 0x28
		TES3::Vector3 direction; // 0x34
	};
	static_assert(sizeof(ParticleBomb) == 0x40, "NI::ParticleBomb failed size validation");

	struct ParticleCollider : ParticleModifier {
		float restitution; // 0x10
		bool spawnOnCollide; // 0x14
		bool dieOnCollide; // 0x15
		TES3::Vector3 collisionPoint; // 0x18
		float collisionTime; // 0x24
	};
	static_assert(sizeof(ParticleCollider) == 0x28, "NI::ParticleCollider failed size validation");

	struct PlanarCollider : ParticleCollider {
		float width; // 0x28
		float length; // 0x2C
		TES3::Vector4 planeEquation; // 0x30
		TES3::Vector3 center; // 0x40
		TES3::Vector3 lengthAxis; // 0x4C
		TES3::Vector3 widthAxis; // 0x58
	};
	static_assert(sizeof(PlanarCollider) == 0x64, "NI::PlanarCollider failed size validation");

	struct SphericalCollider : ParticleCollider {
		float radius; // 0x28
		float radiusSquared; // 0x2C
		TES3::Vector3 position; // 0x30
	};
	static_assert(sizeof(SphericalCollider) == 0x3C, "NI::SphericalCollider failed size validation");

	struct ParticleColorModifier : ParticleModifier {
		Pointer<Object> colorData; // 0x10
	};
	static_assert(sizeof(ParticleColorModifier) == 0x14, "NI::ParticleColorModifier failed size validation");

	struct ParticleGrowFade : ParticleModifier {
		float grow; // 0x10
		float fade; // 0x14
	};
	static_assert(sizeof(ParticleGrowFade) == 0x18, "NI::ParticleGrowFade failed size validation");

	struct ParticleRotation : ParticleModifier {
		bool randomInitialAxis; // 0x10
		TES3::Vector3 initialAxis; // 0x14
		float rotationSpeed; // 0x20
	};
	static_assert(sizeof(ParticleRotation) == 0x24, "NI::ParticleRotation failed size validation");
}
