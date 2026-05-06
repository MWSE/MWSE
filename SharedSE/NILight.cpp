#include "NILight.h"

#include "ExceptionUtil.h"

namespace NI {
	Light* Light::ctor() {
#if defined(SE_NI_LIGHT_FNADDR_CTOR) && SE_NI_LIGHT_FNADDR_CTOR > 0
		const auto NI_Light_ctor = reinterpret_cast<Light*(__thiscall*)(Light*)>(SE_NI_LIGHT_FNADDR_CTOR);
		return NI_Light_ctor(this);
#else
		throw not_implemented_exception();
#endif
	}

	float Light::getDimmer() const {
		return dimmer;
	}

	void Light::setDimmer(float value) {
		dimmer = value;
		revisionId++;
	}

	Color& Light::getAmbientColor() {
		return ambient;
	}

	void Light::setAmbientColor(const Color& color) {
		ambient = color;
		revisionId++;
	}

	Color& Light::getDiffuseColor() {
		return diffuse;
	}

	void Light::setDiffuseColor(const Color& color) {
		diffuse = color;
		revisionId++;
	}

	Color& Light::getSpecularColor() {
		return specular;
	}

	void Light::setSpecularColor(const Color& color) {
		specular = color;
		revisionId++;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void Light::setAmbientColor_lua(sol::object object) {
		ambient = object;
		revisionId++;
	}

	void Light::setDiffuseColor_lua(sol::object object) {
		diffuse = object;
		revisionId++;
	}

	void Light::setSpecularColor_lua(sol::object object) {
		specular = object;
		revisionId++;
	}
#endif
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Light)
#endif
