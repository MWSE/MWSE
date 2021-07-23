#pragma once

#include "TES3Defines.h"

#include "TES3IteratedList.h"

namespace TES3 {
	namespace UI {
		enum class InventoryTileType : int {
			Default = 0,
			Equipped = 1,
			Magic = 2,
			MagicEquipped = 3,
			Bartered = 4,
			MagicBartered = 5,
		};

		namespace InventoryTileFlag {
			typedef unsigned int value_type;

			enum Flag : value_type {
				Bartered = 0x4,
				Equipped = 0x8,
			};
		}

		struct InventoryTile_Struct0x8 {
			Inventory * inventory; // 0x0
			int unknown_0x4;
			Reference * reference; // 0x8
			int unknown_0xC; // Count of some kind?
			int unknown_0x10; // Count of some kind?
		};
		static_assert(sizeof(InventoryTile_Struct0x8) == 0x14, "TES3::UI::InventoryTile_Struct0x8 failed size validation");

		struct InventoryTile {
			Item * item; // 0x0
			ItemData * itemData; // 0x4
			IteratedList<InventoryTile_Struct0x8*> * unknown_0x8;
			int count; // 0xC
			Element * element; // 0x10
			char unknown_0x14;
			char padding_0x15[3];
			unsigned int flags; // 0x18
			char itemId[32]; // 0x1C
			unsigned int tileType; // 0x3C
			bool isBoundItem; // 0x40

			//
			// Custom functions.
			//

			bool getFlag(unsigned int flag) const;
			void setFlag(unsigned int flag, bool value);

			bool getIsBartered() const;
			void setIsBartered(bool value);

			bool getIsEquipped() const;
			void setIsEquipped(bool value);

		};
		static_assert(sizeof(InventoryTile) == 0x44, "TES3::UI::InventoryTile failed size validation");
	}
}
