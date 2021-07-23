#pragma once

#include "TES3Defines.h"

namespace mwse {
	namespace tes3 {
		namespace ui {
			bool requestMenuModeOff();
			bool requestMenuModeOn(short prop = 0);

			int getButtonPressedIndex();
			void resetButtonPressedIndex();

			TES3::UI::Element* getMenuNode(short);
			short getInventoryMenuId();

			TES3::UI::InventoryTile* inventoryAddTile(int, TES3::UI::InventoryTile*);
			signed char inventoryUpdateIcons();
			signed char equipInventoryTile(TES3::UI::InventoryTile*);
			signed char equipInventoryItem(TES3::PhysicalObject*, TES3::ItemData*);
			void flagPaperDollUpdate();

			void showRestWaitMenu(bool allowRest, bool scripted = true);
		}
	}
}
