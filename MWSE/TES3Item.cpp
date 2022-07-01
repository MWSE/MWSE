#include "TES3Item.h"

#include "TES3ItemData.h"

namespace TES3 {
	ItemData * Item::createItemData() {
		return ItemData::createForObject(this);
	}

	bool Item::promptsEquipmentReevaluation() const {
		switch (objectType) {
		case ObjectType::Armor:
		case ObjectType::Clothing:
		case ObjectType::Weapon:
			return true;
		}
		return false;
	}

	sol::table Item::getStolenList_lua(sol::this_state ts) {
		auto stolenList = getStolenList();
		if (!stolenList) {
			return sol::nil;
		}

		sol::state_view state = ts;
		auto list = state.create_table();
		for (const auto& victim : *stolenList) {
			list.add(victim);
		}

		return list;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Item)
