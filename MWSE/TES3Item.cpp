#include "TES3Item.h"

#include "TES3Actor.h"
#include "TES3ItemData.h"
#include "TES3Light.h"
#include "TES3Misc.h"

#include "CodePatchUtil.h"

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

	int Item::getBaseBarterValue(ItemData* itemData, bool useSoulValue, bool useDurability) const {
		// If we have no item data, just get the base value.
		const auto value = getValue();
		if (itemData == nullptr) {
			return value;
		}

		// Handle soul- and MCP-dependent value.
		if (objectType == TES3::ObjectType::Misc && useSoulValue) {
			auto asMisc = static_cast<const TES3::Misc*>(this);
			if (asMisc->isSoulGem() && itemData->getSoul()) {
				auto soulValue = itemData->getSoul()->getSoulValue().value_or(0);
				if (mwse::mcp::getFeatureEnabled(mwse::mcp::feature::SoulgemValueRebalance)) {
					return (soulValue * soulValue * soulValue) / 10000 + soulValue * 2;
				}
				else {
					return value * soulValue;
				}
			}
		}

		// Manage condition.
		if (useDurability) {
			const auto durability = getDurability();
			if (durability > 0) {
				return value * itemData->condition / durability;
			}

			const auto uses = getUses();
			if (uses > 0) {
				return value * itemData->condition / uses;
			}
		}

		return value;
	}

	bool Item::getIsCarriable() const {
		if (objectType == TES3::ObjectType::Light) {
			return static_cast<const TES3::Light*>(this)->getCanCarry();
		}
		return true;
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
