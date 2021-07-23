#pragma once

#include "TES3ObjectLua.h"

#include "TES3Actor.h"

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForTES3Actor(sol::usertype<T>& usertypeDefinition) {
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["actorFlags"] = sol::readonly_property(&TES3::Actor::actorFlags);
			usertypeDefinition["cloneCount"] = sol::readonly_property(&TES3::Actor::cloneCount);
			usertypeDefinition["equipment"] = sol::readonly_property(&TES3::Actor::equipment);
			usertypeDefinition["inventory"] = sol::readonly_property(&TES3::Actor::inventory);

			// Functions exposed as properties.
			usertypeDefinition["barterGold"] = sol::property(&TES3::Actor::getBaseBarterGold, &TES3::Actor::setBaseBarterGold);
			usertypeDefinition["blood"] = sol::property(&TES3::Actor::getBloodType, &TES3::Actor::setBloodType);

			// Basic function binding.
			usertypeDefinition["tradesItemType"] = &TES3::Actor::tradesItemType;
			usertypeDefinition["offersService"] = &TES3::Actor::offersService;

			// Function exposing.
			usertypeDefinition["onInventoryClose"] = &TES3::Actor::onCloseInventory_lua;
			usertypeDefinition["hasItemEquipped"] = &TES3::Actor::hasItemEquipped_lua;
		}
	}
}
