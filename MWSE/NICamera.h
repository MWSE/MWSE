#pragma once

#include "NINode.h"
#include "NIRenderer.h"

namespace NI {
	struct Frustum {
		float left; // 0x0
		float right; // 0x4
		float top; // 0x8
		float bottom; // 0xC
		float near; // 0x10
		float far; // 0x14
	};
	static_assert(sizeof(Frustum) == 0x18, "NI::Frustum failed size validation");

	struct Camera : AVObject {
		TES3::Matrix44 worldToCamera; // 0x90
		float viewDistance; // 0xD0
		float twoDivRmL; // 0xD4
		float twoDivTmB; // 0xD8
		TES3::Vector3 worldDirection; // 0xDC
		TES3::Vector3 worldUp; // 0xE8
		TES3::Vector3 worldRight; // 0xF4
		Frustum viewFrustum; // 0x100
		TES3::Vector4 port; // 0x118
		Pointer<Node> scene; // 0x128
		NI::TArray<ScreenPolygon*> screenPolygons; // 0x12C
		Pointer<Renderer> renderer; // 0x144
		NI::TArray<void*> cullingPlanePtrs; // 0x148
		int countCullingPlanes; // 0x160
		TES3::Vector4 cullingPlanes[6]; // 0x164
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
		void clear_lua(sol::optional<int> flags);
		void swapBuffers();
		void click(bool something = false);
		void click_lua(sol::optional<bool> something = false);

		std::reference_wrapper<TES3::Vector4[6]> getCullingPlanes_lua();

		// Note: screen coordinates are real from the viewport, and not 
		bool windowPointToRay(int screenX, int screenY, TES3::Vector3& out_origin, TES3::Vector3& out_direction);
		bool worldPointToScreenPoint(const TES3::Vector3* point, float& out_screenX, float& out_screenY);
		bool LookAtWorldPoint(const TES3::Vector3* worldPoint, const TES3::Vector3* worldUp);

		// Unlike above, we need to convert the ouput from [width/-2, width/2] to [0, width] and flip the height.
		sol::optional<std::tuple<TES3::Vector3, TES3::Vector3>> windowPointToRay_lua(sol::stack_object);

		// Unlike above, we need to convert the ouput from [0,1] to [width/-2, width/2].
		sol::optional<TES3::Vector2> worldPointToScreenPoint_lua(sol::stack_object);
	};
	static_assert(sizeof(Camera) == 0x1E0, "NI::Camera failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Camera)
