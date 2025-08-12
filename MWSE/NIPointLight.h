#pragma once

#include "NILight.h"

namespace NI {
	struct PointLight : Light {
		float constantAttenuation; // 0xD0
		float linearAttenuation; // 0xD4
		float quadraticAttenuation; // 0xD8

		//
		// Custom functions.
		//

		static Pointer<PointLight> create();

		float getAttenuationAtDistance(float distance) const;
		float getAttenuationAtPoint(const TES3::Vector3* point) const;
		void setAttenuationForRadius(unsigned int radius);
		unsigned int getRadius() const;
		void setRadius(unsigned int radius);

		unsigned int getSortWeight() const;

	};
	static_assert(sizeof(PointLight) == 0xDC, "NI::PointLight failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::PointLight)
