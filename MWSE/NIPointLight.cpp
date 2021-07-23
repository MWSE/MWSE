#include "NIPointLight.h"

#include "TES3DataHandler.h"

#include "MemoryUtil.h"

namespace TES3 {
	enum LightAttenuationFlag {
		UseConstant = 0x1,
		UseLinear = 0x2,
		UseQuadratic = 0x4,
	};

	unsigned int& LightAttenuation_Flags = *reinterpret_cast<unsigned int*>(0x7CB1C0);

	float& LightAttenuation_ConstantValue = *reinterpret_cast<float*>(0x7CB1D4);

	int& LightAttenuation_LinearMethod = *reinterpret_cast<int*>(0x7CB1C4);
	float& LightAttenuation_LinearValue = *reinterpret_cast<float*>(0x7CB1CC);
	float& LightAttenuation_LinearRadiusMultiplier = *reinterpret_cast<float*>(0x7CB1DC);

	int& LightAttenuation_QuadraticMethod = *reinterpret_cast<int*>(0x7CB1C8);
	bool& LightAttenuation_QuadraticInLinear = *reinterpret_cast<bool*>(0x7CB1D8);
	float& LightAttenuation_QuadraticValue = *reinterpret_cast<float*>(0x7CB1D0);
	float& LightAttenuation_QuadraticRadiusMultiplier = *reinterpret_cast<float*>(0x7CB1E0);
}

namespace NI {
	Pointer<PointLight> PointLight::create() {
		PointLight * light = mwse::tes3::_new<PointLight>();
		light->ctor();

		light->vTable.asDynamicEffect = reinterpret_cast<DynamicEffect_vTable*>(0x749D40);
		light->constantAttenuation = 0.0f;
		light->linearAttenuation = 1.0f;
		light->quadraticAttenuation = 0.0f;

		return light;
	}

	void PointLight::setRadius(unsigned int radius) {
		// Set light attenuation.
		setAttenuationForRadius(radius);

		// Set dynamic culling radius, misplaced by Morrowind into specular.
		auto r = float(radius);
		specular.r = r;
		specular.g = r;
		specular.b = r;
	}

	void PointLight::setAttenuationForRadius(unsigned int radius) {
		// Get constant attenuation.
		if (TES3::LightAttenuation_Flags & TES3::LightAttenuationFlag::UseConstant) {
			constantAttenuation = TES3::LightAttenuation_ConstantValue;
		}
		else {
			constantAttenuation = 0.0f;
		}

		// Get linear attenuation.
		auto dataHandler = TES3::DataHandler::get();
		if (TES3::LightAttenuation_Flags & TES3::LightAttenuationFlag::UseLinear || (dataHandler->currentInteriorCell != nullptr && TES3::LightAttenuation_QuadraticInLinear)) {
			float linearRadius = radius * TES3::LightAttenuation_LinearRadiusMultiplier;
			float linear = 0.01f;
			if (TES3::LightAttenuation_LinearMethod == 0) {
				linear = TES3::LightAttenuation_LinearValue;
			}
			else if (TES3::LightAttenuation_LinearMethod == 1 && linearRadius != 0.0f) {
				linear = (1.0f / linearRadius) * TES3::LightAttenuation_LinearValue;
			}
			else if (TES3::LightAttenuation_LinearMethod == 2 && linearRadius != 0.0f) {
				linear = TES3::LightAttenuation_LinearValue / (linearRadius * linearRadius);
			}
			linearAttenuation = linear;
		}
		else {
			linearAttenuation = 0.0f;
		}

		// Get quadratic attenuation.
		if (TES3::LightAttenuation_Flags & TES3::LightAttenuationFlag::UseQuadratic || ((dataHandler->currentInteriorCell == nullptr && TES3::LightAttenuation_QuadraticInLinear))) {
			float quadraticRadius = radius * TES3::LightAttenuation_QuadraticRadiusMultiplier;
			float quadratic = 0.01f;
			if (TES3::LightAttenuation_QuadraticMethod == 0) {
				quadratic = TES3::LightAttenuation_QuadraticValue;
			}
			else if (TES3::LightAttenuation_QuadraticMethod == 1 && quadraticRadius != 0.0f) {
				quadratic = (1.0f / radius) * TES3::LightAttenuation_QuadraticValue;
			}
			else if (TES3::LightAttenuation_QuadraticMethod == 2 && quadraticRadius != 0.0f) {
				quadratic = TES3::LightAttenuation_QuadraticValue / (quadraticRadius * quadraticRadius);
			}
			quadraticAttenuation = quadratic;
		}
		else {
			quadraticAttenuation = 0.0f;
		}

		revisionId++;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::PointLight)
