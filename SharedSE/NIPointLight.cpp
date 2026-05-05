#include "NIPointLight.h"

namespace NI {
	unsigned int PointLight::getRadius() const {
		// Morrowind stores the dynamic-cull radius in the specular channel.
		return static_cast<unsigned int>(specular.r);
	}

	float PointLight::getAttenuationAtDistance(float distance) const {
		const auto Qd2 = quadraticAttenuation * distance * distance;
		const auto Ld = linearAttenuation * distance;
		const auto C = constantAttenuation;
		return 1.0f / (C + Ld + Qd2);
	}

	float PointLight::getAttenuationAtPoint(const Vector3* point) const {
		const auto distance = worldTransform.translation.distance(point);
		return getAttenuationAtDistance(distance);
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::PointLight)
#endif
