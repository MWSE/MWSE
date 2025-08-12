#pragma once

#include "TES3Defines.h"

#include "TES3Vectors.h"

// Must be added to header files that declare Ni types that can be derived.
#define MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3_WEATHER(T) \
int sol_lua_push(sol::types<T>, lua_State* L, const T& obj); \
int sol_lua_push(sol::types<T*>, lua_State* L, const T* obj);

// Must be added to source files that declare Ni types that can be derived.
#define MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3_WEATHER(T) \
int sol_lua_push(sol::types<T>, lua_State* L, const T& obj) { return obj.getOrCreateLuaObject(L).push(L); } \
int sol_lua_push(sol::types<T*>, lua_State* L, const T* obj) { return obj->getOrCreateLuaObject(L).push(L); }

namespace TES3 {
	struct Weather_vTable {
		void(__thiscall* deleting)(Weather*, bool); // 0x0
		void(__thiscall* simulate)(Weather*, float, float); // 0x4
		void(__thiscall* unload)(Weather*); // 0x8
		void(__thiscall* transition)(Weather*); // 0xC
	};
	static_assert(sizeof(Weather_vTable) == 0x10, "TES3::Weather's vtable failed size validation");

	struct Weather {
		Weather_vTable * vTable; // 0x0
		int index; // 0x4
		float transitionDelta; // 0x8
		float glareView; // 0xC
		WeatherController * controller; // 0x10
		Vector3 ambientSunriseCol; // 0x14
		Vector3 ambientDayCol; // 0x20
		Vector3 ambientSunsetCol; // 0x2C
		Vector3 ambientNightCol; // 0x38
		Vector3 fogSunriseCol; // 0x44
		Vector3 fogDayCol; // 0x50
		Vector3 fogSunsetCol; // 0x5C
		Vector3 fogNightCol; // 0x68
		Vector3 skySunriseCol; // 0x74
		Vector3 skyDayCol; // 0x80
		Vector3 skySunsetCol; // 0x8C
		Vector3 skyNightCol; // 0x98
		Vector3 sunSunriseCol; // 0xA4
		Vector3 sunDayCol; // 0xB0
		Vector3 sunSunsetCol; // 0xBC
		Vector3 sunNightCol; // 0xC8
		Vector3 sundiscSunsetCol; // 0xD4
		float unknown_0xE0;
		Vector3 unknown_0xE4;
		float cloudsMaxPercent; // 0xF0
		float landFogDayDepth; // 0xF4
		float landFogNightDepth; // 0xF8
		float cloudsSpeed; // 0xFC
		float windSpeed; // 0x100
		char texturePathCloud[260]; // 0x104
		int unknown_0x208;
		bool ambientPlaying;
		bool underwaterSoundState;
		char soundIDAmbientLoop[260]; // 0x20E
		Sound * soundAmbientLoop; // 0x314

		Weather() = delete;
		~Weather() = delete;

		//
		// Custom functions.
		//

		std::string toJson() const;

		const char* getName() const;

		const char* getCloudTexturePath() const;
		bool setCloudTexturePath(const char* path);
		const char* getAmbientLoopSoundID() const;
		bool setAmbientLoopSoundID(const char* id);

		// Storage for cached userdata.
		sol::object getOrCreateLuaObject(lua_State* L) const;
		static void clearCachedLuaObject(const Weather* object);
		static void clearCachedLuaObjects();
	};
	static_assert(sizeof(Weather) == 0x318, "TES3::Weather failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3_WEATHER(TES3::Weather)
