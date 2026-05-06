#include "NIParticleModifier.h"

namespace NI {
	float SphericalCollider::getRadius() const {
		return radius;
	}

	void SphericalCollider::setRadius(float r) {
		radius = r;
		radiusSquared = r * r;
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ParticleModifier)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Gravity)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ParticleBomb)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ParticleColorModifier)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ParticleGrowFade)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ParticleRotation)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ParticleCollider)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::PlanarCollider)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::SphericalCollider)
#endif
