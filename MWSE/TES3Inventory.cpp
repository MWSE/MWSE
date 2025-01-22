#include "TES3Inventory.h"

#include "TES3Util.h"
#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3Enchantment.h"
#include "TES3LeveledList.h"
#include "TES3Item.h"
#include "TES3MobileActor.h"
#include "TES3Spell.h"
#include "TES3Reference.h"

#include "LuaManager.h"
#include "LuaConvertReferenceToItemEvent.h"
#include "LuaLeveledItemPickedEvent.h"

namespace TES3 {
	//
	// QuickKey
	//

	std::tuple<TES3::BaseObject*, TES3::ItemData*> QuickKey::getMagic() {
		if (type != QuickKeyType::Magic) {
			return {};
		}

		if (spell) {
			return { spell, nullptr };
		}
		else {
			return { item, itemData };
		}
	}

	void QuickKey::setMagic(TES3::BaseObject* object, sol::optional<TES3::ItemData*> maybeItemData) {
		if (object == nullptr) {
			throw std::invalid_argument("Could not assign quickslot to a castable object. Given nil value.");
		}
		else if (object->objectType == ObjectType::Spell) {
			type = QuickKeyType::Magic;
			spell = static_cast<Spell*>(object);
			item = nullptr;
			itemData = nullptr;
		}
		else if (object->isItem()) {
			auto objectAsItem = static_cast<Item*>(object);
			auto enchantment = objectAsItem->getEnchantment();
			if (enchantment == nullptr || (enchantment->castType != EnchantmentCastType::OnUse && enchantment->castType != EnchantmentCastType::Once)) {
				throw std::invalid_argument("Could not assign object to a castable quickslot. Item is missing a castable enchantment.");
			}

			type = QuickKeyType::Magic;
			spell = nullptr;
			item = objectAsItem;
			itemData = maybeItemData.value_or(nullptr);
		}
		else {
			throw std::invalid_argument("Could not assign quickslot to a castable object. Must be a spell or item.");
		}
	}

	std::tuple<TES3::Item*, TES3::ItemData*> QuickKey::getItem() {
		if (type != QuickKeyType::Item) {
			return {};
		}
		
		return { item, itemData };
	}

	void QuickKey::setItem(TES3::Item* object, sol::optional<TES3::ItemData*> maybeItemData) {
		if (object == nullptr) {
			throw std::invalid_argument("Could not assign quickslot to an item. Given nil value.");
		}
		else {
			type = QuickKeyType::Item;
			spell = nullptr;
			item = object;
			itemData = maybeItemData.value_or(nullptr);
		}
	}

	void QuickKey::clear() {
		type = QuickKeyType::None;
		spell = nullptr;
		item = nullptr;
		itemData = nullptr;
	}

	QuickKey* QuickKey::getQuickKey(unsigned int slot) {
		const auto slots = reinterpret_cast<TES3::QuickKey*>(0x7D56B0);
		return &slots[slot];
	}
	
	nonstd::span<QuickKey, 9> QuickKey::getQuickKeys() {
		return nonstd::span<QuickKey, 9>(reinterpret_cast<TES3::QuickKey*>(0x7D56B0), 9);
	}

	//
	// EquipmentStack
	//

	void* EquipmentStack::operator new(size_t size) {
		return mwse::tes3::_new(size);
	}

	void EquipmentStack::operator delete(void* block) {
		mwse::tes3::_delete(block);
	}

	EquipmentStack::EquipmentStack() {
		object = nullptr;
		itemData = nullptr;
	}

	const auto TES3_EquipmentStack_CalculateBarterItemValue = reinterpret_cast<int(__cdecl*)(const TES3::EquipmentStack*)>(0x5A46E0);
	int EquipmentStack::getAdjustedValue() const {
		return TES3_EquipmentStack_CalculateBarterItemValue(this);
	}

	const auto TES3_EquipmentStack_CanonicalCopy = reinterpret_cast<TES3::EquipmentStack * (__cdecl*)(const TES3::EquipmentStack*)>(0x465420);
	EquipmentStack* EquipmentStack::canonicalCopy() const {
		return TES3_EquipmentStack_CanonicalCopy(this);
	}

	//
	// Inventory
	//

	const auto TES3_Inventory_findItemStack = reinterpret_cast<ItemStack* (__thiscall*)(Inventory*, Object*)>(0x49A6C0);
	ItemStack* Inventory::findItemStack(Object* item, ItemData* itemData) {
		ItemStack* stack = TES3_Inventory_findItemStack(this, item);

		if (stack && itemData) {
			if (stack->variables && stack->variables->contains(itemData)) {
				return stack;
			}
			else {
				return nullptr;
			}
		}

		return stack;
	}

	const auto TES3_Inventory_AddItem = reinterpret_cast<int(__thiscall*)(Inventory*, MobileActor *, PhysicalObject *, int, bool, ItemData **)>(0x498530);
	int Inventory::addItem(MobileActor * mobile, PhysicalObject * item, int count, bool overwriteCount, ItemData ** itemDataRef) {
		return TES3_Inventory_AddItem(this, mobile, item, count, overwriteCount, itemDataRef);
	}

	const auto TES3_Inventory_AddItemWithoutData = reinterpret_cast<int(__thiscall*)(Inventory*, MobileActor *, PhysicalObject *, int, bool)>(0x497CD0);
	int Inventory::addItemWithoutData(MobileActor * mobile, PhysicalObject * item, int count, bool something) {
		return TES3_Inventory_AddItemWithoutData(this, mobile, item, count, something);
	}

	const auto TES3_Inventory_AddItemByReference = reinterpret_cast<ItemData*(__thiscall*)(Inventory*, MobileActor *, Reference *, int *)>(0x497BC0);
	ItemData* Inventory::addItemByReference(MobileActor * mobile, Reference * reference, int * out_count) {
		if (mwse::lua::event::ConvertReferenceToItemEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::ConvertReferenceToItemEvent(reference));
		}
		return TES3_Inventory_AddItemByReference(this, mobile, reference, out_count);
	}

	const auto TES3_Inventory_removeItemData = reinterpret_cast<ItemData * (__thiscall*)(Inventory*, Item*, ItemData*)>(0x4997F0);
	void Inventory::removeItemData(Item* item, ItemData* itemData) {
		TES3_Inventory_removeItemData(this, item, itemData);
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

	int Inventory::getItemCount(Item* item) {
		auto stack = findItemStack(item);
		if (stack == nullptr) {
			return 0;
		}

		return stack->count;
	}

	int Inventory::getItemCount_lua(sol::object itemOrItemId) {
		if (itemOrItemId.is<TES3::Item*>()) {
			return getItemCount(itemOrItemId.as<TES3::Item*>());
		}
		else if (itemOrItemId.is<const char*>()) {
			auto dataHandler = TES3::DataHandler::get();
			if (dataHandler == nullptr) {
				return 0;
			}

			auto itemId = itemOrItemId.as<const char*>();
			auto item = dataHandler->nonDynamicData->resolveObjectByType<TES3::Item>(itemId);
			if (item == nullptr) {
				return 0;
			}

			return getItemCount(item);
		}

		return 0;
	}

	bool Inventory::containsItem(Item * item, ItemData * data) {
		return findItemStack(item, data) != nullptr;
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
		TES3::PhysicalObject* item = mwse::lua::getOptionalParamObject<TES3::PhysicalObject>(params, "item");
		if (item == nullptr) {
			throw std::invalid_argument("tes3inventory:addItem: Invalid 'item' parameter provided.");
		}
		else if (item->objectType == TES3::ObjectType::LeveledItem) {
			if (mact || getActor()->isClone()) {
				// Actor has already had leveled items resolved. Resolve the leveled item and add the real item if one was rolled.
				item = static_cast<TES3::LeveledItem*>(item)->resolve();
				if (item == nullptr) {
					return 0;
				}
			}
		}
		else if (!item->isItem()) {
			throw std::invalid_argument("tes3inventory:addItem: Invalid 'item' parameter provided.");
		}

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
			auto dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				auto itemId = itemOrItemId.as<const char*>();
				auto item = dataHandler->nonDynamicData->resolveObjectByType<TES3::Item>(itemId);
				return containsItem(item, itemData.value_or(nullptr));
			}
		}
		return false;
	}

	ItemStack* Inventory::findItemStack_lua(sol::object itemOrItemId, sol::optional<TES3::ItemData*> itemData) {
		if (itemOrItemId.is<TES3::Item*>()) {
			auto item = itemOrItemId.as<TES3::Item*>();
			return findItemStack(item, itemData.value_or(nullptr));
		}
		else if (itemOrItemId.is<const char*>()) {
			auto dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				auto itemId = itemOrItemId.as<const char*>();
				auto item = dataHandler->nonDynamicData->resolveObjectByType<TES3::Item>(itemId);
				return findItemStack(item, itemData.value_or(nullptr));
			}
		}
		return nullptr;
	}

	void Inventory::resolveLeveledLists_lua(sol::optional<MobileActor*> mobile) {
		resolveLeveledLists(mobile.value_or(nullptr));
	}
}
