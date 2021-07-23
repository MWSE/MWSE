#pragma once

#include "NIDynamicEffect.h"
#include "NIColor.h"

namespace NI {
	struct Light : DynamicEffect {
		float dimmer; // 0xA8
		Color ambient; // 0xAC
		Color diffuse; // 0xB8
		Color specular; // 0xC4

		//
		// Other related this-call functions.
		//

		Light * ctor();

		//
		// Custom functions.
		//

		float getDimmer() const;
		void setDimmer(float value);

		Color& getAmbientColor();
		void setAmbientColor(const Color& color);
		void setAmbientColor_lua(sol::object object);

		Color& getDiffuseColor();
		void setDiffuseColor(const Color& color);
		void setDiffuseColor_lua(sol::object object);

		Color& getSpecularColor();
		void setSpecularColor(const Color& color);
		void setSpecularColor_lua(sol::object object);

	};
	static_assert(sizeof(Light) == 0xD0, "NI::Light failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Light)
