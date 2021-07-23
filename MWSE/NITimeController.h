#pragma once

#include "NIDefines.h"

#include "NIObject.h"

namespace NI {
	struct TimeController_vTable : Object_vTable {
		void(__thiscall* start)(TimeController*, float); // 0x2C
		void(__thiscall* stop)(TimeController*); // 0x30
		void(__thiscall* update)(TimeController*, float); // 0x34
		void(__thiscall* setTarget)(TimeController*, ObjectNET*); // 0x38
		float(__thiscall* computeScaledTime)(TimeController*, float); // 0x3C
		void* unknown_0x40;
		bool(__thiscall* targetIsRequiredType)(TimeController*); // 0x44

		TimeController_vTable();
	};
	static_assert(sizeof(TimeController_vTable) == 0x48, "NI::TimeController's vtable failed size validation");

	struct TimeController : Object {
		unsigned short deprecatedFlags; // 0x8
		float frequency; // 0xC
		float phase; // 0x10
		float lowKeyFrame; // 0x14
		float highKeyFrame; // 0x18
		float startTime; // 0x1C
		float lastTime; // 0x20
		float unknown_0x24;
		ObjectNET * target; // 0x28
		Pointer<TimeController> nextController; // 0x2C
		bool unknown_0x30; // Force update? Compute scaled time?

		//
		//
		//

		void ctor();
		void dtor();

		//
		//
		//

		void start(float time);
		void stop();
		void update(float deltaTime);
		void setTarget(ObjectNET*);
		float computeScaledTime(float dt);
		bool targetIsRequiredType();

		//
		// Access to this type's raw functions.
		//

		static constexpr auto _ctor = reinterpret_cast<void(__thiscall*)(TimeController*)>(0x6FC540);
		static constexpr auto _dtor = reinterpret_cast<void(__thiscall*)(TimeController*)>(0x6FC610);

		static constexpr auto _copy = reinterpret_cast<void(__thiscall*)(const TimeController*, TimeController*)>(0x6FC8E0);

		static constexpr auto _loadBinary = reinterpret_cast<void(__thiscall*)(TimeController*, Stream*)>(0x6FC9F0);
		static constexpr auto _registerStreamables = reinterpret_cast<bool(__thiscall*)(TimeController*, Stream*)>(0x6FCAD0);
		static constexpr auto _saveBinary = reinterpret_cast<void(__thiscall*)(TimeController*, Stream*)>(0x6FCB00);
		static constexpr auto _isEqual = reinterpret_cast<bool(__thiscall*)(TimeController*, TimeController*)>(0x6FCBA0);
		static constexpr auto _addViewerStrings = reinterpret_cast<void(__thiscall*)(TimeController*, TArray<char*>*)>(0x6FCC30);

		static constexpr auto _start = reinterpret_cast<void(__thiscall*)(TimeController*, float)>(0x6FC6B0);
		static constexpr auto _stop = reinterpret_cast<void(__thiscall*)(TimeController*)>(0x6FC6E0);
		static constexpr auto _setTarget = reinterpret_cast<void(__thiscall*)(TimeController*, ObjectNET*)>(0x6FC870);
		static constexpr auto _computeScaledTime = reinterpret_cast<float(__thiscall*)(TimeController*, float)>(0x6FC710);

	};
	static_assert(sizeof(TimeController) == 0x34, "NI::TimeController failed size validation");
}
