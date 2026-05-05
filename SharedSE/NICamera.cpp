#include "NICamera.h"

#include "ExceptionUtil.h"
#include "MathUtil.h"

namespace NI {
	bool Frustum::setFOV(float fovDegrees, float aspect) {
		const auto fovRadians = se::math::degreesToRadians(fovDegrees);
		const auto fovTan = std::tan(fovRadians / 2.0f);
		const auto fovTanAspect = fovTan * aspect;

		left = -fovTan;
		right = fovTan;
		top = fovTanAspect;
		bottom = -fovTanAspect;

		return true;
	}

	Camera::Camera() {
#if defined(SE_NI_CAMERA_FNADDR_CTOR) && SE_NI_CAMERA_FNADDR_CTOR > 0
		// Cleanup automatic cruft.
		se::memory::_delete(screenPolygons.storage);
		se::memory::_delete(cullingPlanePtrs.storage);
		memset(this, 0, sizeof(Camera));

		const auto NI_Camera_ctor = reinterpret_cast<void(__thiscall*)(Camera*)>(SE_NI_CAMERA_FNADDR_CTOR);
		NI_Camera_ctor(this);
#else
		throw not_implemented_exception();
#endif
	}

	Camera::~Camera() {
#if defined(SE_NI_CAMERA_FNADDR_DTOR) && SE_NI_CAMERA_FNADDR_DTOR > 0
		const auto NI_Camera_dtor = reinterpret_cast<void(__thiscall*)(Camera*)>(SE_NI_CAMERA_FNADDR_DTOR);
		NI_Camera_dtor(this);
#else
		throw not_implemented_exception();
#endif
	}

	void Camera::clear(Renderer::ClearFlags flags) {
#if defined(SE_NI_CAMERA_FNADDR_CLEAR) && SE_NI_CAMERA_FNADDR_CLEAR > 0
		const auto NI_Camera_clear = reinterpret_cast<void(__thiscall*)(Camera*, Renderer::ClearFlags)>(SE_NI_CAMERA_FNADDR_CLEAR);
		NI_Camera_clear(this, flags);
#else
		throw not_implemented_exception();
#endif
	}

	void Camera::click(bool something) {
#if defined(SE_NI_CAMERA_FNADDR_CLICK) && SE_NI_CAMERA_FNADDR_CLICK > 0
		const auto NI_Camera_click = reinterpret_cast<void(__thiscall*)(Camera*, bool)>(SE_NI_CAMERA_FNADDR_CLICK);
		NI_Camera_click(this, something);
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void Camera::click_lua(sol::optional<bool> something) {
		click(something.value_or(false));
	}
#endif

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	std::reference_wrapper<Vector4[6]> Camera::getCullingPlanes_lua() {
		return std::ref(cullingPlanes);
	}
#endif

	void Camera::swapBuffers() {
#if defined(SE_NI_CAMERA_FNADDR_SWAPBUFFERS) && SE_NI_CAMERA_FNADDR_SWAPBUFFERS > 0
		const auto NI_Camera_swapBuffers = reinterpret_cast<void(__thiscall*)(Camera*)>(SE_NI_CAMERA_FNADDR_SWAPBUFFERS);
		NI_Camera_swapBuffers(this);
#else
		throw not_implemented_exception();
#endif
	}

	bool Camera::LookAtWorldPoint(const Vector3* worldPoint, const Vector3* worldUp) {
#if defined(SE_NI_CAMERA_FNADDR_LOOKATWORLDPOINT) && SE_NI_CAMERA_FNADDR_LOOKATWORLDPOINT > 0
		const auto NI_Camera_lookAtWorldPoint = reinterpret_cast<bool(__thiscall*)(Camera*, const Vector3*, const Vector3*)>(SE_NI_CAMERA_FNADDR_LOOKATWORLDPOINT);
		return NI_Camera_lookAtWorldPoint(this, worldPoint, worldUp);
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void Camera::clear_lua(sol::optional<int> flags) {
		clear(Renderer::ClearFlags(flags.value_or(Renderer::ClearFlags::ALL)));
	}
#endif

	bool Camera::windowPointToRay(int screenX, int screenY, Vector3& out_origin, Vector3& out_direction) {
#if defined(SE_NI_CAMERA_FNADDR_WINDOWPOINTTORAY) && SE_NI_CAMERA_FNADDR_WINDOWPOINTTORAY > 0
		const auto NI_Camera_windowPointToRay = reinterpret_cast<bool(__thiscall*)(Camera*, int, int, Vector3&, Vector3&)>(SE_NI_CAMERA_FNADDR_WINDOWPOINTTORAY);
		return NI_Camera_windowPointToRay(this, screenX, screenY, out_origin, out_direction);
#else
		throw not_implemented_exception();
#endif
	}

	bool Camera::worldPointToScreenPoint(const Vector3* point, float& out_screenX, float& out_screenY) {
#if defined(SE_NI_CAMERA_FNADDR_WORLDPOINTTOSCREENPOINT) && SE_NI_CAMERA_FNADDR_WORLDPOINTTOSCREENPOINT > 0
		const auto NI_Camera_worldPointToScreenPoint = reinterpret_cast<bool(__thiscall*)(Camera*, const Vector3*, float&, float&)>(SE_NI_CAMERA_FNADDR_WORLDPOINTTOSCREENPOINT);
		return NI_Camera_worldPointToScreenPoint(this, point, out_screenX, out_screenY);
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	sol::optional<std::tuple<Vector3, Vector3>> Camera::windowPointToRay_lua(sol::stack_object luaPoint) {
		TES3::Vector2 point;
		if (!mwse::lua::setVectorFromLua(point, luaPoint)) {
			throw std::runtime_error("Could not convert parameter to tes3vector2.");
		}

		std::tuple<Vector3, Vector3> result = {};
		auto worldController = TES3::WorldController::get();
		if (windowPointToRay(int(point.x + worldController->viewWidth / 2), int(worldController->viewHeight / 2 - point.y), std::get<0>(result), std::get<1>(result))) {
			return result;
		}
		return {};
	}

	sol::optional<TES3::Vector2> Camera::worldPointToScreenPoint_lua(sol::stack_object luaPosition) {
		Vector3 position;
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
#endif
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Camera)
#endif
