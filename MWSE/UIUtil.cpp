#include "UIUtil.h"

#include "LuaManager.h"

#include "LuaShowRestWaitMenuEvent.h"

#define TES3_ui_menu_inventory 0x7D3988

#define TES3_ui_requestMenuModeOff 0x595270
#define TES3_ui_requestMenuModeOn 0x595230
#define TES3_ui_getMenuNode 0x595370

#define TES3_ui_buttonPressedIndex 0x7B88C0

#define TES3_ui_inventory_addTile 0x5CBCC0
#define TES3_ui_inventory_updateIcons 0x5CC910
#define TES3_ui_inventory_equipInventoryTileToPlayer 0x5CE130
#define TES3_ui_inventory_equipInventoryItemToPlayer 0x5D1190
#define TES3_ui_data_inventory_updatePaperDoll 0x7B6D04

#define TES3_ui_showRestWaitMenu 0x610170

namespace mwse {
	namespace tes3 {
		namespace ui {
			bool requestMenuModeOff() {
				return reinterpret_cast<signed char(__cdecl *)()>(TES3_ui_requestMenuModeOff)();
			}

			bool requestMenuModeOn(short prop) {
				return reinterpret_cast<signed char(__cdecl *)(short)>(TES3_ui_requestMenuModeOn)(prop);
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

			signed char equipInventoryItem(TES3::PhysicalObject* object, TES3::ItemData* data) {
				return reinterpret_cast<signed char(__cdecl *)(TES3::PhysicalObject*, TES3::ItemData*)>(TES3_ui_inventory_equipInventoryItemToPlayer)(object, data);
			}

			void flagPaperDollUpdate() {
				*reinterpret_cast<signed char*>(TES3_ui_data_inventory_updatePaperDoll) = 1;
			}

			void showRestWaitMenu(bool allowRest, bool scripted) {
				// Execute event. If the event blocked the call, bail.
				if (mwse::lua::event::ShowRestWaitMenuEvent::getEventEnabled()) {
					mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
					auto stateHandle = luaManager.getThreadSafeStateHandle();
					sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::ShowRestWaitMenuEvent(allowRest, scripted));
					if (eventData.valid()) {
						if (eventData.get_or("block", false)) {
							return;
						}

						allowRest = eventData["allowRest"];
					}
				}

				reinterpret_cast<void(__cdecl *)(bool)>(TES3_ui_showRestWaitMenu)(allowRest);
			}
		}
	}
}
