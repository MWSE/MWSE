#include "MGEApiLua.h"

#include "TES3Game.h"

#include "LuaUtil.h"

namespace mge::lua {

	//
	// CoreInterface
	//

	bool CoreInterface::enabled() {
		return api != nullptr;
	}

	sol::object CoreInterface::getVersion(sol::this_state ts) {
		if (api == nullptr) {
			return sol::nil;
		}

		// Convert packed version to semver table. MGE XE major version is offset for legacy reasons.
		int ver = api->getMGEVersion() - 0x40000;
		int major = (ver >> 16) & 0xFF, minor = (ver >> 8) & 0xFF, patch = ver & 0xFF;

		sol::state_view state = ts;
		return state.create_table_with("major", major, "minor", minor, "patch", patch);
	}

	void CoreInterface::loadConfig() {
		if (!api->loadConfig()) {
			throw std::runtime_error("Could not load configuration.");
		}
	}

	void CoreInterface::saveConfig() {
		if (!api->saveConfig()) {
			throw std::runtime_error("Could not save configuration.");
		}
	}
	
	bool CoreInterface::reloadDistantLand() {
		return api->reloadDistantLand();
	}

	float CoreInterface::getGUIScale() {
		return api->guiGetScale();
	}

	void CoreInterface::setGUIScale(float value) {
		api->guiSetScale(value);
	}

	LightingMode CoreInterface::getLightingMode() {
		return api->lightingModeGet();
	}

	void CoreInterface::setLightingMode(LightingMode mode) {
		api->lightingModeSet(mode);
	}

	void CoreInterface::saveScreenshot(sol::optional<sol::table> params) {
		const char* path = mwse::lua::getOptionalParam<const char*>(params, "path", nullptr);
		bool captureWithUI = mwse::lua::getOptionalParam<bool>(params, "captureWithUI", false);

		if (!path) {
			throw std::invalid_argument("path argument missing.");
		}

		static_cast<MGEAPIv2*>(api)->saveScreenshot(path, captureWithUI);
	}

	//
	// CameraConfig
	//

	float CameraConfig::getFoV() {
		return api->cameraGetFoV();
	}

	void CameraConfig::setFoV(float fov) {
		api->cameraSetFoV(fov);
	}

	TES3::Vector3 CameraConfig::getThirdPersonOffset() {
		TES3::Vector3 offset;
		api->cameraThirdPersonGetOffset(&offset.x);
		return offset;
	}

	void CameraConfig::setThirdPersonOffset(sol::stack_object param) {
		TES3::Vector3 offset;
		if (!mwse::lua::setVectorFromLua(offset, param)) {
			throw std::invalid_argument("Provided argument is not convertable to a vector3.");
		}

		api->cameraThirdPersonSetOffset(&offset.x);
	}

	bool CameraConfig::getZoomEnabled() {
		return api->zoomGetEnabled();
	}

	void CameraConfig::setZoomEnabled(bool value) {
		api->zoomSetEnabled(value);
	}

	float CameraConfig::getZoom() {
		return api->zoomGetZoom();
	}

	void CameraConfig::setZoom(float value) {
		api->zoomSetZoom(value);
	}

	void CameraConfig::zoomIn(sol::optional<sol::table> params) {
		auto amount = mwse::lua::getOptionalParam<float>(params, "amount", 0.0625f);
		api->zoomIn(amount);
	}

	void CameraConfig::zoomOut(sol::optional<sol::table> params) {
		auto amount = mwse::lua::getOptionalParam<float>(params, "amount", 0.0625f);
		api->zoomOut(amount);
	}

	void CameraConfig::setZoomContinuous(sol::optional<sol::table> params) {
		auto rate = mwse::lua::getOptionalParam<float>(params, "rate", 1.0f);
		auto targetRate = mwse::lua::getOptionalParam<float>(params, "targetRate", rate);
		api->zoomSetZoomContinuous(rate, targetRate);
	}

	void CameraConfig::stopZoom() {
		api->zoomStop();
	}

	bool CameraConfig::getCameraShakeEnabled() {
		return api->cameraShakeGetEnabled();
	}

	void CameraConfig::setCameraShakeEnabled(bool value) {
		api->cameraShakeSetEnabled(value);
	}

	float CameraConfig::getCameraShakeMagnitude() {
		return api->cameraShakeGetMagnitude();
	}

	void CameraConfig::setCameraShakeMagnitude(float value) {
		api->cameraShakeSetMagnitude(value);
	}

	float CameraConfig::getCameraShakeAcceleration() {
		return api->cameraShakeGetAcceleration();
	}

	void CameraConfig::setCameraShakeAcceleration(float value) {
		api->cameraShakeSetAcceleration(value);
	}

	float CameraConfig::getNearRenderDistance() {
		return static_cast<MGEAPIv3*>(api)->nearRenderDistanceGet();
	}

	void CameraConfig::setNearRenderDistance(float distance) {
		static_cast<MGEAPIv3*>(api)->nearRenderDistanceSet(distance);
	}

	//
	// ShadersConfig
	//

	sol::table ShadersConfig::getShaders(sol::this_state ts) {
		sol::state_view state(ts);
		sol::table shaders = state.create_table();

		for (size_t i = 0; true; ++i) {
			auto s = api->shaderListShaders(i);
			if (s) {
				shaders[i + 1] = ShaderHandleLua(s);
			}
			else {
				break;
			}
		}

		return shaders;
	}

	std::string ShadersConfig::debugShaders() {
		std::stringstream out;

		for (size_t i = 0; true; i++) {
			auto s = api->shaderListShaders(i);
			if (s) {
				char line[256];
				std::snprintf(line, sizeof(line), "%p %s\n", s, api->shaderGetName(s));
				out << line;
			}
			else {
				break;
			}
		}

		return std::move(out.str());
	}

	sol::optional<ShaderHandleLua> ShadersConfig::findShader(sol::optional<sol::table> params) {
		auto id = mwse::lua::getOptionalParam<const char*>(params, "name", nullptr);
		if (id == nullptr) {
			throw std::invalid_argument("name argument missing.");
		}

		auto handle = api->shaderGetShader(id);
		if (handle) {
			return { handle };
		}

		return {};
	}

	sol::optional<ShaderHandleLua> ShadersConfig::loadShader(sol::optional<sol::table> params) {
		auto id = mwse::lua::getOptionalParam<const char*>(params, "name", nullptr);
		if (id == nullptr) {
			throw std::invalid_argument("name argument missing.");
		}

		auto handle = api->shaderLoad(id);
		if (handle) {
			return { handle };
		}

		return {};
	}

	float ShadersConfig::getHDRReactionSpeed() {
		return api->shaderGetHDRReactionSpeed();
	}

	void ShadersConfig::setHDRReactionSpeed(float speed) {
		api->shaderSetHDRReactionSpeed(speed);
	}

	//
	// WeatherConfig
	//
	sol::table WeatherConfig::getScattering(sol::this_state ts) {
		float inscatter[3], outscatter[3];
		api->weatherScatteringGet(inscatter, outscatter);

		sol::state_view state = ts;
		sol::table in = state.create_table_with(1, inscatter[0], 2, inscatter[1], 3, inscatter[2]);
		sol::table out = state.create_table_with(1, outscatter[0], 2, outscatter[1], 3, outscatter[2]);
		sol::table scattering = state.create_table_with("inscatter", in, "outscatter", out);
		return scattering;
	}

	void WeatherConfig::setScattering(sol::optional<sol::table> params) {
		auto inscatter = mwse::lua::getOptionalParamVector3(params, "inscatter");
		auto outscatter = mwse::lua::getOptionalParamVector3(params, "outscatter");

		if (inscatter && outscatter) {
			api->weatherScatteringSet(&inscatter.value().x, &outscatter.value().x);
		}
		else {
			throw std::invalid_argument("inscatter and outscatter must be 3-vectors.");
		}
	}

	sol::table WeatherConfig::getSkylightScattering(sol::this_state ts) {
		float skylight_scatter[4];
		static_cast<MGEAPIv2*>(api)->weatherScatteringSkylightGet(skylight_scatter);

		sol::state_view state = ts;
		sol::table far = state.create_table_with(1, skylight_scatter[0], 2, skylight_scatter[1], 3, skylight_scatter[2]);
		sol::table scattering = state.create_table_with("skylight", far, "mix", skylight_scatter[3]);
		return scattering;
	}

	void WeatherConfig::setSkylightScattering(sol::optional<sol::table> params) {
		auto skylight_scatter = mwse::lua::getOptionalParamVector3(params, "skylight");
		auto skylight_mix = mwse::lua::getOptionalParam<float>(params, "mix", 0.44f);

		if (skylight_scatter) {
			TES3::Vector4 v(skylight_scatter.value().x, skylight_scatter.value().y, skylight_scatter.value().z, skylight_mix);
			static_cast<MGEAPIv2*>(api)->weatherScatteringSkylightSet(&v.x);
		}
		else {
			throw std::invalid_argument("skylight must be a 3-vector.");
		}
	}

	sol::table WeatherConfig::getDLFog(int weatherID, sol::this_state ts) {
		float fogDistMult, fogOffset;
		api->weatherDistantFogGet(weatherID, &fogDistMult, &fogOffset);

		sol::state_view state = ts;
		return state.create_table_with("weather", weatherID, "distance", fogDistMult, "offset", fogOffset);
	}

	void WeatherConfig::setDLFog(sol::optional<sol::table> params) {
		auto weatherID = mwse::lua::getOptionalParam<int>(params, "weather", -1);
		auto fogDistMult = mwse::lua::getOptionalParam<float>(params, "distance", -1.0f);
		auto fogOffset = mwse::lua::getOptionalParam<float>(params, "offset", -1.0f);

		if (weatherID < 0) {
			throw std::invalid_argument("weather parameter required.");
		}
		if (fogDistMult < 0) {
			throw std::invalid_argument("distance parameter required.");
		}
		if (fogOffset < 0) {
			throw std::invalid_argument("offset parameter required.");
		}

		api->weatherDistantFogSet(weatherID, fogDistMult, fogOffset);
	}

	sol::table WeatherConfig::getPPLLight(int weatherID, sol::this_state ts) {
		float sunMult, ambMult;
		api->weatherPerPixelLightGet(weatherID, &sunMult, &ambMult);

		sol::state_view state = ts;
		return state.create_table_with("weather", weatherID, "sun", sunMult, "ambient", ambMult);
	}

	void WeatherConfig::setPPLLight(sol::optional<sol::table> params) {
		auto weatherID = mwse::lua::getOptionalParam<int>(params, "weather", -1);
		auto sunMult = mwse::lua::getOptionalParam<float>(params, "sun", -1.0f);
		auto ambMult = mwse::lua::getOptionalParam<float>(params, "ambient", -1.0f);

		if (weatherID < 0) {
			throw std::invalid_argument("weather parameter required.");
		}
		if (sunMult < 0) {
			throw std::invalid_argument("sun parameter required.");
		}
		if (ambMult < 0) {
			throw std::invalid_argument("ambient parameter required.");
		}

		api->weatherPerPixelLightSet(weatherID, sunMult, ambMult);
	}

	sol::table WeatherConfig::getWind(int weatherID, sol::this_state ts) {
		if (!(weatherID >= 0 && weatherID < 10)) {
			throw std::invalid_argument("valid weather parameter required.");
		}

		float speed = api->getDistantLandRenderConfig()->Wind[weatherID];
		sol::state_view state = ts;
		return state.create_table_with("weather", weatherID, "speed", speed);
	}

	void WeatherConfig::setWind(sol::optional<sol::table> params) {
		auto weatherID = mwse::lua::getOptionalParam<int>(params, "weather", -1);
		auto speed = mwse::lua::getOptionalParam<float>(params, "speed", -1.0f);

		if (!(weatherID >= 0 && weatherID < 10)) {
			throw std::invalid_argument("valid weather parameter required.");
		}
		if (speed < 0) {
			throw std::invalid_argument("speed parameter required.");
		}

		api->getDistantLandRenderConfig()->Wind[weatherID] = speed;
	}

	//
	// LegacyInterface
	//

	int LegacyInterface::getScreenHeight() {
		return TES3::Game::get()->windowHeight;
	}

	int LegacyInterface::getScreenWidth() {
		return TES3::Game::get()->windowWidth;
	}

	void LegacyInterface::enableZoom() {
		api->zoomSetEnabled(true);
	}

	void LegacyInterface::disableZoom() {
		api->zoomSetEnabled(false);
	}

	void LegacyInterface::toggleZoom() {
		api->zoomSetEnabled(!api->zoomGetEnabled());
	}

	void LegacyInterface::zoomIn(sol::optional<sol::table> params) {
		auto amount = mwse::lua::getOptionalParam<float>(params, "amount");
		if (amount) {
			api->zoomIn(amount.value());
		}
	}

	void LegacyInterface::zoomOut(sol::optional<sol::table> params) {
		auto amount = mwse::lua::getOptionalParam<float>(params, "amount");
		if (amount) {
			api->zoomOut(amount.value());
		}
	}

	void LegacyInterface::setZoom(sol::optional<sol::table> params) {
		auto amount = mwse::lua::getOptionalParam<float>(params, "amount");
		if (amount) {
			api->zoomSetZoom(amount.value());
		}
	}

	ShaderHandle LegacyInterface::findShader(sol::optional<sol::table> params) {
		auto shaderName = mwse::lua::getOptionalParam<const char*>(params, "shader", nullptr);
		if (shaderName == nullptr) {
			return nullptr;
		}

		return api->shaderGetShader(shaderName);
	}

	void LegacyInterface::enableShader(sol::optional<sol::table> params) {
		auto shader = findShader(params);
		if (shader) {
			api->shaderSetEnabled(shader, true);
		}
	}

	void LegacyInterface::disableShader(sol::optional<sol::table> params) {
		auto shader = findShader(params);
		if (shader) {
			api->shaderSetEnabled(shader, false);
		}
	}

	void LegacyInterface::setShaderFloat(sol::optional<sol::table> params) {
		auto shader = findShader(params);
		auto variable = mwse::lua::getOptionalParam<const char*>(params, "variable", nullptr);
		auto value = mwse::lua::getOptionalParam<float>(params, "value", 0.0f);

		if (shader && variable) {
			api->shaderSetFloat(shader, variable, value);
		}
	}

	void LegacyInterface::setShaderLong(sol::optional<sol::table> params) {
		auto shader = findShader(params);
		auto variable = mwse::lua::getOptionalParam<const char*>(params, "variable", nullptr);
		auto value = mwse::lua::getOptionalParam<int>(params, "value", 0);

		if (shader && variable) {
			api->shaderSetInt(shader, variable, value);
		}
	}

	std::tuple<float, float> LegacyInterface::getWeatherDLFog(int weatherID) {
		float fogDistMult, fogOffset;
		api->weatherDistantFogGet(weatherID, &fogDistMult, &fogOffset);
		return std::make_tuple(fogDistMult, fogOffset);
	}

	void LegacyInterface::setWeatherDLFog(int weatherID, float fogDistMult, float fogOffset) {
		api->weatherDistantFogSet(weatherID, fogDistMult, fogOffset);
	}

	std::tuple<float, float> LegacyInterface::getWeatherPPLLight(int weatherID) {
		float sunMult, ambMult;
		api->weatherPerPixelLightGet(weatherID, &sunMult, &ambMult);
		return std::make_tuple(sunMult, ambMult);
	}

	void LegacyInterface::setWeatherPPLLight(int weatherID, float sunMult, float ambMult) {
		api->weatherPerPixelLightSet(weatherID, sunMult, ambMult);
	}
}
