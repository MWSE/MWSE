#include "TES3WaterController.h"

#include "LuaUtil.h"

namespace TES3 {
	const auto TES3_WaterController_createRipple = reinterpret_cast<void(__thiscall*)(WaterController*, float, float, float, float, bool)>(0x51C1E0);
	void WaterController::createRipple(float x, float y, float scale, float speed, bool active) {
		TES3_WaterController_createRipple(this, x, y, scale, speed, active);
	}

	const auto TES3_WaterController_setRainFrequency = reinterpret_cast<void(__thiscall*)(WaterController*, float)>(0x51DAB0);
	void WaterController::setRainFrequency(float frequency) {
		TES3_WaterController_setRainFrequency(this, frequency);
	}

	void WaterController::createRipple_lua(sol::optional<sol::table> params) {
		using namespace mwse::lua;

		const auto x = getOptionalParam<float>(params, "x");
		const auto y = getOptionalParam<float>(params, "y");
		if (!x || !y) {
			throw std::exception("An x and y position must be defined.");
		}

		const auto scale = getOptionalParam(params, "scale", 1.0f);
		const auto speed = getOptionalParam(params, "speed", 1.0f);
		const auto active = getOptionalParam(params, "active", true);

		createRipple(x.value(), y.value(), scale, speed, active);
	}

	std::reference_wrapper<float[3]> WaterController::getRippleAlphas() {
		return rippleAlphas;
	}

	nonstd::span<WaterController::Ripple> WaterController::getRipples() {
		if (ripples == nullptr) {
			return {};
		}
		return { ripples, maxRippleCount };
	}
}
