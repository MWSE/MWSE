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

		//
		// Construction/destruction.
		//

		void ctor();
		void dtor();

		//
		// Related this-call functions.
		//

		void start(float time);
		void stop();
		void update(float deltaTime);
		void setTarget(ObjectNET*);
		float computeScaledTime(float dt);
		bool targetIsRequiredType() const;
		bool getActive() const;
		void setActive(bool active);
		unsigned int getAnimTimingType() const;
		void setAnimTimingType(unsigned int type);
		unsigned int getCycleType() const;
		void setCycleType(unsigned int type);

		// Engine function pointer used by FlipController and others. Per-target
		// address resolved via NIConfig.{Morrowind,TESConstructionSet}.h. The
		// CSSE address is unknown today (macro = 0x0), so the constexpr is gated
		// behind `> 0` and call sites must guard for the not-defined case.
#if defined(SE_NI_TIMECONTROLLER_FNADDR_COPY) && SE_NI_TIMECONTROLLER_FNADDR_COPY > 0
		static constexpr auto _copy = reinterpret_cast<void(__thiscall*)(const TimeController*, TimeController*)>(SE_NI_TIMECONTROLLER_FNADDR_COPY);
#endif

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		//
		// Access to engine raw functions. Addresses are Morrowind.exe-specific.
		// (consider migrating to per-target macros via NIConfig files in the
		// guard-cleanup pass).
		//

		static constexpr auto _ctor = reinterpret_cast<void(__thiscall*)(TimeController*)>(0x6FC540);
		static constexpr auto _dtor = reinterpret_cast<void(__thiscall*)(TimeController*)>(0x6FC610);

		static constexpr auto _loadBinary = reinterpret_cast<void(__thiscall*)(TimeController*, Stream*)>(0x6FC9F0);
		static constexpr auto _registerStreamables = reinterpret_cast<bool(__thiscall*)(TimeController*, Stream*)>(0x6FCAD0);
		static constexpr auto _linkObject = reinterpret_cast<void(__thiscall*)(TimeController*, Stream*)>(0x6FCA70);
		static constexpr auto _saveBinary = reinterpret_cast<void(__thiscall*)(TimeController*, Stream*)>(0x6FCB00);
		static constexpr auto _isEqual = reinterpret_cast<bool(__thiscall*)(const TimeController*, const TimeController*)>(0x6FCBA0);
		static constexpr auto _addViewerStrings = reinterpret_cast<void(__thiscall*)(TimeController*, TArray<char*>*)>(0x6FCC30);

		static constexpr auto _start = reinterpret_cast<void(__thiscall*)(TimeController*, float)>(0x6FC6B0);
		static constexpr auto _stop = reinterpret_cast<void(__thiscall*)(TimeController*)>(0x6FC6E0);
		static constexpr auto _setTarget = reinterpret_cast<void(__thiscall*)(TimeController*, ObjectNET*)>(0x6FC870);
		static constexpr auto _computeScaledTime = reinterpret_cast<float(__thiscall*)(TimeController*, float)>(0x6FC710);
#endif
	};
	static_assert(sizeof(TimeController) == 0x34, "NI::TimeController failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::TimeController)
#endif
