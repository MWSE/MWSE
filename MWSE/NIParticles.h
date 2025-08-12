#pragma once

#include "NITriBasedGeometry.h"

namespace NI {
	struct ParticlesData : TriBasedGeometryData {
		float radius; // 0x38
		unsigned short activeCount; // 0x3C
		float* sizes; // 0x40

		static Pointer<ParticlesData> create(unsigned short _vertexCount, TES3::Vector3* _vertices, TES3::Vector3* _normals, PackedColor* _colors);
		static Pointer<ParticlesData> create(unsigned short vertexCount, bool hasNormals, bool hasColors);

		//
		// Accessor methods.
		//

		nonstd::span<float> getSizes();
	};
	static_assert(sizeof(ParticlesData) == 0x44, "NI::ParticlesData failed size validation");

	struct AutoNormalParticlesData : ParticlesData {
		static Pointer<AutoNormalParticlesData> create(unsigned short _vertexCount, TES3::Vector3* _vertices, PackedColor* _colors);
		static Pointer<AutoNormalParticlesData> create(unsigned short vertexCount, bool hasColors);
	};
	static_assert(sizeof(AutoNormalParticlesData) == 0x44, "NI::AutoNormalParticlesData failed size validation");

	struct RotatingParticlesData : ParticlesData {
		Quaternion* rotations; // 0x44

		//
		// Accessor methods.
		//

		nonstd::span<NI::Quaternion> getRotations();
	};
	static_assert(sizeof(RotatingParticlesData) == 0x48, "NI::RotatingParticlesData failed size validation");

	struct Particles : TriBasedGeometry {
		AVObject* staticBoundPositionSource; // 0xAC

		Particles(ParticlesData* data);

		static Pointer<Particles> create(unsigned short vertexCount, bool hasNormals, bool hasColors);

		//
		// Accessor methods.
		//

		ParticlesData* getModelData() const { return static_cast<ParticlesData*>(modelData.get()); }
	};
	static_assert(sizeof(Particles) == 0xB0, "NI::Particles failed size validation");

	struct AutoNormalParticles : Particles {
		AutoNormalParticles(AutoNormalParticlesData* data);

		static Pointer<AutoNormalParticles> create(unsigned short vertexCount, bool hasColors);

		//
		// Accessor methods.
		//

		AutoNormalParticlesData* getModelData() const { return static_cast<AutoNormalParticlesData*>(modelData.get()); }
	};
	static_assert(sizeof(AutoNormalParticles) == 0xB0, "NI::AutoNormalParticles failed size validation");

	struct RotatingParticles : Particles {
		RotatingParticles(RotatingParticlesData* data);

		//
		// Accessor methods.
		//

		RotatingParticlesData* getModelData() const { return static_cast<RotatingParticlesData*>(modelData.get()); }
	};
	static_assert(sizeof(RotatingParticles) == 0xB0, "NI::RotatingParticles failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Particles)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::AutoNormalParticles)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::RotatingParticles)
