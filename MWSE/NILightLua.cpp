#include "NILightLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NINode.h"
#include "NIAmbientLight.h"
#include "NIDirectionalLight.h"
#include "NIRTTI.h"
#include "NISpotLight.h"

namespace mwse {
	namespace lua {
		void bindNILight() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for NI::Light.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::Light>("niLight");
				usertypeDefinition["new"] = sol::no_constructor;

				// Inherit NI::DynamicEffect.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Object, NI::ObjectNET, NI::AVObject, NI::DynamicEffect>();
				setUserdataForNILight(usertypeDefinition);
			}

			// Binding for NI::AmbientLight.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::AmbientLight>("niAmbientLight");
				usertypeDefinition["new"] = sol::no_constructor;

				// Inherit NI::Light.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Object, NI::ObjectNET, NI::AVObject, NI::DynamicEffect, NI::Light>();
				setUserdataForNILight(usertypeDefinition);
			}

			// Binding for NI::DirectionalLight.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::DirectionalLight>("niDirectionalLight");
				usertypeDefinition["new"] = sol::no_constructor;

				// Inherit NI::Light.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Object, NI::ObjectNET, NI::AVObject, NI::DynamicEffect, NI::Light>();
				setUserdataForNILight(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["direction"] = &NI::DirectionalLight::direction;
			}

			// Binding for NI::PointLight.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::PointLight>("niPointLight");
				usertypeDefinition["new"] = &NI::PointLight::create;

				// Inherit NI::Light.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Object, NI::ObjectNET, NI::AVObject, NI::DynamicEffect, NI::Light>();
				setUserdataForNIPointLight(usertypeDefinition);
			}

			// Binding for NI::SpotLight.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::SpotLight>("niSpotLight");
				usertypeDefinition["new"] = sol::no_constructor;

				// Inherit NI::Light.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Object, NI::ObjectNET, NI::AVObject, NI::DynamicEffect, NI::Light, NI::PointLight>();
				setUserdataForNIPointLight(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["direction"] = &NI::SpotLight::direction;
				usertypeDefinition["spotAngle"] = &NI::SpotLight::spotAngle;
				usertypeDefinition["spotExponent"] = &NI::SpotLight::spotExponent;
			}
		}
	}
}
