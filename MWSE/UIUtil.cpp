#include "UIUtil.h"

#include "LuaManager.h"

#include "LuaShowRestWaitMenuEvent.h"

namespace mwse::tes3::ui {
	const auto TES3_ui_requestMenuModeOff = reinterpret_cast<bool(__cdecl*)()>(0x595270);
	bool requestMenuModeOff() {
		return TES3_ui_requestMenuModeOff();
	}

	const auto TES3_ui_requestMenuModeOn = reinterpret_cast<bool(__cdecl*)(bool)>(0x595230);
	bool requestMenuModeOn(short prop) {
		return TES3_ui_requestMenuModeOn(prop);
	}

	auto& TES3_ui_buttonPressedIndex = *reinterpret_cast<int*>(0x7B88C0);
	int getButtonPressedIndex() {
		return TES3_ui_buttonPressedIndex;
	}

	void resetButtonPressedIndex() {
		TES3_ui_buttonPressedIndex = -1;
	}

	const auto TES3_ui_getMenuNode = reinterpret_cast<TES3::UI::Element * (__cdecl*)(short)>(0x595370);
	TES3::UI::Element* getMenuNode(short id) {
		return TES3_ui_getMenuNode(id);
	}

	short getInventoryMenuId() {
		return *reinterpret_cast<short*>(0x7D3988);
	}

	const auto TES3_ui_inventory_addTile = reinterpret_cast<TES3::UI::InventoryTile * (__cdecl*)(int, TES3::UI::InventoryTile*)>(0x5CBCC0);
	TES3::UI::InventoryTile* inventoryAddTile(int _UNUSED_, TES3::UI::InventoryTile* tile) {
		return TES3_ui_inventory_addTile(_UNUSED_, tile);
	}

	const auto TES3_ui_inventory_updateIcons = reinterpret_cast<void(__cdecl*)()>(0x5CC910);
	void inventoryUpdateIcons() {
		return TES3_ui_inventory_updateIcons();
	}

	const auto TES3_ui_inventory_equipInventoryTileToPlayer = reinterpret_cast<bool(__cdecl*)(TES3::UI::InventoryTile*)>(0x5CE130);
	bool equipInventoryTile(TES3::UI::InventoryTile* tile) {
		return TES3_ui_inventory_equipInventoryTileToPlayer(tile);
	}

	const auto TES3_ui_inventory_equipInventoryItemToPlayer = reinterpret_cast<bool(__cdecl*)(TES3::PhysicalObject*, TES3::ItemData*)>(0x5D1190);
	bool equipInventoryItem(TES3::PhysicalObject* object, TES3::ItemData* data) {
		return TES3_ui_inventory_equipInventoryItemToPlayer(object, data);
	}

	auto& TES3_ui_data_inventory_updatePaperDoll = *reinterpret_cast<bool*>(0x7B6D04);
	void flagPaperDollUpdate() {
		TES3_ui_data_inventory_updatePaperDoll = 1;
	}

	const auto TES3_ui_showRestWaitMenu = reinterpret_cast<void(__cdecl*)(bool)>(0x610170);
	void showRestWaitMenu(bool allowRest, bool scripted) {
		// Execute event. If the event blocked the call, bail.
		if (mwse::lua::event::ShowRestWaitMenuEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::ShowRestWaitMenuEvent(allowRest, scripted));
			if (eventData.valid()) {
				if (eventData.get_or("block", false)) {
					return;
				}

				allowRest = eventData["allowRest"];
			}
		}

		TES3_ui_showRestWaitMenu(allowRest);
	}
}
