#pragma once

#include "NIDynamicEffect.h"
#include "NIColor.h"

namespace NI {
	struct Light : DynamicEffect {
		float dimmer; // 0xA8
		Color ambient; // 0xAC
		Color diffuse; // 0xB8
		Color specular; // 0xC4

		// MWSE-pattern relocatable construction; impl in MWSE-private NILight.cpp.
		// CSSE doesn't currently call this.
		Light* ctor();

		//
		// Custom functions.
		//

		float getDimmer() const;
		void setDimmer(float value);

		Color& getAmbientColor();
		void setAmbientColor(const Color& color);

		Color& getDiffuseColor();
		void setDiffuseColor(const Color& color);

		Color& getSpecularColor();
		void setSpecularColor(const Color& color);

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		void setAmbientColor_lua(sol::object object);
		void setDiffuseColor_lua(sol::object object);
		void setSpecularColor_lua(sol::object object);
#endif
	};
	static_assert(sizeof(Light) == 0xD0, "NI::Light failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Light)
#endif
