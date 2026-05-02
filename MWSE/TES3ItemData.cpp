#include "TES3ItemData.h"

#include "TES3Creature.h"
#include "TES3DataHandler.h"
#include "TES3Enchantment.h"
#include "TES3GlobalVariable.h"
#include "TES3Light.h"
#include "TES3Misc.h"
#include "TES3Script.h"
#include "TES3Weapon.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "MemoryUtil.h"

#include "TES3ScriptLua.h"

namespace TES3 {
	//
	// Vanilla ItemData
	//

	void* ItemDataVanilla::operator new(size_t size) {
		return mwse::tes3::_new(size);
	}
	void ItemDataVanilla::operator delete(void* address) {
		mwse::tes3::_delete(address);
	}

	const auto TES3_ItemData_constructor = reinterpret_cast<ItemDataVanilla* (__thiscall*)(ItemDataVanilla*)>(0x4E44B0);
	ItemDataVanilla* ItemDataVanilla::ctor(ItemDataVanilla* self) {
		return TES3_ItemData_constructor(self);
	}

	const auto TES3_ItemData_destructor = reinterpret_cast<void (__thiscall *)(ItemDataVanilla*)>(0x4E44E0);
	void ItemDataVanilla::dtor(ItemDataVanilla* self) {
		TES3_ItemData_destructor(self);
	}

	ItemDataVanilla* ItemDataVanilla::createForObject(Object* object) {
		return reinterpret_cast<TES3::ItemDataVanilla* (__cdecl*)(TES3::BaseObject*)>(0x4E7750)(object);
	}

	//
	// MWSE-Extended ItemData
	//

	ItemData::LuaData::LuaData() {
		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		data = stateHandle.getState().create_table();
		tempData = stateHandle.getState().create_table();
	}

	ItemData::ItemData() {
		ctor(this);
	}

	ItemData::~ItemData() {
		dtor(this);
	}

	ItemData* ItemData::ctor(ItemData* self) {
		ItemDataVanilla::ctor(self);
		self->luaData = nullptr;
		self->flags = 0;
		return self;
	}

	void ItemData::dtor(ItemData* self) {
		ItemDataVanilla::dtor(self);

		if (self->luaData) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			delete self->luaData;
			self->luaData = nullptr;
		}
	}

	ItemData* ItemData::createForObject(Object* object) {
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
			itemData->timeLeft = float(static_cast<Light*>(object)->time);
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

	ItemData* ItemData::createForBoundItem(Object* object) {
		auto itemData = createForObject(object);
		itemData->flags |= ItemDataFlags::ItemDataFlag_BoundItem;
		return itemData;
	}

	const auto TES3_IsItemDataStackable = reinterpret_cast<bool (__cdecl*)(ItemDataVanilla*, Item*, bool)>(0x4E7970);
	bool ItemData::isItemDataStackable(ItemData* itemData, Item* item, bool ignoreOwnership) {
		// Note: itemData argument may be nullptr.

		// Vanilla checks.
		if (!TES3_IsItemDataStackable(itemData, item, ignoreOwnership)) {
			return false;
		}
		
		// Due to checks in TES3_IsItemDataStackable, after this point item and itemData are non-null.

		// Bound items must not lose item data.
		if (itemData->flags & ItemDataFlags::ItemDataFlag_BoundItem) {
			return false;
		}

		// Lua data.
		if (itemData->luaData) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			static sol::protected_function fnTableEmpty = stateHandle.getState()["table"]["empty"];
			if (!fnTableEmpty(itemData->luaData->data, true) || !fnTableEmpty(itemData->luaData->tempData, true)) {
				return false;
			}
		}

		return true;
	}

	TES3::BaseObject* ItemData::getOwner() const {
		return owner;
	}

	void ItemData::setOwner_lua(sol::object value) {
		if (value == sol::nil) {
			owner = nullptr;
		}
		else if (value.is<TES3::BaseObject*>()) {
			auto newOwner = value.as<TES3::BaseObject*>();

			// We only support NPC and Faction owners.
			if (newOwner->objectType != TES3::ObjectType::NPC && newOwner->objectType != TES3::ObjectType::Faction) {
				throw std::exception("Ownership must be an NPC or a faction.");
			}

			// If the owner type is changing, reset the requirement.
			const auto oldOwnerType = owner ? owner->objectType : ObjectType::Invalid;
			if (oldOwnerType != newOwner->objectType) {
				requiredVariable = nullptr;
			}

			owner = newOwner;
		}
	}

	sol::object ItemData::getOwnerRequirement_lua(sol::this_state ts) const {
		if (owner == nullptr) {
			return sol::nil;
		}
		else if (owner->objectType == TES3::ObjectType::Faction) {
			sol::state_view state = ts;
			return sol::make_object(state, requiredRank);
		}
		else if (owner->objectType == TES3::ObjectType::NPC) {
			sol::state_view state = ts;
			return sol::make_object(state, requiredVariable);
		}

		return sol::nil;
	}

	void ItemData::setOwnerRequirement_lua(sol::object value) {
		if (owner == nullptr) {
			throw std::exception("An owner must be set before the requirement can be set.");
		}
		else if (owner->objectType == TES3::ObjectType::Faction) {
			if (value.is<int>()) {
				requiredRank = value.as<int>();
			}
			else if (value == sol::nil) {
				requiredRank = 0;
			}
			else {
				throw std::exception("Faction ownership used. Requirement must be a rank (number).");
			}
		}
		else if (owner->objectType == TES3::ObjectType::NPC) {
			if (value.is<TES3::GlobalVariable*>()) {
				requiredVariable = value.as<TES3::GlobalVariable*>();
			}
			else if (value == sol::nil) {
				requiredVariable = nullptr;
			}
			else {
				throw std::exception("NPC ownership used. Requirement must be a global variable.");
			}
		}
	}

	Actor* ItemData::getSoul() const {
		__try {
			if (soul != nullptr && charge != -1.0f && soul->isActor()) {
				return soul;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return nullptr;
		}

		return nullptr;
	}

	void ItemData::setSoul_lua(sol::object actor) {
		if (charge == -1.0f) {
			return;
		}
		else if (actor.is<TES3::Creature*>()) {
			soul = actor.as<TES3::Creature*>();
		}
		else if (actor.is<TES3::CreatureInstance*>()) {
			soul = actor.as<TES3::CreatureInstance*>()->baseCreature;
		}
		else if (actor == sol::nil) {
			soul = nullptr;
		}
	}

	bool ItemData::isBoundItem() const {
		return (flags & ItemDataFlags::ItemDataFlag_BoundItem) != 0;
	}

	void ItemData::setLuaDataTable(sol::object data) {
		if (data == sol::nil) {
			if (luaData != nullptr) {
				luaData->data = sol::nil;
				luaData->tempData = sol::nil;
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

	void ItemData::setLuaTempDataTable(sol::object data) {
		if (data == sol::nil) {
			if (luaData != nullptr) {
				luaData->data = sol::nil;
				luaData->tempData = sol::nil;
				delete luaData;
				luaData = nullptr;
			}
		}
		else if (data.is<sol::table>()) {
			if (luaData == nullptr) {
				luaData = new TES3::ItemData::LuaData();
			}
			luaData->tempData = data;
		}
		else {
			throw std::exception("Invalid data type assignment. Must be a table or nil.");
		}
	}

	sol::table ItemData::getOrCreateLuaDataTable() {
		if (luaData == nullptr) {
			luaData = new ItemData::LuaData();
		}

		return luaData->data;
	}

	sol::table ItemData::getOrCreateLuaTempDataTable() {
		if (luaData == nullptr) {
			luaData = new ItemData::LuaData();
		}

		return luaData->tempData;
	}

	std::shared_ptr<mwse::lua::ScriptContext> ItemData::createContext() {
		return std::make_shared<mwse::lua::ScriptContext>(script, scriptData);
	}

	bool ItemData::setScriptShortValue(const char* name, short value) {
		if (script) {
			auto varIndex = script->getShortVarIndex(name);
			if (varIndex.has_value()) {
				scriptData->shortVarValues[varIndex.value()] = value;
				return true;
			}
		}
		return false;
	}
}
