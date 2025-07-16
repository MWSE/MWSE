#include "TES3UIInventoryTileLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"

#include "TES3Item.h"

namespace mwse::lua {
	void bindTES3UIInventoryTile() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::UI::InventoryTile>("tes3inventoryTile");
		usertypeDefinition["new"] = sol::no_constructor;

		// Functions exposed as properties.
		usertypeDefinition["count"] = sol::readonly_property(&TES3::UI::InventoryTile::count);
		usertypeDefinition["element"] = sol::readonly_property(&TES3::UI::InventoryTile::element);
		usertypeDefinition["flags"] = sol::readonly_property(&TES3::UI::InventoryTile::flags);
		usertypeDefinition["isBoundItem"] = sol::readonly_property(&TES3::UI::InventoryTile::isBoundItem);
		usertypeDefinition["item"] = sol::readonly_property(&TES3::UI::InventoryTile::item);
		usertypeDefinition["itemData"] = sol::readonly_property(&TES3::UI::InventoryTile::itemData);
		usertypeDefinition["type"] = sol::readonly_property(&TES3::UI::InventoryTile::tileType);

		// Access into flags.
		usertypeDefinition["isBartered"] = sol::property(&TES3::UI::InventoryTile::getIsBartered, &TES3::UI::InventoryTile::setIsBartered);
		usertypeDefinition["isEquipped"] = sol::property(&TES3::UI::InventoryTile::getIsEquipped, &TES3::UI::InventoryTile::setIsEquipped);
	}
}
