#pragma once

#include "NIObject.h"

namespace NI {
	struct TimeController_vTable : Object_vTable {
		void(__thiscall* start)(TimeController*, float); // 0x2C
		void(__thiscall* stop)(TimeController*); // 0x30
		void(__thiscall* update)(TimeController*, float); // 0x34
		void(__thiscall* setTarget)(TimeController*, ObjectNET*); // 0x38
		float(__thiscall* computeScaledTime)(TimeController*, float); // 0x3C
		void(__thiscall* onPreDisplay)(TimeController*); // 0x40
		bool(__thiscall* targetIsRequiredType)(const TimeController*); // 0x44

		TimeController_vTable();
	};
	static_assert(sizeof(TimeController_vTable) == 0x48, "NI::TimeController's vtable failed size validation");

	namespace TimeControllerFlags {
		typedef unsigned short value_type;

		enum TimeControllerFlags : value_type {
			AppTime = 0,
			AppInit = 1,
			AppTimingMask = 1,
			Loop = 0,
			Reverse = 2,
			Clamp = 4,
			CycleTypeMask = 6,
			Active = 8
		};
	}

	struct TimeController : Object {
		TimeControllerFlags::value_type flags; // 0x8
		float frequency; // 0xC
		float phase; // 0x10
		float lowKeyFrame; // 0x14
		float highKeyFrame; // 0x18
		float startTime; // 0x1C
		float lastTime; // 0x20
		float lastScaledTime; // 0x24
		ObjectNET* target; // 0x28
		Pointer<TimeController> nextController; // 0x2C
		bool updateRequired; // 0x30

		// Engine function pointers. Address is Morrowind.exe-specific; CSSE never calls these.
		static constexpr auto _copy = reinterpret_cast<void(__thiscall*)(const TimeController*, TimeController*)>(0x6FC8E0);
	};
	static_assert(sizeof(TimeController) == 0x34, "NI::TimeController failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::TimeController)
#endif
