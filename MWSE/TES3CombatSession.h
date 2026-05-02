#pragma once

#include "TES3Defines.h"

#include "TES3Deque.h"

namespace TES3 {
	namespace CombatSessionNextAction {
		enum CombatSessionNextAction {
			Undecided = 0,
			Attack = 3,
			AlchemyOrSummon = 6,
			SpellCast = 8,
			UseEnchantedItem = 10
		};
	}

	struct CombatSessionVanilla {
		struct Unknown_0x24 {
			short unknown_0x0;
			char unknown_0x2;
			char unknown_0x3;
			bool unknown_0x4;
			int unknown_0x8;
			int unknown_0xC;
			int unknown_0x10;
			int unknown_0x14;
		};
		float combatDistance; // 0x0
		signed char unknown_0x4;
		signed char unknown_0x5;
		unsigned short mcpAlchemyLastUseTimestamp; // 0x6 // MCP added for potion cooldown.
		int ammoDamage; // 0x8
		MobileActor* parentActor; // 0xC
		EquipmentStack* selectedWeapon; // 0x10
		int unknown_0x14;
		EquipmentStack* selectedShield; // 0x18
		ItemStack* selectedItem; // 0x1C
		Spell* selectedSpell; // 0x20
		Unknown_0x24* unknown_0x24;
		int nextAction; // 0x28
		float spellPriority; // 0x2C
		float alchemyPriority; // 0x30
		Deque<Spell*> spells; // 0x34
		float combatDelayTimer; // 0x40
		float combatDelayNextHitTime; // 0x44
		signed char potionUseFlag; // 0x48
		signed char spellUseFlag; // 0x49
		signed char enchantedItemUseFlag; // 0x4A
		char padding_0x4B;

		//
		// Related static functions.
		// 

		static CombatSessionVanilla* __fastcall ctor(CombatSessionVanilla* self, DWORD _UNUSED_, MobileActor* mobile);
		static void __fastcall dtor(CombatSessionVanilla* self);

	};
	static_assert(sizeof(CombatSessionVanilla) == 0x4C, "TES3::CombatSessionVanilla failed size validation");

	struct CombatSession : CombatSessionVanilla {
		class LuaData {
		public:
			LuaData();

			sol::object data;
		};
		LuaData* luaData;

		//
		// Overrides for vanilla handlers.
		//

		CombatSession(MobileActor* mobile);
		~CombatSession();

		static CombatSession* __fastcall ctor(CombatSession* self, DWORD _UNUSED_, MobileActor* mobile);
		static void __fastcall dtor(CombatSession* self);
		
		//
		// Other related this-call functions.
		//

		float chooseAlchemyWithEffect(short id);

		void changeEquipment(TES3::EquipmentStack* equipmentStack);

		void determineNextAction();

		//
		// Custom functions.
		//

		void setLuaData(sol::object data);
		sol::object getLuaData();

	};
	static_assert(sizeof(CombatSessionVanilla) == 0x4C, "TES3::CombatSession failed size validation");
}
