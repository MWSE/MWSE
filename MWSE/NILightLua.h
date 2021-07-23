#pragma once

#include "NIDynamicEffectLua.h"

#include "NIColor.h"

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForNILight(sol::usertype<T>& usertypeDefinition) {
			setUserdataForNIDynamicEffect(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["ambient"] = sol::property(&NI::Light::getAmbientColor, &NI::Light::setAmbientColor_lua);
			usertypeDefinition["diffuse"] = sol::property(&NI::Light::getDiffuseColor, &NI::Light::setDiffuseColor_lua);
			usertypeDefinition["dimmer"] = sol::property(&NI::Light::getDimmer, &NI::Light::setDimmer);
			usertypeDefinition["specular"] = sol::property(&NI::Light::getSpecularColor, &NI::Light::setSpecularColor_lua);
		}

		template <typename T>
		void setUserdataForNIPointLight(sol::usertype<T>& usertypeDefinition) {
			setUserdataForNILight(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["constantAttenuation"] = &NI::PointLight::constantAttenuation;
			usertypeDefinition["linearAttenuation"] = &NI::PointLight::linearAttenuation;
			usertypeDefinition["quadraticAttenuation"] = &NI::PointLight::quadraticAttenuation;

			// Basic function binding.
			usertypeDefinition["setAttenuationForRadius"] = &NI::PointLight::setAttenuationForRadius;
			usertypeDefinition["setRadius"] = &NI::PointLight::setRadius;
		}

		void bindNILight();
	}
}
