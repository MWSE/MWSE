#pragma once

#include "TES3Defines.h"

#include "NITArray.h"

#include "TES3IteratedList.h"
#include "TES3Vectors.h"

namespace TES3 {
	struct ItemStack {
		int count; // 0x0
		Object * object; // 0x4
		NI::TArray<ItemData*> * variables; // 0x8

	};
	static_assert(sizeof(ItemStack) == 0xC, "TES3::ItemStack failed size validation");

	struct EquipmentStack {
		Object * object; // 0x0
		ItemData * variables; // 0x4

		//
		// Other related helper functions.
		//

		int getAdjustedValue();
	};
	static_assert(sizeof(EquipmentStack) == 0x8, "TES3::EquipmentStack failed size validation");

	struct Inventory {
		unsigned int flags; // 0x0
		IteratedList<ItemStack*> itemStacks; // 0x4
		int unknown_0x18;

		//
		// Other related this-call functions.
		//

		ItemStack* findItemStack(Object* item);

		int addItem(MobileActor * mobile, Item * item, int count, bool overwriteCount, ItemData ** itemDataRef);
		int addItemWithoutData(MobileActor * mobile, Item * item, int count, bool something);
		ItemData* addItemByReference(MobileActor * mobile, Reference * reference, int * out_count);
		void removeItemWithData(MobileActor * mobile, Item * item, ItemData * itemData, int count, bool deleteStackData);
		void dropItem(MobileActor* mobileActor, Item * item, ItemData * itemData, int count, Vector3 position, Vector3 orientation, bool ignoreItemData = false);

		void resolveLeveledLists(MobileActor* mobile = nullptr);

		//
		// Custom functions.
		//

		// This makes the assumption that there are no free-floating inventories in memory, and that they only exist in actors.
		// This is true everywhere but when checking if the game needs to declone inventories.
		Actor* getActor();

		bool containsItem(Item * item, ItemData * data = nullptr);

		float calculateContainedWeight() const;

		int getSoulGemCount();

		int addItem_lua(sol::table params);
		void removeItem_lua(sol::table params);
		bool contains_lua(sol::object itemOrItemId, sol::optional<TES3::ItemData*> itemData);
		void resolveLeveledLists_lua(sol::optional<MobileActor*> mobile);

		//
		// Container wrapping methods.
		// This allows lua and C++ to interface with this container as if it were the wrapped object.
		//

		using T = ItemStack*;
		using value_type = T;
		using size_type = size_t;
		using difference_type = int;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using iterator = IteratedList<ItemStack*>::iterator;
		using const_iterator = const iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		iterator begin() const { return itemStacks.begin(); }
		iterator end() const { return itemStacks.end(); }
		reverse_iterator rbegin() const { return std::make_reverse_iterator(end()); }
		reverse_iterator rend() const { return std::make_reverse_iterator(begin()); }
		const_iterator cbegin() const { return begin(); }
		const_iterator cend() const { return end(); }
		const_reverse_iterator crbegin() const { return rbegin(); }
		const_reverse_iterator crend() const { return rend(); }
		size_type size() const noexcept { return itemStacks.count; }
		bool empty() const noexcept { return itemStacks.count == 0; }

	};
	static_assert(sizeof(Inventory) == 0x1C, "TES3::Inventory failed size validation");
}
