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

		// Creates a new PointLight using the engine allocator and initializes
		// attenuation defaults. Implemented only in Morrowind-context builds.
		static Pointer<PointLight> create();

		float getAttenuationAtDistance(float distance) const;
		// Point parameter uses NI::Vector3 layout, which is identical to TES3::Vector3.
		float getAttenuationAtPoint(const Vector3* point) const;

		// Sets attenuation coefficients from Morrowind's per-game-settings globals.
		// Requires TES3::DataHandler — implemented only in Morrowind-context builds.
		void setAttenuationForRadius(unsigned int radius);

		// Morrowind stores dynamic-cull radius in the specular channel.
		unsigned int getRadius() const;
		void setRadius(unsigned int radius);

		// Returns a weight for light-count-overflow sorting.
		// Requires NI::Node::getLightCount — implemented only in Morrowind-context builds.
		unsigned int getSortWeight() const;
	};
	static_assert(sizeof(PointLight) == 0xDC, "NI::PointLight failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::PointLight)
#endif
