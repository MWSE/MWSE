#include "NICamera.h"

#include "TES3WorldController.h"

#include "LuaUtil.h"

namespace NI {
	const auto NI_Camera_click = reinterpret_cast<void (__thiscall*)(Camera*, bool)>(0x6CC7B0);
	void Camera::click(bool something) {
		NI_Camera_click(this, something);
	}

	void Camera::click_lua(sol::optional<bool> something) {
		click(something.value_or(false));
	}

	const auto NI_Camera_windowPointToRay = reinterpret_cast<bool(__thiscall*)(Camera*, int, int, TES3::Vector3&, TES3::Vector3&)>(0x6CDA70);
	bool Camera::windowPointToRay(int screenX, int screenY, TES3::Vector3& out_origin, TES3::Vector3& out_direction) {
		return NI_Camera_windowPointToRay(this, screenX, screenY, out_origin, out_direction);
	}

	const auto NI_Camera_worldPointToScreenPoint = reinterpret_cast<bool(__thiscall*)(Camera*, const TES3::Vector3*, float&, float&)>(0x6CD450);
	bool Camera::worldPointToScreenPoint(const TES3::Vector3* point, float& out_screenX, float& out_screenY) {
		return NI_Camera_worldPointToScreenPoint(this, point, out_screenX, out_screenY);
	}

	sol::optional<std::tuple<TES3::Vector3, TES3::Vector3>> Camera::windowPointToRay_lua(sol::stack_object luaPoint) {
		TES3::Vector2 point;
		if (!mwse::lua::setVectorFromLua(point, luaPoint)) {
			throw std::runtime_error("Could not convert parameter to tes3vector2.");
		}

		std::tuple<TES3::Vector3, TES3::Vector3> result = {};
		auto worldController = TES3::WorldController::get();
		if (windowPointToRay(int(point.x + worldController->viewWidth/2), int(worldController->viewHeight/2 - point.y), std::get<0>(result), std::get<1>(result))) {
			return result;
		}
		return {};
	}

	sol::optional<TES3::Vector2> Camera::worldPointToScreenPoint_lua(sol::stack_object luaPosition) {
		TES3::Vector3 position;
		if (!mwse::lua::setVectorFromLua(position, luaPosition)) {
			throw std::runtime_error("Could not convert parameter to tes3vector2.");
		}

		float x, y;
		if (worldPointToScreenPoint(&position, x, y)) {
			auto worldController = TES3::WorldController::get();
			return TES3::Vector2((x - 0.5f) * worldController->viewWidth, (y - 0.5f) * worldController->viewHeight);
		}
		return {};
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Camera)
