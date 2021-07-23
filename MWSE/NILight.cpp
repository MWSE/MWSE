#include "NILight.h"

namespace NI {
	const auto NI_Light_ctor = reinterpret_cast<Light*(__thiscall*)(Light*)>(0x6F9940);
	Light * Light::ctor() {
		return NI_Light_ctor(this);
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

	void Light::setAmbientColor_lua(sol::object object) {
		ambient = object;
		revisionId++;
	}

	Color& Light::getDiffuseColor() {
		return diffuse;
	}

	void Light::setDiffuseColor(const Color& color) {
		diffuse = color;
		revisionId++;
	}

	void Light::setDiffuseColor_lua(sol::object object) {
		diffuse = object;
		revisionId++;
	}

	Color& Light::getSpecularColor() {
		return specular;
	}

	void Light::setSpecularColor(const Color& color) {
		specular = color;
		revisionId++;
	}

	void Light::setSpecularColor_lua(sol::object object) {
		specular = object;
		revisionId++;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Light)
