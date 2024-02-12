#pragma once

#include "MGEApi.h"
#include "MGEPostShaders.h"

#include "TES3Vectors.h"

namespace mge::lua {
	class CoreInterface {
	public:
		static bool enabled();
		static sol::object getVersion(sol::this_state ts);
		static void loadConfig();
		static void saveConfig();

		static bool reloadDistantLand();

		static float getGUIScale();
		static void setGUIScale(float value);

		static LightingMode getLightingMode();
		static void setLightingMode(LightingMode mode);

		// API v2+ for following functions
		static void saveScreenshot(sol::optional<sol::table> params);
	};

	class CameraConfig {
	public:
		static float getFoV();
		static void setFoV(float fov);
		static TES3::Vector3 getThirdPersonOffset();
		static void setThirdPersonOffset(sol::stack_object param);
		static bool getZoomEnabled();
		static void setZoomEnabled(bool value);
		static float getZoom();
		static void setZoom(float value);
		static void zoomIn(sol::optional<sol::table> params);
		static void zoomOut(sol::optional<sol::table> params);
		static void setZoomContinuous(sol::optional<sol::table> params);
		static void stopZoom();
		static bool getCameraShakeEnabled();
		static void setCameraShakeEnabled(bool value);
		static float getCameraShakeMagnitude();
		static void setCameraShakeMagnitude(float value);
		static float getCameraShakeAcceleration();
		static void setCameraShakeAcceleration(float value);

		// API v3+ for following functions
		static float getNearRenderDistance();
		static void setNearRenderDistance(float distance);
	};

	class RenderFeatures {
	public:
		template <RenderFeature F>
		static bool getFeatureEnabled() {
			return api->featureGetEnabled(F);
		}

		template <RenderFeature F>
		static void setFeatureEnabled(bool enabled) {
			api->featureSetEnabled(F, enabled);
		}

		template <RenderFeature F>
		static inline void bindFeature(sol::usertype<RenderFeatures>& userdata, const char* name) {
			userdata[name] = sol::property(&getFeatureEnabled<F>, &setFeatureEnabled<F>);
		}
	};

	class ShadersConfig {
	public:
		static sol::table getShaders(sol::this_state ts);
		static std::string debugShaders();
		static sol::optional<ShaderHandleLua> findShader(sol::optional<sol::table> params);
		static sol::optional<ShaderHandleLua> loadShader(sol::optional<sol::table> params);

		static float getHDRReactionSpeed();
		static void setHDRReactionSpeed(float speed);
	};

	class WeatherConfig {
	public:
		static sol::table getScattering(sol::this_state ts);
		static void setScattering(sol::optional<sol::table> params);
		static sol::table getSkylightScattering(sol::this_state ts);
		static void setSkylightScattering(sol::optional<sol::table> params);
		static sol::table getDLFog(int weatherID, sol::this_state ts);
		static void setDLFog(sol::optional<sol::table> params);
		static sol::table getPPLLight(int weatherID, sol::this_state ts);
		static void setPPLLight(sol::optional<sol::table> params);
		static sol::table getWind(int weatherID, sol::this_state ts);
		static void setWind(sol::optional<sol::table> params);
	};

	class LegacyInterface {
	public:
		static int getScreenHeight();
		static int getScreenWidth();

		static void enableZoom();
		static void disableZoom();
		static void toggleZoom();
		static void zoomIn(sol::optional<sol::table> params);
		static void zoomOut(sol::optional<sol::table> params);
		static void setZoom(sol::optional<sol::table> params);

		static ShaderHandle findShader(sol::optional<sol::table> params);
		static void enableShader(sol::optional<sol::table> params);
		static void disableShader(sol::optional<sol::table> params);
		static void setShaderFloat(sol::optional<sol::table> params);
		static void setShaderLong(sol::optional<sol::table> params);

		static std::tuple<float, float> getWeatherDLFog(int weatherID);
		static void setWeatherDLFog(int weatherID, float fogDistMult, float fogOffset);
		static std::tuple<float, float> getWeatherPPLLight(int weatherID);
		static void setWeatherPPLLight(int weatherID, float sunMult, float ambMult);
	};
}
