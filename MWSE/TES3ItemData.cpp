#include "TES3ItemData.h"

#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Enchantment.h"
#include "TES3Light.h"
#include "TES3Misc.h"
#include "TES3Weapon.h"

#include "LuaManager.h"

#include <unordered_set>
#include <Windows.h>

#if _DEBUG
#define DEBUG_CUSTOM_ITEMDATA_EXTENSIONS TRUE
#else
#define DEBUG_CUSTOM_ITEMDATA_EXTENSIONS FALSE
#endif

namespace TES3 {
	//
	// Vanilla ItemData
	//

	const auto TES3_ItemData_constructor = reinterpret_cast<ItemDataVanilla*(__thiscall *)(ItemDataVanilla*)>(0x4E44B0);
	ItemDataVanilla * ItemDataVanilla::ctor(ItemDataVanilla * self) {
		return TES3_ItemData_constructor(self);
	}

	const auto TES3_ItemData_destructor = reinterpret_cast<void(__thiscall *)(ItemDataVanilla*)>(0x4E44E0);
	void ItemDataVanilla::dtor(ItemDataVanilla * self) {
		TES3_ItemData_destructor(self);
	}

	ItemDataVanilla * ItemDataVanilla::createForObject(Object * object) {
		return reinterpret_cast<TES3::ItemDataVanilla*(__cdecl *)(TES3::BaseObject*)>(0x4E7750)(object);
	}

	//
	// MWSE-Extended ItemData
	//

#if DEBUG_CUSTOM_ITEMDATA_EXTENSIONS
	std::unordered_set<ItemData*> validItemDataCache;
	std::mutex validItemDataCacheMutex;
#endif

	ItemData::LuaData::LuaData() {
		data = mwse::lua::LuaManager::getInstance().createTable();
	}

	ItemData::ItemData() {
		ctor(this);
	}

	ItemData::~ItemData() {
		dtor(this);
	}

	ItemData * ItemData::ctor(ItemData * self) {
		ItemDataVanilla::ctor(self);
		self->luaData = nullptr;

#if DEBUG_CUSTOM_ITEMDATA_EXTENSIONS
		if (ItemData::test_itemDataIsManaged(self)) {
			throw std::exception("Attempting to construct already handled ItemData.");
		}

		validItemDataCacheMutex.lock();
		validItemDataCache.insert(self);
		validItemDataCacheMutex.unlock();
#endif

		return self;
	}

	std::queue<TES3::ItemData::LuaData*> threadedDeletionQueue;

	void ItemData::dtor(ItemData * self) {
		ItemDataVanilla::dtor(self);

#if DEBUG_CUSTOM_ITEMDATA_EXTENSIONS
		if (!ItemData::test_itemDataIsManaged(self)) {
			throw std::exception("Attempting to destruct invalid ItemData.");
		}
		else {
			validItemDataCacheMutex.lock();
			validItemDataCache.erase(self);
			validItemDataCacheMutex.unlock();
		}

#endif

		if (self->luaData) {
			auto dataHandler = TES3::DataHandler::get();
			if (dataHandler != nullptr && dataHandler->mainThreadID != GetCurrentThreadId()) {
				threadedDeletionQueue.push(self->luaData);
				self->luaData = nullptr;
			}
			else {
				while (!threadedDeletionQueue.empty()) {
					auto itemData = threadedDeletionQueue.front();
					threadedDeletionQueue.pop();
					delete itemData;
				}

				delete self->luaData;
				self->luaData = nullptr;
			}
		}
	}

	ItemData * ItemData::createForObject(Object * object) {
		auto itemData = mwse::tes3::_new<ItemData>();
		ItemData::ctor(itemData);

		switch (object->objectType) {
		case TES3::ObjectType::Armor:
			itemData->condition = object->getDurability();
			break;
		case TES3::ObjectType::Weapon:
			if (static_cast<Weapon*>(object)->hasDurability()) {
				itemData->condition = object->getDurability();
			}
			break;
		case TES3::ObjectType::Lockpick:
		case TES3::ObjectType::Probe:
		case TES3::ObjectType::Repair:
			itemData->condition = object->getUses();
			break;
		case TES3::ObjectType::Light:
			itemData->timeLeft = static_cast<Light*>(object)->time;
			break;
		case TES3::ObjectType::Misc:
			itemData->count = static_cast<Misc*>(object)->getGoldStackCount();
			break;
		}

		auto enchantment = object->getEnchantment();
		if (enchantment) {
			itemData->charge = enchantment->maxCharge;
		}

		return itemData;
	}

	const auto TES3_IsItemFullyRepaired = reinterpret_cast<bool(__cdecl *)(ItemDataVanilla*, Item*, bool)>(0x4E7970);
	bool ItemData::isFullyRepaired(ItemData * itemData, Item * item, bool fromBarterMenu) {
		if (!TES3_IsItemFullyRepaired(itemData, item, fromBarterMenu)) {
			return false;
		}

#if DEBUG_CUSTOM_ITEMDATA_EXTENSIONS
		if (!ItemData::test_itemDataIsManaged(itemData)) {
			throw std::exception("Attempting to check repair for invalid ItemData.");
		}
#endif

		if (itemData->luaData) {
			// We can only check the table state from the main thread. If we aren't on the main thread, assume that the table isn't empty.
			if (TES3::DataHandler::get()->mainThreadID == GetCurrentThreadId()) {
				return itemData->luaData->data.empty();
			}
			else {
				return false;
			}
		}

		return true;
	}

	void ItemData::setLuaDataTable(sol::object data) {
#if DEBUG_CUSTOM_ITEMDATA_EXTENSIONS
		if (!ItemData::test_itemDataIsManaged(this)) {
			throw std::exception("Attempting to set lua table for invalid ItemData.");
		}

		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler != nullptr && dataHandler->mainThreadID != GetCurrentThreadId()) {
			throw std::exception("Cannot be called from outside the main thread.");
		}
#endif

		if (data == sol::nil) {
			if (luaData != nullptr) {
				luaData->data = sol::nil;
				delete luaData;
				luaData = nullptr;
			}
		}
		else if (data.is<sol::table>()) {
			if (luaData == nullptr) {
				luaData = new TES3::ItemData::LuaData();
			}
			luaData->data = data;
		}
		else {
			throw std::exception("Invalid data type assignment. Must be a table or nil.");
		}
	}

	sol::table ItemData::getOrCreateLuaDataTable() {
#if DEBUG_CUSTOM_ITEMDATA_EXTENSIONS
		if (!ItemData::test_itemDataIsManaged(this)) {
			throw std::exception("Attempting to create lua table for invalid ItemData.");
		}

		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler->mainThreadID != GetCurrentThreadId()) {
			throw std::exception("Cannot be called from outside the main thread.");
		}
#endif

		if (luaData == nullptr) {
			luaData = new ItemData::LuaData();
		}

		return luaData->data;
	}

	bool ItemData::test_itemDataIsManaged(ItemData * itemData) {
#if DEBUG_CUSTOM_ITEMDATA_EXTENSIONS
		if (itemData == nullptr) {
			return true;
		}

		validItemDataCacheMutex.lock();
		auto count = validItemDataCache.count(itemData);
		validItemDataCacheMutex.unlock();
		if (count > 1) {
			throw std::exception("Managed ItemData is mishandled.");
		}
		return count != 0;
#else
		return true;
#endif
	}
}
