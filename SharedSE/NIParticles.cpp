#include "NIParticles.h"

namespace NI {
	nonstd::span<float> ParticlesData::getSizes() {
		if (sizes) { return nonstd::span(sizes, vertexCount); }
		return {};
	}

	nonstd::span<NI::Quaternion> RotatingParticlesData::getRotations() {
		if (rotations) { return nonstd::span(rotations, vertexCount); }
		return {};
	}

	Pointer<ParticlesData> Particles::getModelData() const {
		return static_cast<ParticlesData*>(modelData.get());
	}

	Pointer<AutoNormalParticlesData> AutoNormalParticles::getModelData() const {
		return static_cast<AutoNormalParticlesData*>(modelData.get());
	}

	Pointer<RotatingParticlesData> RotatingParticles::getModelData() const {
		return static_cast<RotatingParticlesData*>(modelData.get());
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Particles)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::AutoNormalParticles)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::RotatingParticles)
#endif
