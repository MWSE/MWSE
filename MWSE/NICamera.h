#pragma once

#include "NIAVObject.h"

namespace NI {
	struct Camera : AVObject {
		TES3::Matrix44 worldToCamera; // 0x90
		float viewDistance; // 0xD0
		float twoDivRmL; // 0xD4
		float twoDivTmB; // 0xD8
		TES3::Vector3 worldDirection; // 0xDC
		TES3::Vector3 worldUp; // 0xE8
		TES3::Vector3 worldRight; // 0xF4
		float viewFrustum[6]; // 0x100
		TES3::Vector4 port; // 0x118
		Object * scene; // 0x128
		NI::TArray<void*> unknown_0x12C; // Screen related?
		Object * renderer; // 0x144
		NI::TArray<void*> unknown_0x148; // Multiple cameras?
		int unknown_0x160;
		TES3::Vector4 cullingPlanes[6]; // 0x164
		float unknown_0x1C4;
		float unknown_0x1C8;
		float unknown_0x1CC;
		float unknown_0x1D0;
		void * unknown_0x1D4;
		int unknown_0x1D8;
		float LODAdjust; // 0x1DC

		//
		// Other related this-call functions.
		//

		void click(bool something = false);
		void click_lua(sol::optional<bool> something = false);

		// Note: screen coordinates are real from the viewport, and not 
		bool windowPointToRay(int screenX, int screenY, TES3::Vector3& out_origin, TES3::Vector3& out_direction);
		bool worldPointToScreenPoint(const TES3::Vector3* point, float& out_screenX, float& out_screenY);

		// Unlike above, we need to convert the ouput from [width/-2, width/2] to [0, width] and flip the height.
		sol::optional<std::tuple<TES3::Vector3, TES3::Vector3>> windowPointToRay_lua(sol::stack_object);

		// Unlike above, we need to convert the ouput from [0,1] to [width/-2, width/2].
		sol::optional<TES3::Vector2> worldPointToScreenPoint_lua(sol::stack_object);

	};
	static_assert(sizeof(Camera) == 0x1E0, "NI::Camera failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Camera)
