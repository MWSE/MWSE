#include "MGEUtilLua.h"

#include "MGEApi.h"
#include "MGEPostShaders.h"
#include "TES3Game.h"

#include "LuaManager.h"
#include "LuaUtil.h"
#include "Log.h"

#include "MGEApiLua.h"

namespace mwse::lua {

	//
	// Expose it all to lua.
	//

	bool isMGEAPIAvailable() {
		return mge::api != nullptr;
	}

	void bindMGEUtil() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		sol::table lua_mge = state["mge"];

		// General functions.
		lua_mge["enabled"] = &mge::lua::CoreInterface::enabled;
		lua_mge["getVersion"] = &mge::lua::CoreInterface::getVersion;

		lua_mge["loadConfig"] = &mge::lua::CoreInterface::loadConfig;
		lua_mge["saveConfig"] = &mge::lua::CoreInterface::saveConfig;

		// Distant land functions.
		{
			using DistantLandRenderConfig = mge::DistantLandRenderConfig;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<DistantLandRenderConfig>("mgeDistantLandRenderConfig");
			usertypeDefinition["new"] = sol::no_constructor;

			// Properties.
			usertypeDefinition["aboveWaterFogEnd"] = &DistantLandRenderConfig::AboveWaterFogEnd;
			usertypeDefinition["aboveWaterFogStart"] = &DistantLandRenderConfig::AboveWaterFogStart;
			usertypeDefinition["belowWaterFogEnd"] = &DistantLandRenderConfig::BelowWaterFogEnd;
			usertypeDefinition["belowWaterFogStart"] = &DistantLandRenderConfig::BelowWaterFogStart;
			usertypeDefinition["drawDistance"] = &DistantLandRenderConfig::DrawDist;
			usertypeDefinition["farStaticEnd"] = &DistantLandRenderConfig::FarStaticEnd;
			usertypeDefinition["farStaticMinSize"] = &DistantLandRenderConfig::FarStaticMinSize;
			usertypeDefinition["interiorFogEnd"] = &DistantLandRenderConfig::InteriorFogEnd;
			usertypeDefinition["interiorFogStart"] = &DistantLandRenderConfig::InteriorFogStart;
			usertypeDefinition["nearStaticEnd"] = &DistantLandRenderConfig::NearStaticEnd;
			usertypeDefinition["fogOffsetPercent"] = sol::readonly_property(&DistantLandRenderConfig::getFogOffset);
			usertypeDefinition["fogDistance"] = sol::readonly_property(&DistantLandRenderConfig::getFogDist);
			usertypeDefinition["veryFarStaticEnd"] = &DistantLandRenderConfig::VeryFarStaticEnd;
			usertypeDefinition["veryFarStaticMinSize"] = &DistantLandRenderConfig::VeryFarStaticMinSize;
			usertypeDefinition["waterCaustics"] = &DistantLandRenderConfig::WaterCaustics;
			usertypeDefinition["waterWaveHeight"] = &DistantLandRenderConfig::WaterWaveHeight;
			usertypeDefinition["wind"] = sol::readonly_property(&DistantLandRenderConfig::getWind);
			// Note that DistantLandRenderConfig::ShadowResolution does not appear, as it is not configurable.
		}
		lua_mge["distantLandRenderConfig"] = mge::api->getDistantLandRenderConfig();

		// Core interface functions.
		{
			using CoreInterface = mge::lua::CoreInterface;

			lua_mge["reloadDistantLand"] = &CoreInterface::reloadDistantLand;

			lua_mge["getUIScale"] = &CoreInterface::getGUIScale;
			lua_mge["setUIScale"] = &CoreInterface::setGUIScale;
			lua_mge["getLightingMode"] = &CoreInterface::getLightingMode;
			lua_mge["setLightingMode"] = &CoreInterface::setLightingMode;

			if (mge::apiVersion >= 2) {
				lua_mge["saveScreenshot"] = &CoreInterface::saveScreenshot;
			}
		}

		// Rendering feature functions.
		{
			using RenderFeature = mge::RenderFeature;
			using RenderFeatures = mge::lua::RenderFeatures;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<mge::lua::RenderFeatures>("mgeRenderFeatures");
			usertypeDefinition["new"] = sol::no_constructor;

			RenderFeatures::bindFeature<RenderFeature::AtmosphericScattering>(usertypeDefinition, "atmosphericScattering");
			RenderFeatures::bindFeature<RenderFeature::BlurReflections>(usertypeDefinition, "blurReflections");
			RenderFeatures::bindFeature<RenderFeature::CrosshairAutoHide>(usertypeDefinition, "crosshairAutoHide");
			RenderFeatures::bindFeature<RenderFeature::DisplayMessages>(usertypeDefinition, "displayMessages");
			RenderFeatures::bindFeature<RenderFeature::DistantLand>(usertypeDefinition, "distantLand");
			RenderFeatures::bindFeature<RenderFeature::DistantStatics>(usertypeDefinition, "distantStatics");
			RenderFeatures::bindFeature<RenderFeature::DistantWater>(usertypeDefinition, "distantWater");
			RenderFeatures::bindFeature<RenderFeature::DynamicRipples>(usertypeDefinition, "dynamicRipples");
			RenderFeatures::bindFeature<RenderFeature::ExponentialFog>(usertypeDefinition, "exponentialFog");
			RenderFeatures::bindFeature<RenderFeature::FPSCounter>(usertypeDefinition, "fpsCounter");
			RenderFeatures::bindFeature<RenderFeature::Grass>(usertypeDefinition, "grass");
			RenderFeatures::bindFeature<RenderFeature::NoMWMGEBlending>(usertypeDefinition, "noMWMGEBlending");
			RenderFeatures::bindFeature<RenderFeature::NoMWSunglare>(usertypeDefinition, "noMWSunglare");
			RenderFeatures::bindFeature<RenderFeature::PauseRenderingInMenus>(usertypeDefinition, "pauseRenderingInMenus");
			RenderFeatures::bindFeature<RenderFeature::ReflectInterior>(usertypeDefinition, "reflectInterior");
			RenderFeatures::bindFeature<RenderFeature::ReflectiveWater>(usertypeDefinition, "reflectiveWater");
			RenderFeatures::bindFeature<RenderFeature::ReflectNearStatics>(usertypeDefinition, "reflectNearStatics");
			RenderFeatures::bindFeature<RenderFeature::ReflectSky>(usertypeDefinition, "reflectSky");
			RenderFeatures::bindFeature<RenderFeature::Shaders>(usertypeDefinition, "shaders");
			RenderFeatures::bindFeature<RenderFeature::Shadows>(usertypeDefinition, "shadows");
			RenderFeatures::bindFeature<RenderFeature::TransparencyAA>(usertypeDefinition, "transparencyAA");
			RenderFeatures::bindFeature<RenderFeature::UpdateHDR>(usertypeDefinition, "updateHDR");
		}
		lua_mge["render"] = mge::lua::RenderFeatures();

		// Shader-related functions.
		{
			using mge::lua::ShadersConfig;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<ShadersConfig>("mgeShadersConfig");
			usertypeDefinition["new"] = sol::no_constructor;

			// Properties.
			usertypeDefinition["list"] = sol::readonly_property(&ShadersConfig::getShaders);
			usertypeDefinition["debug"] = sol::readonly_property(&ShadersConfig::debugShaders);
			usertypeDefinition["hdrReactionSpeed"] = sol::property(&ShadersConfig::getHDRReactionSpeed, &ShadersConfig::setHDRReactionSpeed);

			// Functions.
			usertypeDefinition["find"] = &ShadersConfig::findShader;
			usertypeDefinition["load"] = &ShadersConfig::loadShader;
		}
		lua_mge["shaders"] = mge::lua::ShadersConfig();

		// Camera functions.
		{
			using mge::lua::CameraConfig;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<CameraConfig>("mgeCameraConfig");
			usertypeDefinition["new"] = sol::no_constructor;

			// Properties.
			usertypeDefinition["fov"] = sol::property(&CameraConfig::getFoV, &CameraConfig::setFoV);
			usertypeDefinition["shakeAcceleration"] = sol::property(&CameraConfig::getCameraShakeAcceleration, &CameraConfig::setCameraShakeAcceleration);
			usertypeDefinition["shakeEnable"] = sol::property(&CameraConfig::getCameraShakeEnabled, &CameraConfig::setCameraShakeEnabled);
			usertypeDefinition["shakeMagnitude"] = sol::property(&CameraConfig::getCameraShakeMagnitude, &CameraConfig::setCameraShakeMagnitude);
			usertypeDefinition["stopZoom"] = CameraConfig::stopZoom;
			usertypeDefinition["thirdPersonOffset"] = sol::property(&CameraConfig::getThirdPersonOffset, &CameraConfig::setThirdPersonOffset);
			usertypeDefinition["zoom"] = sol::property(&CameraConfig::getZoom, &CameraConfig::setZoom);
			usertypeDefinition["zoomContinuous"] = CameraConfig::setZoomContinuous;
			usertypeDefinition["zoomEnable"] = sol::property(&CameraConfig::getZoomEnabled, &CameraConfig::setZoomEnabled);
			usertypeDefinition["zoomIn"] = CameraConfig::zoomIn;
			usertypeDefinition["zoomOut"] = CameraConfig::zoomOut;

			if (mge::apiVersion >= 3) {
				usertypeDefinition["nearRenderDistance"] = sol::property(&CameraConfig::getNearRenderDistance, &CameraConfig::setNearRenderDistance);
			}
		}
		lua_mge["camera"] = mge::lua::CameraConfig();

		// MGE XE weather functions.
		{
			using mge::lua::WeatherConfig;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<WeatherConfig>("mgeWeatherConfig");
			usertypeDefinition["new"] = sol::no_constructor;

			// Functions.
			usertypeDefinition["getDistantFog"] = &WeatherConfig::getDLFog;
			usertypeDefinition["setDistantFog"] = &WeatherConfig::setDLFog;
			usertypeDefinition["getPerPixelLighting"] = &WeatherConfig::getPPLLight;
			usertypeDefinition["setPerPixelLighting"] = &WeatherConfig::setPPLLight;
			usertypeDefinition["getScattering"] = &WeatherConfig::getScattering;
			usertypeDefinition["setScattering"] = &WeatherConfig::setScattering;
			usertypeDefinition["getWind"] = &WeatherConfig::getWind;
			usertypeDefinition["setWind"] = &WeatherConfig::setWind;

			if (mge::apiVersion >= 2) {
				usertypeDefinition["getSkylightScattering"] = &WeatherConfig::getSkylightScattering;
				usertypeDefinition["setSkylightScattering"] = &WeatherConfig::setSkylightScattering;
			}
		}
		lua_mge["weather"] = mge::lua::WeatherConfig();

		// Macro functions with on-screen notifications.
		auto lua_macros = lua_mge.create_named("macros");
		lua_macros["decreaseFOV"] = mge::macros->DecreaseFOV;
		lua_macros["decreaseViewRange"] = mge::macros->DecreaseViewRange;
		lua_macros["decreaseZoom"] = mge::macros->DecreaseZoom;
		lua_macros["disableMusic"] = mge::macros->DisableMusic;
		lua_macros["haggleLess1"] = mge::macros->HaggleLess1;
		lua_macros["haggleLess10"] = mge::macros->HaggleLess10;
		lua_macros["haggleLess100"] = mge::macros->HaggleLess100;
		lua_macros["haggleLess1000"] = mge::macros->HaggleLess1000;
		lua_macros["haggleLess10000"] = mge::macros->HaggleLess10000;
		lua_macros["haggleMore1"] = mge::macros->HaggleMore1;
		lua_macros["haggleMore10"] = mge::macros->HaggleMore10;
		lua_macros["haggleMore100"] = mge::macros->HaggleMore100;
		lua_macros["haggleMore1000"] = mge::macros->HaggleMore1000;
		lua_macros["haggleMore10000"] = mge::macros->HaggleMore10000;
		lua_macros["increaseFOV"] = mge::macros->IncreaseFOV;
		lua_macros["increaseViewRange"] = mge::macros->IncreaseViewRange;
		lua_macros["increaseZoom"] = mge::macros->IncreaseZoom;
		lua_macros["moveBack3PCam"] = mge::macros->MoveBack3PCam;
		lua_macros["moveDown3PCam"] = mge::macros->MoveDown3PCam;
		lua_macros["moveForward3PCam"] = mge::macros->MoveForward3PCam;
		lua_macros["moveLeft3PCam"] = mge::macros->MoveLeft3PCam;
		lua_macros["moveRight3PCam"] = mge::macros->MoveRight3PCam;
		lua_macros["moveUp3PCam"] = mge::macros->MoveUp3PCam;
		lua_macros["nextTrack"] = mge::macros->NextTrack;
		lua_macros["resetEnableZoom"] = mge::macros->ResetEnableZoom;
		lua_macros["showLastMessage"] = mge::macros->ShowLastMessage;
		lua_macros["takeScreenshot"] = mge::macros->TakeScreenshot;
		lua_macros["toggleBlending"] = mge::macros->ToggleBlending;
		lua_macros["toggleCrosshair"] = mge::macros->ToggleCrosshair;
		lua_macros["toggleDistantLand"] = mge::macros->ToggleDistantLand;
		lua_macros["toggleFpsCounter"] = mge::macros->ToggleFpsCounter;
		lua_macros["toggleGrass"] = mge::macros->ToggleGrass;
		lua_macros["toggleLightingMode"] = mge::macros->ToggleLightingMode;
		lua_macros["toggleShaders"] = mge::macros->ToggleShaders;
		lua_macros["toggleShadows"] = mge::macros->ToggleShadows;
		lua_macros["toggleStatusText"] = mge::macros->ToggleStatusText;
		lua_macros["toggleTransparencyAA"] = mge::macros->ToggleTransparencyAA;
		lua_macros["toggleZoom"] = mge::macros->ToggleZoom;

		// Legacy functions.
		lua_mge["disableShader"] = &mge::lua::LegacyInterface::disableShader;
		lua_mge["disableZoom"] = &mge::lua::LegacyInterface::disableZoom;
		lua_mge["enableShader"] = &mge::lua::LegacyInterface::enableShader;
		lua_mge["enableZoom"] = &mge::lua::LegacyInterface::enableZoom;
		lua_mge["getScreenHeight"] = &mge::lua::LegacyInterface::getScreenHeight;
		lua_mge["getScreenWidth"] = &mge::lua::LegacyInterface::getScreenWidth;
		lua_mge["getWeatherDLFog"] = &mge::lua::LegacyInterface::getWeatherDLFog;
		lua_mge["getWeatherPPLLight"] = &mge::lua::LegacyInterface::getWeatherPPLLight;
		lua_mge["getWeatherScattering"] = &mge::lua::WeatherConfig::getScattering;
		lua_mge["getZoom"] = &mge::lua::CameraConfig::getZoom;
		lua_mge["setShaderFloat"] = &mge::lua::LegacyInterface::setShaderFloat;
		lua_mge["setShaderLong"] = &mge::lua::LegacyInterface::setShaderLong;
		lua_mge["setWeatherDLFog"] = &mge::lua::LegacyInterface::setWeatherDLFog;
		lua_mge["setWeatherPPLLight"] = &mge::lua::LegacyInterface::setWeatherPPLLight;
		lua_mge["setWeatherScattering"] = &mge::lua::WeatherConfig::setScattering;
		lua_mge["setZoom"] = &mge::lua::LegacyInterface::setZoom;
		lua_mge["toggleZoom"] = &mge::lua::LegacyInterface::toggleZoom;
		lua_mge["zoomIn"] = &mge::lua::LegacyInterface::zoomIn;
		lua_mge["zoomOut"] = &mge::lua::LegacyInterface::zoomOut;
	}
}