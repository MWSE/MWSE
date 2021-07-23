#include "TES3UIInventoryTile.h"

#include "BitUtil.h"

namespace TES3 {
	namespace UI {
		bool InventoryTile::getFlag(unsigned int flag) const {
			return BITMASK_TEST(flags, flag);
		}

		void InventoryTile::setFlag(unsigned int flag, bool value) {
			BITMASK_SET(flags, flag, value);
		}

		bool InventoryTile::getIsBartered() const {
			return BITMASK_TEST(flags, InventoryTileFlag::Bartered);
		}

		void InventoryTile::setIsBartered(bool value) {
			BITMASK_SET(flags, InventoryTileFlag::Bartered, value);
		}


		bool InventoryTile::getIsEquipped() const {
			return BITMASK_TEST(flags, InventoryTileFlag::Equipped);
		}

		void InventoryTile::setIsEquipped(bool value) {
			BITMASK_SET(flags, InventoryTileFlag::Equipped, value);
		}
	}
}
