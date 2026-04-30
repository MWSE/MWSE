#include "NICamera.h"

#include "TES3WorldController.h"

#include "LuaUtil.h"

namespace NI {
	const auto NI_Camera_ctor = reinterpret_cast<void(__thiscall*)(Camera*)>(0x6CC200);
	Camera::Camera() {
		NI_Camera_ctor(this);
	}

	const auto NI_Camera_dtor = reinterpret_cast<void(__thiscall*)(Camera*)>(0x6CC510);
	Camera::~Camera() {
		NI_Camera_dtor(this);
	}

	const auto NI_Camera_clear = reinterpret_cast<void(__thiscall*)(Camera*, Renderer::ClearFlags)>(0x6CC780);
	void Camera::clear(Renderer::ClearFlags flags) {
		NI_Camera_clear(this, flags);
	}

	void Camera::clear_lua(sol::optional<int> flags) {
		clear(Renderer::ClearFlags(flags.value_or(Renderer::ClearFlags::ALL)));
	}

	const auto NI_Camera_click = reinterpret_cast<void (__thiscall*)(Camera*, bool)>(0x6CC7B0);
	void Camera::click(bool something) {
		NI_Camera_click(this, something);
	}

	const auto NI_Camera_swapBuffers = reinterpret_cast<void(__thiscall*)(Camera*)>(0x6CC930);
	void Camera::swapBuffers() {
		NI_Camera_swapBuffers(this);
	}

	void Camera::click_lua(sol::optional<bool> something) {
		click(something.value_or(false));
	}

	std::reference_wrapper<TES3::Vector4[6]> Camera::getCullingPlanes_lua() {
		return std::ref(cullingPlanes);
	}

	const auto NI_Camera_windowPointToRay = reinterpret_cast<bool(__thiscall*)(Camera*, int, int, TES3::Vector3&, TES3::Vector3&)>(0x6CDA70);
	bool Camera::windowPointToRay(int screenX, int screenY, TES3::Vector3& out_origin, TES3::Vector3& out_direction) {
		return NI_Camera_windowPointToRay(this, screenX, screenY, out_origin, out_direction);
	}

	const auto NI_Camera_worldPointToScreenPoint = reinterpret_cast<bool(__thiscall*)(Camera*, const TES3::Vector3*, float&, float&)>(0x6CD450);
	bool Camera::worldPointToScreenPoint(const TES3::Vector3* point, float& out_screenX, float& out_screenY) {
		return NI_Camera_worldPointToScreenPoint(this, point, out_screenX, out_screenY);
	}

	const auto NI_Camera_lookAtWorldPoint = reinterpret_cast<bool(__thiscall*)(Camera*, const TES3::Vector3*, const TES3::Vector3*)>(0x6CD5D0);
	bool Camera::LookAtWorldPoint(const TES3::Vector3* worldPoint, const TES3::Vector3* worldUp) {
		return NI_Camera_lookAtWorldPoint(this, worldPoint, worldUp);
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
