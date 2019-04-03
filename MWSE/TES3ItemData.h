#pragma once

#include "TES3Defines.h"

#include "sol.hpp"

namespace TES3 {
	struct ItemDataVanilla {
		int count; // 0x0
		BaseObject * owner; // 0x4
		union {
			long requiredRank;
			GlobalVariable * requiredVariable;
		}; // 0x8
		union {
			int condition;
			float timeLeft;
		}; // 0xC
		union {
			float charge;
			Actor* soul;
		}; // 0x10
		Script * script; // 0x14
		ScriptVariables * scriptData; // 0x18

		//
		// Related static functions.
		// 

		static ItemDataVanilla * __fastcall ctor(ItemDataVanilla * self);
		static void __fastcall dtor(ItemDataVanilla * self);

		static ItemDataVanilla * __fastcall createForObject(Object * object);

	};
	static_assert(sizeof(ItemDataVanilla) == 0x1C, "TES3::ItemData failed size validation");


	struct ItemData : ItemDataVanilla {
		class LuaData {
		public:
			LuaData();

			sol::table data;
		};
		LuaData * luaData;

		//
		// Overrides for vanilla handlers.
		//

		ItemData();
		~ItemData();

		static ItemData * __fastcall ctor(ItemData * self);
		static void __fastcall dtor(ItemData * self);

		static ItemData * __cdecl createForObject(Object * object);

		static bool __cdecl isFullyRepaired(ItemData * itemData, Item * item, bool fromBarterMenu = false);

		//
		// Custom functions.
		//

		void setLuaDataTable(sol::object data);
		sol::table getOrCreateLuaDataTable();

	};
}
