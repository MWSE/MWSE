#pragma once

#include "TES3Defines.h"

namespace mwse {
	namespace tes3 {
		namespace ui {
			TES3::UI::Element* getTopMenu();

			bool requestMenuModeOff();
			bool requestMenuModeOn(short prop = 0);

			int messagePlayer(const char*);
			int getButtonPressedIndex();
			void resetButtonPressedIndex();

			TES3::UI::Element* getMenuNode(short);
			short getInventoryMenuId();

			TES3::UI::InventoryTile* inventoryAddTile(int, TES3::UI::InventoryTile*);
			signed char inventoryUpdateIcons();
			signed char equipInventoryTile(TES3::UI::InventoryTile*);
			void flagPaperDollUpdate();

			void updateStatsPane();

			void showRestWaitMenu(bool allowRest, bool scripted = true);
		}
	}
}
