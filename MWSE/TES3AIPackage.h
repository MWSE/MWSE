#pragma once

#include "TES3Defines.h"
#include "TES3Vectors.h"

// Must be added to header files that declare Ni types that can be derived.
#define MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3_AIPACKAGE(T) \
int sol_lua_push(sol::types<T>, lua_State* L, const T& obj); \
int sol_lua_push(sol::types<T*>, lua_State* L, const T* obj);

// Must be added to source files that declare Ni types that can be derived.
#define MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3_AIPACKAGE(T) \
int sol_lua_push(sol::types<T>, lua_State* L, const T& obj) { return obj.getOrCreateLuaObject(L).push(L); } \
int sol_lua_push(sol::types<T*>, lua_State* L, const T* obj) { return obj->getOrCreateLuaObject(L).push(L); }

namespace TES3 {
	// These enums have different package ordering.
	// Of these two enums, prefer use of AIPackageType enum for script output.
	enum struct AIPackageType {
		Wander = 0,
		Travel = 1,
		Escort = 2,
		Follow = 3,
		Activate = 4,
	};

	enum struct AIPackageConfigType {
		Travel = 1,
		Wander = 2,
		Escort = 3,
		Follow = 4,
		Activate = 5,
	};

	struct AIPackageVirtualTable {
		AIPackage* (__thiscall * deleting)(AIPackage*, bool); // 0x0
		void (__thiscall * simulate)(AIPackage*); // 0x4
		bool (__thiscall * movement)(AIPackage*); // 0x8
		bool (__thiscall * initialize)(AIPackage*); // 0xC
		void (__thiscall * cleanup)(AIPackage*); // 0x10
	};
	static_assert(sizeof(AIPackageVirtualTable) == 0x14, "TES3::AIPackage_vTable failed size validation");

	struct AIPackage {
		AIPackageVirtualTable * vTable; // 0x0
		char unknown_0x4;
		bool moving; // 0x5
		char pad_0x6[2];
		float startGameHour; // 0x8
		MobileActor * targetActor; // 0xC
		AIPackageType packageType; // 0x10
		void * unknown_0x14; // Pointer to a vector?
		short duration; // 0x18
		char pad_0x19[2];
		int distance; // 0x1C
		int hourOfDay; // 0x20
		Vector3 targetPosition; // 0x24
		bool done; // 0x30
		bool started; // 0x31
		bool reset; // 0x32
		bool finalized; // 0x33
		MobileActor * owningActor; // 0x34
		Cell * destinationCell; // 0x38

		//
		// vTable accessor functions.
		//

		void simulate();
		bool movement();
		bool initialize();
		void cleanup();

		//
		// Other related this-call functions.
		//

		MobileActor* getTargetActor() const;
		void setTargetActor(TES3::MobileActor* target);
		void setTargetActorAsFriend(TES3::MobileActor* target);
		void setTargetActorAsFriendIfActive(TES3::MobileActor* target);

		//
		// Custom functions
		//

		// Allow dynamic recasting to derived types.
		sol::object getOrCreateLuaObject(lua_State* L) const;

	};
	static_assert(sizeof(AIPackage) == 0x3C, "TES3::AIPackage failed size validation");

	struct AIPackageConfig {
		AIPackageConfigType type; // 0x0

		// Helper function.
		AIPackageType toPackageType() const;
	};
	static_assert(sizeof(AIPackageConfig) == 0x4, "TES3::AIPackageConfig failed size validation");

	struct AIPackageTravel : AIPackage {
		struct Config : AIPackageConfig {
			Vector3 position; // 0x4
			bool reset; // 0x10
		};
		Vector3 destination; // 0x3C
		int unknown_0x48;
		Vector3 unknown_0x4C;
		void * pathingData; // 0x58
	};
	static_assert(sizeof(AIPackageTravel) == 0x5C, "TES3::AIPackageTravel failed size validation");
	static_assert(sizeof(AIPackageTravel::Config) == 0x14, "TES3::AIPackageTravel::Config failed size validation");

	struct AIPackageWander : AIPackage {
		struct Config : AIPackageConfig {
			unsigned short range; // 0x4
			unsigned char duration; // 0x6
			unsigned char time; // 0x7
			unsigned char idles[8]; // 0x08
			bool reset; // 0x10
		};
		struct IdleNode {
			unsigned char index; // 0x0
			char pad_1[3];
			int chance;  // 0x4
		};
		Vector3 unknown_0x3C;
		int unknown_0x48;
		short unknown_0x4C;
		signed char unknown_0x4E;
		signed char unknown_0x4F;
		Reference * activationTarget; // 0x50
		int unknown_0x54;
		IdleNode idles[8]; // 0x58

		std::reference_wrapper<IdleNode[8]> getIdles();
	};
	static_assert(sizeof(AIPackageWander) == 0x98, "TES3::AIPackageWander failed size validation");
	static_assert(sizeof(AIPackageWander::Config) == 0x14, "TES3::AIPackageWander::Config failed size validation");
	static_assert(sizeof(AIPackageWander::IdleNode) == 0x8, "TES3::AIPackageWander::IdleNode failed size validation");

	struct AIPackageEscort : AIPackage {
		struct Config : AIPackageConfig {
			Vector3 destination; // 0x4
			unsigned char duration; // 0x10
			Actor * actor; // 0x14;
			Cell * cell; // 0x18
			bool reset; // 0x1C
		};
		Vector3 destination; // 0x3C
		int unknown_0x48;
		int unknown_0x4C;
	};
	static_assert(sizeof(AIPackageEscort) == 0x50, "TES3::AIPackageEscort failed size validation");
	static_assert(sizeof(AIPackageEscort::Config) == 0x20, "TES3::AIPackageEscort::Config failed size validation");

	struct AIPackageFollow : AIPackage {
		struct Config : AIPackageConfig {
			Vector3 destination; // 0x4
			unsigned char duration; // 0x10
			Actor * actor; // 0x14;
			Cell * cell; // 0x18
			bool reset; // 0x1C
		};
		Vector3 destination; // 0x3C
		int unknown_0x48;
		unsigned char unknown_0x4C;
		unsigned char unknown_0x4D;
		unsigned char unknown_0x4E;
		unsigned char unknown_0x4F;
		int unknown_0x50;
		int unknown_0x54;
		int unknown_0x58;
		int unknown_0x5C;
		int unknown_0x60;
		float followDistance; // 0x64
	};
	static_assert(sizeof(AIPackageFollow) == 0x68, "TES3::AIPackageFollow failed size validation");
	static_assert(sizeof(AIPackageFollow::Config) == 0x20, "TES3::AIPackageFollow::Config failed size validation");

	struct AIPackageActivate : AIPackage {
		struct Config : AIPackageConfig {
			Reference * target; // 0x4
			bool reset; // 0x8
		};
		int unknown_0x3C;
		int unknown_0x40;
		int unknown_0x44;
		int unknown_0x48;
		int unknown_0x4C;
		Reference * activateTarget; // 0x50
	};
	static_assert(sizeof(AIPackageActivate) == 0x54, "TES3::AIPackageActivate failed size validation");
	static_assert(sizeof(AIPackageActivate::Config) == 0xC, "TES3::AIPackageActivate::Config failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3_AIPACKAGE(TES3::AIPackage)
