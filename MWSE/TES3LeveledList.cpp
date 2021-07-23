#include "TES3LeveledList.h"

#include "LuaManager.h"

#include "LuaLeveledCreaturePickedEvent.h"
#include "LuaLeveledItemPickedEvent.h"

namespace TES3 {
	const auto TES3_LeveledCreature_resolve = reinterpret_cast<Object * (__thiscall*)(LeveledCreature*)>(0x4CF870);
	Object * LeveledCreature::resolve() {
		// Call the original function.
		Object * result = TES3_LeveledCreature_resolve(this);

		// Allow the event to override the pick.
		if (mwse::lua::event::LeveledCreaturePickedEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::LeveledCreaturePickedEvent(this, result));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;

				if (eventData.get_or("block", false)) {
					return nullptr;
				}

				result = eventData["pick"];
			}
		}

		return result;
	}

	const auto TES3_LeveledCreature_InsertEntry = reinterpret_cast<bool(__thiscall*)(LeveledCreature*, BaseObject*, short)>(0x4CFCC0);
	bool LeveledCreature::insert(BaseObject* entry, short level) {
		return TES3_LeveledCreature_InsertEntry(this, entry, level);
	}

	const auto TES3_LeveledCreature_RemoveEntry = reinterpret_cast<bool(__thiscall*)(LeveledCreature*, BaseObject*, short)>(0x4CFC40);
	bool LeveledCreature::remove(BaseObject* entry, short level) {
		return TES3_LeveledCreature_RemoveEntry(this, entry, level);
	}

	bool LeveledCreature::getLeveledFlag(LeveledCreatureFlags::Flags flag) const {
		return (flags & flag) != 0;
	}

	void LeveledCreature::setLeveledFlag(LeveledCreatureFlags::Flags flag, bool value) {
		if (value) {
			flags |= flag;
		}
		else {
			flags &= ~flag;
		}
	}

	bool LeveledCreature::getCalculateFromAllLevels() const {
		return getLeveledFlag(LeveledCreatureFlags::CalculateFromAllLevels);
	}

	void LeveledCreature::setCalculateFromAllLevels(bool value) {
		setLeveledFlag(LeveledCreatureFlags::CalculateFromAllLevels, value);
	}

	const auto TES3_LeveledItem_resolve = reinterpret_cast<Object * (__thiscall*)(LeveledItem*)>(0x4D0BD0);
	Object * LeveledItem::resolve() {
		// Call the original function.
		Object * result = TES3_LeveledItem_resolve(this);

		// Allow the event to override the pick.
		if (mwse::lua::event::LeveledItemPickedEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::LeveledItemPickedEvent(this, result));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;

				if (eventData.get_or("block", false)) {
					return nullptr;
				}

				result = eventData["pick"];
			}
		}

		return result;
	}

	const auto TES3_LeveledItem_InsertEntry = reinterpret_cast<bool(__thiscall*)(LeveledItem*, BaseObject*, short)>(0x4D0EE0);
	bool LeveledItem::insert(BaseObject* entry, short level) {
		return TES3_LeveledItem_InsertEntry(this, entry, level);
	}

	const auto TES3_LeveledItem_RemoveEntry = reinterpret_cast<bool(__thiscall*)(LeveledItem*, BaseObject*, short)>(0x4D0E60);
	bool LeveledItem::remove(BaseObject* entry, short level) {
		return TES3_LeveledItem_RemoveEntry(this, entry, level);
	}


	bool LeveledItem::getLeveledFlag(LeveledItemFlags::Flags flag) const {
		return (flags & flag) != 0;
	}

	void LeveledItem::setLeveledFlag(LeveledItemFlags::Flags flag, bool value) {
		if (value) {
			flags |= flag;
		}
		else {
			flags &= ~flag;
		}
	}

	bool LeveledItem::getCalculateForEachItem() const {
		return getLeveledFlag(LeveledItemFlags::CalculateForEachItem);
	}

	void LeveledItem::setCalculateForEachItem(bool value) {
		setLeveledFlag(LeveledItemFlags::CalculateForEachItem, value);
	}

	bool LeveledItem::getCalculateFromAllLevels() const {
		return getLeveledFlag(LeveledItemFlags::CalculateFromAllLevels);
	}

	void LeveledItem::setCalculateFromAllLevels(bool value) {
		setLeveledFlag(LeveledItemFlags::CalculateFromAllLevels, value);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::LeveledCreature)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::LeveledItem)
