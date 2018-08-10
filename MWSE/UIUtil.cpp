#include "UIUtil.h"

#include "LuaManager.h"
#include "LuaEvents.h"

#define TES3_ui_getMenuOnTop 0x595290

#define TES3_ui_menu_inventory 0x7D3988

#define TES3_ui_requestMenuModeOff 0x595270
#define TES3_ui_requestMenuModeOn 0x595230
#define TES3_ui_getMenuNode 0x595370

#define TES3_ui_messagePlayer 0x5F90C0
#define TES3_ui_buttonPressedIndex 0x7B88C0

#define TES3_ui_inventory_addTile 0x5CBCC0
#define TES3_ui_inventory_updateIcons 0x5CC910
#define TES3_ui_inventory_equipInventoryTileToPlayer 0x5CE130
#define TES3_ui_data_inventory_updatePaperDoll 0x7B6D04

#define TES3_ui_updateStatsPane 0x6266D0

#define TES3_ui_showRestWaitMenu 0x610170

namespace mwse {
	namespace tes3 {
		namespace ui {
			TES3::UI::Element* getTopMenu() {
				return reinterpret_cast<TES3::UI::Element*(__stdcall *)()>(TES3_ui_getMenuOnTop)();
			}

			bool requestMenuModeOff() {
				return reinterpret_cast<signed char(__cdecl *)()>(TES3_ui_requestMenuModeOff)();
			}

			bool requestMenuModeOn(short prop) {
				return reinterpret_cast<signed char(__cdecl *)(short)>(TES3_ui_requestMenuModeOn)(prop);
			}

			int messagePlayer(const char* message) {
				return reinterpret_cast<int(__cdecl *)(const char*, char*, signed char)>(TES3_ui_messagePlayer)(message, 0, 1);
			}

			int getButtonPressedIndex() {
				return *reinterpret_cast<int*>(TES3_ui_buttonPressedIndex);
			}

			void resetButtonPressedIndex() {
				*reinterpret_cast<int*>(TES3_ui_buttonPressedIndex) = -1;
			}

			TES3::UI::Element* getMenuNode(short id) {
				return reinterpret_cast<TES3::UI::Element*(__cdecl *)(short)>(TES3_ui_getMenuNode)(id);
			}

			short getInventoryMenuId() {
				return *reinterpret_cast<short*>(TES3_ui_menu_inventory);
			}

			TES3::UI::InventoryTile* inventoryAddTile(int something, TES3::UI::InventoryTile* tile) {
				return reinterpret_cast<TES3::UI::InventoryTile*(__cdecl *)(int, TES3::UI::InventoryTile*)>(TES3_ui_inventory_addTile)(something, tile);
			}

			signed char inventoryUpdateIcons() {
				return reinterpret_cast<signed char(__cdecl *)()>(TES3_ui_inventory_updateIcons)();
			}

			signed char equipInventoryTile(TES3::UI::InventoryTile* tile) {
				return reinterpret_cast<signed char(__cdecl *)(TES3::UI::InventoryTile*)>(TES3_ui_inventory_equipInventoryTileToPlayer)(tile);
			}

			void flagPaperDollUpdate() {
				*reinterpret_cast<signed char*>(TES3_ui_data_inventory_updatePaperDoll) = 1;
			}

			void updateStatsPane() {
				reinterpret_cast<void(__cdecl *)()>(TES3_ui_updateStatsPane)();
			}

			void showRestWaitMenu(bool allowRest, bool scripted) {
				// Execute event. If the event blocked the call, bail.
				mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
				sol::table eventData = luaManager.triggerEvent(new mwse::lua::event::ShowRestWaitMenuEvent(allowRest, scripted));
				if (eventData.valid()) {
					if (eventData["block"] == true) {
						return;
					}

					allowRest = eventData["allowRest"];
				}

				reinterpret_cast<void(__cdecl *)(bool)>(TES3_ui_showRestWaitMenu)(allowRest);
			}
		}
	}
}
