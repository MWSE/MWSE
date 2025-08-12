#pragma once

#include "TES3Defines.h"

#include "NITArray.h"

#include "TES3IteratedList.h"
#include "TES3Vectors.h"

namespace TES3 {
	enum class QuickKeyType : unsigned int {
		None = 0,
		Item = 1,
		Magic = 2,
	};

	struct QuickKey {
		QuickKeyType type; // 0x0
		Spell* spell; // 0x4
		Item* item; // 0x8
		ItemData* itemData; // 0xC

		QuickKey() = delete;
		~QuickKey() = delete;

		std::tuple<TES3::BaseObject*, TES3::ItemData*> getMagic();
		void setMagic(TES3::BaseObject* object, sol::optional<TES3::ItemData*> itemData);

		std::tuple<TES3::Item*, TES3::ItemData*> getItem();
		void setItem(TES3::Item* object, sol::optional<TES3::ItemData*> itemData);

		void clear();

		static QuickKey* getQuickKey(unsigned int slot);
		static nonstd::span<QuickKey, 9> getQuickKeys();
	};
	static_assert(sizeof(QuickKey) == 0x10, "TES3::QuickKey failed size validation");

	struct ItemStack {
		int count; // 0x0
		Object * object; // 0x4
		NI::TArray<ItemData*> * variables; // 0x8

		ItemStack() = delete;
		~ItemStack() = delete;
	};
	static_assert(sizeof(ItemStack) == 0xC, "TES3::ItemStack failed size validation");

	struct EquipmentStack {
		Object * object; // 0x0
		ItemData * itemData; // 0x4

		static void* operator new(size_t size);
		static void operator delete(void* block);

		EquipmentStack();

		//
		// Other related helper functions.
		//

		int getAdjustedValue() const;
		EquipmentStack* canonicalCopy() const;
	};
	static_assert(sizeof(EquipmentStack) == 0x8, "TES3::EquipmentStack failed size validation");

	struct Inventory {
		unsigned int flags; // 0x0
		IteratedList<ItemStack*> itemStacks; // 0x4
		Light * internalLight; // 0x18

		//
		// Other related this-call functions.
		//

		ItemStack* findItemStack(Object* item, ItemData* itemData = nullptr);

		int addItem(MobileActor * mobile, PhysicalObject * item, int count, bool overwriteCount, ItemData ** itemDataRef);
		int addItemWithoutData(MobileActor * mobile, PhysicalObject * item, int count, bool something);
		ItemData* addItemByReference(MobileActor * mobile, Reference * reference, int * out_count);
		void removeItemData(Item* item, ItemData* itemData);
		void removeItemWithData(MobileActor * mobile, Item * item, ItemData * itemData, int count, bool deleteStackData);
		void dropItem(MobileActor* mobileActor, Item * item, ItemData * itemData, int count, Vector3 position, Vector3 orientation, bool ignoreItemData = false);

		void resolveLeveledLists(MobileActor* mobile = nullptr);

		//
		// Custom functions.
		//

		// This makes the assumption that there are no free-floating inventories in memory, and that they only exist in actors.
		// This is true everywhere but when checking if the game needs to declone inventories.
		Actor* getActor();

		int getItemCount(Item* item);
		int getItemCount_lua(sol::object itemOrItemId);
		bool containsItem(Item * item, ItemData * data = nullptr);

		float calculateContainedWeight() const;

		int getSoulGemCount();

		int addItem_lua(sol::table params);
		void removeItem_lua(sol::table params);
		bool contains_lua(sol::object itemOrItemId, sol::optional<TES3::ItemData*> itemData);
		ItemStack* findItemStack_lua(sol::object itemOrItemId, sol::optional<TES3::ItemData*> itemData);
		void resolveLeveledLists_lua(sol::optional<MobileActor*> mobile);

		//
		// Container wrapping methods.
		// This allows lua and C++ to interface with this container as if it were the wrapped object.
		//

		using value_type = decltype(itemStacks)::value_type;
		using size_type = decltype(itemStacks)::size_type;
		using difference_type = decltype(itemStacks)::difference_type;
		using pointer = decltype(itemStacks)::pointer;
		using const_pointer = decltype(itemStacks)::const_pointer;
		using reference = decltype(itemStacks)::const_reference;
		using const_reference = decltype(itemStacks)::const_reference;
		using iterator = decltype(itemStacks)::iterator;
		using const_iterator = decltype(itemStacks)::const_iterator;
		using reverse_iterator = decltype(itemStacks)::reverse_iterator;
		using const_reverse_iterator = decltype(itemStacks)::const_reverse_iterator;

		iterator begin() const { return itemStacks.begin(); }
		iterator end() const { return itemStacks.end(); }
		reverse_iterator rbegin() const { return itemStacks.rbegin(); }
		reverse_iterator rend() const { return itemStacks.rend(); }
		const_iterator cbegin() const { return itemStacks.cbegin(); }
		const_iterator cend() const { return itemStacks.cend(); }
		const_reverse_iterator crbegin() const { return itemStacks.crbegin(); }
		const_reverse_iterator crend() const { return itemStacks.crend(); }
		size_type size() const noexcept { return itemStacks.size(); }
		bool empty() const noexcept { return itemStacks.empty(); }

	};
	static_assert(sizeof(Inventory) == 0x1C, "TES3::Inventory failed size validation");
}
