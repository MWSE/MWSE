#include "TES3Inventory.h"

#include "TES3Util.h"
#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3Item.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

#include "LuaManager.h"
#include "LuaConvertReferenceToItemEvent.h"
#include "LuaLeveledItemPickedEvent.h"

namespace TES3 {
	//
	// EquipmentStack
	//

	const auto TES3_EquipmentStack_CalculateBarterItemValue = reinterpret_cast<int(__cdecl*)(const TES3::EquipmentStack*)>(0x5A46E0);
	int EquipmentStack::getAdjustedValue() {
		return TES3_EquipmentStack_CalculateBarterItemValue(this);
	}

	//
	// Inventory
	//

	const auto TES3_Inventory_findItemStack = reinterpret_cast<ItemStack* (__thiscall*)(Inventory*, Object*)>(0x49A6C0);
	ItemStack* Inventory::findItemStack(Object* item) {
		return TES3_Inventory_findItemStack(this, item);
	}

	const auto TES3_Inventory_AddItem = reinterpret_cast<int(__thiscall*)(Inventory*, MobileActor *, Item *, int, bool, ItemData **)>(0x498530);
	int Inventory::addItem(MobileActor * mobile, Item * item, int count, bool overwriteCount, ItemData ** itemDataRef) {
		return TES3_Inventory_AddItem(this, mobile, item, count, overwriteCount, itemDataRef);
	}

	const auto TES3_Inventory_AddItemWithoutData = reinterpret_cast<int(__thiscall*)(Inventory*, MobileActor *, Item *, int, bool)>(0x497CD0);
	int Inventory::addItemWithoutData(MobileActor * mobile, Item * item, int count, bool something) {
		return TES3_Inventory_AddItemWithoutData(this, mobile, item, count, something);
	}

	const auto TES3_Inventory_AddItemByReference = reinterpret_cast<ItemData*(__thiscall*)(Inventory*, MobileActor *, Reference *, int *)>(0x497BC0);
	ItemData* Inventory::addItemByReference(MobileActor * mobile, Reference * reference, int * out_count) {
		if (mwse::lua::event::ConvertReferenceToItemEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::ConvertReferenceToItemEvent(reference));
		}
		return TES3_Inventory_AddItemByReference(this, mobile, reference, out_count);
	}

	const auto TES3_Inventory_RemoveItemWithData = reinterpret_cast<void(__thiscall*)(Inventory*, MobileActor*, Item *, ItemData *, int, bool)>(0x499550);
	void Inventory::removeItemWithData(MobileActor * mobile, Item * item, ItemData * itemData, int count, bool deleteStackData) {
		TES3_Inventory_RemoveItemWithData(this, mobile, item, itemData, count, deleteStackData);
	}

	const auto TES3_Inventory_DropItem = reinterpret_cast<void(__thiscall*)(Inventory*, MobileActor*, Item *, ItemData *, int, Vector3, Vector3, bool)>(0x49B090);
	void Inventory::dropItem(MobileActor* mobileActor, Item * item, ItemData * itemData, int count, Vector3 position, Vector3 orientation, bool ignoreItemData) {
		TES3_Inventory_DropItem(this, mobileActor, item, itemData, count, position, orientation, ignoreItemData);
	}

	// Note: A custom call to TES3_Inventory_resolveLeveledLists is made in LuaManager.cpp.
	const auto TES3_Inventory_resolveLeveledLists = reinterpret_cast<void(__thiscall*)(Inventory*, MobileActor*)>(0x49A190);
	void Inventory::resolveLeveledLists(MobileActor* mobile) {
		mwse::lua::event::LeveledItemPickedEvent::m_Reference = mobile ? mobile->reference : getActor()->getReference();
		TES3_Inventory_resolveLeveledLists(this, mobile);
		mwse::lua::event::LeveledItemPickedEvent::m_Reference = nullptr;
	}

	Actor* Inventory::getActor() {
		return reinterpret_cast<Actor*>(reinterpret_cast<BYTE*>(this) - offsetof(Actor, inventory));
	}

	bool Inventory::containsItem(Item * item, ItemData * data) {
		ItemStack * stack = findItemStack(item);
		if (stack == nullptr) {
			return false;
		}

		if (data) {
			if (stack->variables) {
				return stack->variables->contains(data);
			}
			else {
				return false;
			}
		}

		return true;
	}

	const auto TES3_Inventory_calculateContainedWeight = reinterpret_cast<float(__thiscall*)(const Inventory*)>(0x49A080);
	float Inventory::calculateContainedWeight() const {
		return TES3_Inventory_calculateContainedWeight(this);
	}

	int Inventory::getSoulGemCount() {
		int count = 0;
		for (auto& stack : itemStacks) {
			if (mwse::tes3::isSoulGem(stack->object)) {
				count++;
			}
		}
		return count;
	}

	int Inventory::addItem_lua(sol::table params) {
		TES3::MobileActor* mact = mwse::lua::getOptionalParamMobileActor(params, "mobile");
		TES3::Item* item = mwse::lua::getOptionalParamObject<TES3::Item>(params, "item");
		int count = mwse::lua::getOptionalParam<int>(params, "count", 1);
		TES3::ItemData* itemData = mwse::lua::getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);
		return addItem(mact, item, count, false, itemData ? &itemData : nullptr);
	}

	void Inventory::removeItem_lua(sol::table params) {
		TES3::MobileActor* mact = mwse::lua::getOptionalParamMobileActor(params, "mobile");
		TES3::Item* item = mwse::lua::getOptionalParamObject<TES3::Item>(params, "item");
		int count = mwse::lua::getOptionalParam<int>(params, "count", 1);
		TES3::ItemData* itemData = mwse::lua::getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);
		bool deleteItemData = mwse::lua::getOptionalParam<bool>(params, "deleteItemData", false);
		removeItemWithData(mact, item, itemData, count, deleteItemData);
	}

	bool Inventory::contains_lua(sol::object itemOrItemId, sol::optional<TES3::ItemData*> itemData) {
		if (itemOrItemId.is<TES3::Item*>()) {
			auto item = itemOrItemId.as<TES3::Item*>();
			return containsItem(item, itemData.value_or(nullptr));
		}
		else if (itemOrItemId.is<const char*>()) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				auto itemId = itemOrItemId.as<const char*>();
				auto item = dataHandler->nonDynamicData->resolveObjectByType<TES3::Item>(itemId);
				return containsItem(item, itemData.value_or(nullptr));
			}
		}
		return false;
	}

	void Inventory::resolveLeveledLists_lua(sol::optional<MobileActor*> mobile) {
		resolveLeveledLists(mobile.value_or(nullptr));
	}
}
