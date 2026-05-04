#pragma once

#include "NITriBasedGeometry.h"
#include "NIVector3.h"

namespace NI {
	struct ParticlesData : TriBasedGeometryData {
		float radius; // 0x38
		unsigned short activeCount; // 0x3C
		float* sizes; // 0x40

		// Implemented only in Morrowind-context builds (use engine allocator + ctor).
		static Pointer<ParticlesData> create(unsigned short vertexCount, Vector3* vertices, Vector3* normals, PackedColor* colors);
		static Pointer<ParticlesData> create(unsigned short vertexCount, bool hasNormals, bool hasColors);

		nonstd::span<float> getSizes();
	};
	static_assert(sizeof(ParticlesData) == 0x44, "NI::ParticlesData failed size validation");

	struct AutoNormalParticlesData : ParticlesData {
		// Implemented only in Morrowind-context builds.
		static Pointer<AutoNormalParticlesData> create(unsigned short vertexCount, Vector3* vertices, PackedColor* colors);
		static Pointer<AutoNormalParticlesData> create(unsigned short vertexCount, bool hasColors);
	};
	static_assert(sizeof(AutoNormalParticlesData) == 0x44, "NI::AutoNormalParticlesData failed size validation");

	struct RotatingParticlesData : ParticlesData {
		Quaternion* rotations; // 0x44

		nonstd::span<NI::Quaternion> getRotations();
	};
	static_assert(sizeof(RotatingParticlesData) == 0x48, "NI::RotatingParticlesData failed size validation");

	struct Particles : TriBasedGeometry {
		AVObject* staticBoundPositionSource; // 0xAC

		Particles(ParticlesData* data);
		static Pointer<Particles> create(unsigned short vertexCount, bool hasNormals, bool hasColors);

		Pointer<ParticlesData> getModelData() const;
	};
	static_assert(sizeof(Particles) == 0xB0, "NI::Particles failed size validation");

	struct AutoNormalParticles : Particles {
		AutoNormalParticles(AutoNormalParticlesData* data);
		static Pointer<AutoNormalParticles> create(unsigned short vertexCount, bool hasColors);

		Pointer<AutoNormalParticlesData> getModelData() const;
	};
	static_assert(sizeof(AutoNormalParticles) == 0xB0, "NI::AutoNormalParticles failed size validation");

	struct RotatingParticles : Particles {
		RotatingParticles(RotatingParticlesData* data);

		Pointer<RotatingParticlesData> getModelData() const;
	};
	static_assert(sizeof(RotatingParticles) == 0xB0, "NI::RotatingParticles failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Particles)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::AutoNormalParticles)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::RotatingParticles)
#endif
