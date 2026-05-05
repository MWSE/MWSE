#pragma once

#include "NINode.h"
#include "NIRenderer.h"
#include "NIVector4.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "TES3Vectors.h"
#endif

namespace NI {
	struct Frustum {
		float left; // 0x0
		float right; // 0x4
		float top; // 0x8
		float bottom; // 0xC
		float near; // 0x10
		float far; // 0x14

		bool setFOV(float fovDegrees, float aspect);
	};
	static_assert(sizeof(Frustum) == 0x18, "NI::Frustum failed size validation");

	struct Camera : AVObject {
		// worldToCamera is logically a 4x4 matrix of floats (16 floats / 0x40 bytes).
		// In MWSE context expose it as TES3::Matrix44 so MWSE-private callers and
		// sol bindings see the structured type. CSSE keeps the raw float array.
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		TES3::Matrix44 worldToCamera; // 0x90
#else
		float worldToCamera[4][4]; // 0x90
#endif
		float viewDistance; // 0xD0
		float twoDivRmL; // 0xD4
		float twoDivTmB; // 0xD8
		Vector3 worldDirection; // 0xDC
		Vector3 worldUp; // 0xE8
		Vector3 worldRight; // 0xF4
		Frustum viewFrustum; // 0x100
		Vector4 port; // 0x118
		Pointer<Node> scene; // 0x128
		TArray<ScreenPolygon*> screenPolygons; // 0x12C
		Pointer<Renderer> renderer; // 0x144
		TArray<void*> cullingPlanePtrs; // 0x148
		int countCullingPlanes; // 0x160
		Vector4 cullingPlanes[6]; // 0x164
		uint32_t usedCullingPlanesBitfield[4]; // 0x1C4
		void* pointer_0x1D4;
		int field_0x1D8;
		float LODAdjust; // 0x1DC

		Camera();
		~Camera();

		//
		// Other related this-call functions.
		//

		void clear(Renderer::ClearFlags flags = Renderer::ClearFlags::ALL);

		void click(bool something = false);

		// Engine-dispatch helpers (per-target macros).
		void swapBuffers();
		bool LookAtWorldPoint(const Vector3* worldPoint, const Vector3* worldUp);

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		void clear_lua(sol::optional<int> flags);
		void click_lua(sol::optional<bool> something = false);

		std::reference_wrapper<Vector4[6]> getCullingPlanes_lua();
#endif

		// Note: screen coordinates are real from the viewport, and not
		bool windowPointToRay(int screenX, int screenY, Vector3& out_origin, Vector3& out_direction);
		bool worldPointToScreenPoint(const Vector3* point, float& out_screenX, float& out_screenY);

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		// Unlike above, we need to convert the ouput from [width/-2, width/2] to [0, width] and flip the height.
		sol::optional<std::tuple<Vector3, Vector3>> windowPointToRay_lua(sol::stack_object);

		// Unlike above, we need to convert the ouput from [0,1] to [width/-2, width/2].
		sol::optional<TES3::Vector2> worldPointToScreenPoint_lua(sol::stack_object);
#endif
	};
	static_assert(sizeof(Camera) == 0x1E0, "NI::Camera failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Camera)
#endif
