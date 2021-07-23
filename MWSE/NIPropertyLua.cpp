#include "NIPropertyLua.h"
#include "NIObjectLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIRTTI.h"
#include "NIProperty.h"
#include "NITexture.h"

namespace mwse {
	namespace lua {
		void bindNIProperties() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for NI::Property.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::Property>("niProperty");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::ObjectNET, NI::Object>();
				setUserdataForNIProperty(usertypeDefinition);
			}

			// Binding for NI::AlphaProperty.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::AlphaProperty>("niAlphaProperty");
				usertypeDefinition["new"] = &NI::AlphaProperty::create;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Property, NI::ObjectNET, NI::Object>();
				setUserdataForNIProperty(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["alphaTestRef"] = &NI::AlphaProperty::alphaTestRef;
			}

			// Binding for NI::FogProperty.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::FogProperty>("niFogProperty");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Property, NI::ObjectNET, NI::Object>();
				setUserdataForNIProperty(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["color"] = sol::readonly_property(&NI::FogProperty::getColor);
				usertypeDefinition["density"] = &NI::FogProperty::density;
			}

			// Binding for NI::MaterialProperty.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::MaterialProperty>("niMaterialProperty");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Property, NI::ObjectNET, NI::Object>();
				setUserdataForNIProperty(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["alpha"] = sol::property(&NI::MaterialProperty::getAlpha, &NI::MaterialProperty::setAlpha);
				usertypeDefinition["ambient"] = sol::property(&NI::MaterialProperty::getAmbient, &NI::MaterialProperty::setAmbient_lua);
				usertypeDefinition["diffuse"] = sol::property(&NI::MaterialProperty::getDiffuse, &NI::MaterialProperty::setDiffuse_lua);
				usertypeDefinition["emissive"] = sol::property(&NI::MaterialProperty::getEmissive, &NI::MaterialProperty::setEmissive_lua);
				usertypeDefinition["shininess"] = sol::property(&NI::MaterialProperty::getShininess, &NI::MaterialProperty::setShininess);
				usertypeDefinition["specular"] = sol::property(&NI::MaterialProperty::getSpecular, &NI::MaterialProperty::setSpecular_lua);

				// Basic function binding.
				usertypeDefinition["incrementRevisionId"] = &NI::MaterialProperty::incrementRevisionId;
			}

			// Binding for NI::StencilProperty.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::StencilProperty>("niStencilProperty");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Property, NI::ObjectNET, NI::Object>();
				setUserdataForNIProperty(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["drawMode"] = &NI::StencilProperty::drawMode;
				usertypeDefinition["enabled"] = &NI::StencilProperty::enabled;
				usertypeDefinition["failAction"] = &NI::StencilProperty::failAction;
				usertypeDefinition["mask"] = &NI::StencilProperty::mask;
				usertypeDefinition["passAction"] = &NI::StencilProperty::passAction;
				usertypeDefinition["testFunc"] = &NI::StencilProperty::testFunc;
				usertypeDefinition["reference"] = &NI::StencilProperty::reference;
				usertypeDefinition["zFailAction"] = &NI::StencilProperty::zFailAction;
			}

			// Binding for NI::TexturingProperty::Map.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::TexturingProperty::Map>("niTexturingPropertyMap");
				usertypeDefinition["new"] = &NI::TexturingProperty::Map::create;

				// Basic property binding.
				usertypeDefinition["clampMode"] = &NI::TexturingProperty::Map::clampMode;
				usertypeDefinition["filterMode"] = &NI::TexturingProperty::Map::filterMode;
				usertypeDefinition["texCoordSet"] = &NI::TexturingProperty::Map::texCoordSet;

				// Properties that need extra work before returning.
				usertypeDefinition["texture"] = sol::property(&NI::TexturingProperty::Map::getTexture_lua, &NI::TexturingProperty::Map::setTexture_lua);
			}

			// Binding for NI::TexturingProperty.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::TexturingProperty>("niTexturingProperty");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Property, NI::ObjectNET, NI::Object>();
				setUserdataForNIProperty(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["applyMode"] = &NI::TexturingProperty::applyMode;
				usertypeDefinition["canAddDecal"] = sol::readonly_property(&NI::TexturingProperty::canAddDecalMap);
				usertypeDefinition["decalCount"] = sol::readonly_property(&NI::TexturingProperty::getDecalCount);
				usertypeDefinition["maps"] = sol::readonly_property(&NI::TexturingProperty::maps);

				// Convenient access to (non-decal) maps.
				usertypeDefinition["baseMap"] = sol::property(&NI::TexturingProperty::getBaseMap, &NI::TexturingProperty::setBaseMap);
				usertypeDefinition["bumpMap"] = sol::property(&NI::TexturingProperty::getBumpMap, &NI::TexturingProperty::setBumpMap);
				usertypeDefinition["darkMap"] = sol::property(&NI::TexturingProperty::getDarkMap, &NI::TexturingProperty::setDarkMap);
				usertypeDefinition["detailMap"] = sol::property(&NI::TexturingProperty::getDetailMap, &NI::TexturingProperty::setDetailMap);
				usertypeDefinition["glossMap"] = sol::property(&NI::TexturingProperty::getGlossMap, &NI::TexturingProperty::setGlossMap);
				usertypeDefinition["glowMap"] = sol::property(&NI::TexturingProperty::getGlowMap, &NI::TexturingProperty::setGlowMap);

				// Basic function binding.
				usertypeDefinition["addDecalMap"] = &NI::TexturingProperty::addDecalMap_lua;
				usertypeDefinition["removeDecalMap"] = &NI::TexturingProperty::removeDecal_lua;
			}

			// Binding for NI::VertexColorProperty.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::VertexColorProperty>("niVertexColorProperty");
				usertypeDefinition["new"] = &NI::VertexColorProperty::create;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Property, NI::ObjectNET, NI::Object>();
				setUserdataForNIProperty(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["lighting"] = &NI::VertexColorProperty::lighting;
				usertypeDefinition["source"] = &NI::VertexColorProperty::source;
			}

			// Binding for NI::ZBufferProperty.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<NI::ZBufferProperty>("niZBufferProperty");
				usertypeDefinition["new"] = &NI::ZBufferProperty::create;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<NI::Property, NI::ObjectNET, NI::Object>();
				setUserdataForNIProperty(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["testFunction"] = &NI::ZBufferProperty::testFunction;

				// Deprecated bindings.
				usertypeDefinition["mask"] = &NI::ZBufferProperty::testFunction;
			}

		}
	}
}
