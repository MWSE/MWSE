#include "TES3BaseObject.h"

#include "TES3GameFile.h"

#include "BitUtil.h"

#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1

#include "LuaManager.h"
#include "ReferenceTracker.h"

#include "TES3Activator.h"
#include "TES3Actor.h"
#include "TES3Alchemy.h"
#include "TES3Apparatus.h"
#include "TES3Birthsign.h"
#include "TES3BodyPart.h"
#include "TES3Book.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3Container.h"
#include "TES3Creature.h"
#include "TES3DataHandler.h"
#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"
#include "TES3Door.h"
#include "TES3Enchantment.h"
#include "TES3Faction.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3Ingredient.h"
#include "TES3Item.h"
#include "TES3Land.h"
#include "TES3LeveledList.h"
#include "TES3Light.h"
#include "TES3Lockpick.h"
#include "TES3Misc.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3Probe.h"
#include "TES3Quest.h"
#include "TES3Race.h"
#include "TES3Reference.h"
#include "TES3Region.h"
#include "TES3RepairTool.h"
#include "TES3Script.h"
#include "TES3Skill.h"
#include "TES3Sound.h"
#include "TES3SoundGenerator.h"
#include "TES3Spell.h"
#include "TES3Static.h"
#include "TES3UIMenuController.h"
#include "TES3Weapon.h"
#include "TES3WorldController.h"

#include "LuaObjectCopiedEvent.h"
#include "LuaObjectInvalidatedEvent.h"

#include "Log.h"
#include "LuaUtil.h"

#include "MemoryUtil.h"

#include <sstream>
#include <unordered_map>
#include <unordered_set>

#elif defined(SE_TARGETS_CS) && SE_TARGETS_CS == 1

#include "CSBirthsign.h"
#include "CSClass.h"
#include "CSFaction.h"
#include "CSRace.h"
#include "CSScript.h"

#include "StringUtil.h"

#endif

namespace TES3 {
	const char* BaseObject::getObjectID() const {
		return vTable.base->getObjectID(this);
	}

	const char* BaseObject::getSourceFilename() const {
		if (sourceFile) {
			return sourceFile->fileName;
		}
		return nullptr;
	}

	bool BaseObject::isFromMaster() const {
		return BIT_TEST(objectFlags, ObjectFlag::FromMasterBit);
	}

	bool BaseObject::getModified() const {
		return BIT_TEST(objectFlags, ObjectFlag::ModifiedBit);
	}

	void BaseObject::setModified(bool modified) {
		vTable.base->setModified(this, modified);
	}

	bool BaseObject::getDisabled() const {
		return BIT_TEST(objectFlags, ObjectFlag::DisabledBit);
	}

	bool BaseObject::getDeleted() const {
		return BIT_TEST(objectFlags, ObjectFlag::DeleteBit);
	}

	void BaseObject::setDeleted(bool deleted) {
#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1
		
		BIT_SET(objectFlags, ObjectFlag::DeleteBit, deleted);

#elif defined(SE_TARGETS_CS) && SE_TARGETS_CS == 1

		const auto BaseObject_setDeleted = reinterpret_cast<void(__thiscall*)(BaseObject*, bool)>(0x547810);
		BaseObject_setDeleted(this, deleted);
#endif
	}

	bool BaseObject::getPersistent() const {
		return BIT_TEST(objectFlags, ObjectFlag::PersistentBit);
	}

	void BaseObject::setPersistent(bool value) {
		BIT_SET(objectFlags, ObjectFlag::PersistentBit, value);
	}

	bool BaseObject::getBlocked() const {
		return BIT_TEST(objectFlags, ObjectFlag::BlockedBit);
	}

	void BaseObject::setBlocked(bool value) {
		BIT_SET(objectFlags, ObjectFlag::BlockedBit, value);
	}

	bool BaseObject::getScaleModifiedToOne() const {
		return BIT_TEST(objectFlags, ObjectFlag::ScaleModifiedToOneBit);
	}

	void BaseObject::setScaleModifiedToOne(bool value) {
		BIT_SET(objectFlags, ObjectFlag::ScaleModifiedToOneBit, value);
	}

	bool BaseObject::isMobileCapableActor() const {
		switch (objectType) {
		case ObjectType::Creature:
		case ObjectType::CreatureClone:
		case ObjectType::NPC:
		case ObjectType::NPCClone:
			return true;
		default:
			return false;
		}
	}

#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1
	void* BaseObject::operator new(size_t size) {
		return se::memory::_new(size);
	}

	void BaseObject::operator delete(void* address) {
		se::memory::_delete(address);

	}

	const auto BaseObject_dtor = reinterpret_cast<BaseObject * (__thiscall*)(BaseObject*)>(0x4F0CA0);
	void BaseObject::dtor() {
		const auto dataHandler = TES3::DataHandler::get();

		clearCachedLuaObject(this);
		mwse::ReferenceTracker::invalidateObject(this);

		if (objectType == ObjectType::Cell && dataHandler && dataHandler->nonDynamicData) {
			dataHandler->nonDynamicData->clearCellByNameCache(static_cast<const Cell*>(this));
		}

		if (UI::MenuInputController::lastTooltipObject == this) {
			UI::MenuInputController::lastTooltipObject = nullptr;
			UI::MenuInputController::lastTooltipItemData = nullptr;
			UI::MenuInputController::lastTooltipCount = 0;
		}

		using namespace mwse::lua::event;
		if (ObjectCopiedEvent::ms_LastCopied == this || ObjectCopiedEvent::ms_LastCopiedFrom == this) {
			ObjectCopiedEvent::ms_LastCopied = nullptr;
			ObjectCopiedEvent::ms_LastCopiedFrom = nullptr;
		}

		BaseObject_dtor(this);
	}

	const auto BaseObject_writeFileHeader = reinterpret_cast<bool(__thiscall*)(const BaseObject*, GameFile*)>(0x4EEE60);
	bool BaseObject::writeFileHeader(GameFile* file) const {
		return BaseObject_writeFileHeader(this, file);
	}

	BaseObject* BaseObject::getBaseObject() {
		auto object = static_cast<BaseObject*>(this);

		if (object->objectType == ObjectType::Reference) {
			object = static_cast<Reference*>(object)->baseObject;
		}

		if (object->isActor() && static_cast<Actor*>(object)->isClone()) {
			object = static_cast<Actor*>(object)->getBaseActor();
		}

		return object;
	}

	BaseObject const* BaseObject::getBaseObject() const {
		auto object = static_cast<const BaseObject*>(this);

		if (object->objectType == ObjectType::Reference) {
			object = static_cast<const Reference*>(object)->baseObject;
		}

		if (object == nullptr) {
			return nullptr;
		}

		if (object->isActor() && static_cast<const Actor*>(object)->isClone()) {
			object = static_cast<const Actor*>(object)->getBaseActor();
		}

		return object;
	}

	bool BaseObject::isPhysicalObject() const {
		switch (objectType) {
		case TES3::ObjectType::Activator:
		case TES3::ObjectType::Alchemy:
		case TES3::ObjectType::Ammo:
		case TES3::ObjectType::Apparatus:
		case TES3::ObjectType::Armor:
		case TES3::ObjectType::Bodypart:
		case TES3::ObjectType::Book:
		case TES3::ObjectType::Clothing:
		case TES3::ObjectType::Container:
		case TES3::ObjectType::Creature:
		case TES3::ObjectType::CreatureClone:
		case TES3::ObjectType::Door:
		case TES3::ObjectType::Ingredient:
		case TES3::ObjectType::LeveledCreature:
		case TES3::ObjectType::LeveledItem:
		case TES3::ObjectType::Light:
		case TES3::ObjectType::Lockpick:
		case TES3::ObjectType::Misc:
		case TES3::ObjectType::NPC:
		case TES3::ObjectType::NPCClone:
		case TES3::ObjectType::Probe:
		case TES3::ObjectType::Repair:
		case TES3::ObjectType::Static:
		case TES3::ObjectType::Weapon:
			return true;
		default:
			return false;
		}
	}

	PhysicalObject* BaseObject::asPhysicalObject() {
		if (!isPhysicalObject()) return nullptr;
		return static_cast<PhysicalObject*>(this);
	}

	PhysicalObject const* BaseObject::asPhysicalObject() const {
		if (!isPhysicalObject()) return nullptr;
		return static_cast<const PhysicalObject*>(this);

	}

	bool BaseObject::isActor() const {
		switch (objectType) {
		case TES3::ObjectType::Container:
		case TES3::ObjectType::Creature:
		case TES3::ObjectType::CreatureClone:
		case TES3::ObjectType::NPC:
		case TES3::ObjectType::NPCClone:
			return true;
		default:
			return false;
		}
	}

	bool BaseObject::isItem() const {
		switch (objectType) {
		case TES3::ObjectType::Alchemy:
		case TES3::ObjectType::Ammo:
		case TES3::ObjectType::Apparatus:
		case TES3::ObjectType::Armor:
		case TES3::ObjectType::Book:
		case TES3::ObjectType::Clothing:
		case TES3::ObjectType::Ingredient:
		case TES3::ObjectType::Light:
		case TES3::ObjectType::Lockpick:
		case TES3::ObjectType::Misc:
		case TES3::ObjectType::Probe:
		case TES3::ObjectType::Repair:
		case TES3::ObjectType::Weapon:
			return true;
		default:
			return false;
		}
	}

	bool BaseObject::isWeaponOrAmmo() const {
		switch (objectType) {
		case TES3::ObjectType::Ammo:
		case TES3::ObjectType::Weapon:
			return true;
		default:
			return false;
		}
	}

	bool BaseObject::supportsActivate() const {
		// Make sure we aren't dealing with references.
		auto asBase = getBaseObject();

		if (asBase->isItem()) {
			return static_cast<const Item*>(this)->getIsCarriable();
		}

		if (asBase->objectType == ObjectType::NPC || asBase->objectType == ObjectType::Creature) {
			const auto macp = WorldController::get() ? WorldController::get()->getMobilePlayer() : nullptr;
			if (macp) {
				return macp->getFlagInCombat();
			}

			return true;
		}

		switch (asBase->objectType) {
		case TES3::ObjectType::Activator:
		case TES3::ObjectType::Container:
		case TES3::ObjectType::Door:
			return true;
		default:
			return false;
		}
	}

	bool BaseObject::getLinksResolved() const {
		return BIT_TEST(objectFlags, ObjectFlag::LinksResolvedBit);
	}

	void BaseObject::setLinksResolved(bool value) {
		BIT_SET(objectFlags, ObjectFlag::LinksResolvedBit, value);
	}

	bool BaseObject::getUpdatesCollisionGroups() const {
		const auto baseObject = getBaseObject();

		switch (baseObject->objectType) {
		case ObjectType::Activator:
		case ObjectType::Container:
		case ObjectType::Door:
		case ObjectType::Land:
		case ObjectType::Static:
			return true;
		case ObjectType::Light:
			return !static_cast<const Light*>(baseObject)->getCanCarry();
		}
		return false;
	}

	std::unordered_set<const BaseObject*> sourcelessObjects;

	bool BaseObject::getSourceless() const {
		return isSourcelessObject(this);
	}
	
	void BaseObject::setSourceless(bool sourceless) const {
		if (sourceless) {
			setSourcelessObject(this);
		}
		else {
			sourcelessObjects.erase(this);
		}
	}

	const auto TES3_isSourcelessObject = reinterpret_cast<bool(__stdcall*)(const BaseObject*)>(0x4C1980);
	bool __stdcall BaseObject::isSourcelessObject(const BaseObject* object) {
		return TES3_isSourcelessObject(object) || sourcelessObjects.contains(object);
	}

	void BaseObject::setSourcelessObject(const BaseObject* object) {
		if (!TES3_isSourcelessObject(object)) {
			sourcelessObjects.insert(object);
		}
	}

	bool BaseObject::getSupportsLuaData() const {
		// Gold does all kinds of funky things. No ItemData creation on it is allowed.
		if (objectType == ObjectType::Misc && static_cast<const Misc*>(this)->isGold()) {
			return false;
		}

		// Projectiles cannot have custom data, it breaks the equip interface.
		if (isWeaponOrAmmo()) {
			return !static_cast<const Weapon*>(this)->isProjectile();
		}

		return true;
	}

	std::string BaseObject::toJson() const {
		std::ostringstream ss;
		ss << "\"tes3baseObject:" << getObjectID() << "\"";
		return ss.str();
	}
	
	static std::unordered_map<const BaseObject*, sol::object> baseObjectCache;
	
	bool BaseObject::hasCachedLuaObject() const {
		return baseObjectCache.contains(this);
	}
	
	sol::object BaseObject::getCachedLuaObject() const {
		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		auto cacheHit = baseObjectCache.find(this);
		if (cacheHit != baseObjectCache.end()) {
			auto result = cacheHit->second;
			return result;
		}
		return sol::nil;
	}

	sol::object BaseObject::getOrCreateLuaObject(lua_State* L) const {
		if (this == nullptr) {
			return sol::nil;
		}

		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();

		auto cacheHit = baseObjectCache.find(this);
		if (cacheHit != baseObjectCache.end()) {
			auto result = cacheHit->second;
			return result;
		}

		// Make sure we're looking at the main state.
		L = stateHandle.getState();

		sol::object ref = sol::nil;
		switch ((uint32_t)vTable.object) {
		case TES3::VirtualTableAddress::Activator:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Activator*>(this));
			break;
		case TES3::VirtualTableAddress::Alchemy:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Alchemy*>(this));
			break;
		case TES3::VirtualTableAddress::AnimationGroup:
			ref = sol::make_object_userdata(L, static_cast<const TES3::AnimationGroup*>(this));
			break;
		case TES3::VirtualTableAddress::Apparatus:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Apparatus*>(this));
			break;
		case TES3::VirtualTableAddress::Armor:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Armor*>(this));
			break;
		case TES3::VirtualTableAddress::Birthsign:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Birthsign*>(this));
			break;
		case TES3::VirtualTableAddress::BodyPart:
			ref = sol::make_object_userdata(L, static_cast<const TES3::BodyPart*>(this));
			break;
		case TES3::VirtualTableAddress::Book:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Book*>(this));
			break;
		case TES3::VirtualTableAddress::Cell:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Cell*>(this));
			break;
		case TES3::VirtualTableAddress::Class:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Class*>(this));
			break;
		case TES3::VirtualTableAddress::Clothing:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Clothing*>(this));
			break;
		case TES3::VirtualTableAddress::ContainerBase:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Container*>(this));
			break;
		case TES3::VirtualTableAddress::ContainerInstance:
			ref = sol::make_object_userdata(L, static_cast<const TES3::ContainerInstance*>(this));
			break;
		case TES3::VirtualTableAddress::CreatureBase:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Creature*>(this));
			break;
		case TES3::VirtualTableAddress::CreatureInstance:
			ref = sol::make_object_userdata(L, static_cast<const TES3::CreatureInstance*>(this));
			break;
		case TES3::VirtualTableAddress::Dialogue:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Dialogue*>(this));
			break;
		case TES3::VirtualTableAddress::DialogueInfo:
			ref = sol::make_object_userdata(L, static_cast<const TES3::DialogueInfo*>(this));
			break;
		case TES3::VirtualTableAddress::Door:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Door*>(this));
			break;
		case TES3::VirtualTableAddress::Enchantment:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Enchantment*>(this));
			break;
		case TES3::VirtualTableAddress::Faction:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Faction*>(this));
			break;
		case TES3::VirtualTableAddress::GlobalVariable:
			ref = sol::make_object_userdata(L, static_cast<const TES3::GlobalVariable*>(this));
			break;
		case TES3::VirtualTableAddress::GameSetting:
			ref = sol::make_object_userdata(L, static_cast<const TES3::GameSetting*>(this));
			break;
		case TES3::VirtualTableAddress::Ingredient:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Ingredient*>(this));
			break;
		case TES3::VirtualTableAddress::Land:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Land*>(this));
			break;
		case TES3::VirtualTableAddress::LeveledCreature:
			ref = sol::make_object_userdata(L, static_cast<const TES3::LeveledCreature*>(this));
			break;
		case TES3::VirtualTableAddress::LeveledItem:
			ref = sol::make_object_userdata(L, static_cast<const TES3::LeveledItem*>(this));
			break;
		case TES3::VirtualTableAddress::Light:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Light*>(this));
			break;
		case TES3::VirtualTableAddress::Lockpick:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Lockpick*>(this));
			break;
		case TES3::VirtualTableAddress::MagicEffect:
			ref = sol::make_object_userdata(L, static_cast<const TES3::MagicEffect*>(this));
			break;
		case TES3::VirtualTableAddress::Miscellaneous:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Misc*>(this));
			break;
		case TES3::VirtualTableAddress::NPCBase:
			ref = sol::make_object_userdata(L, static_cast<const TES3::NPC*>(this));
			break;
		case TES3::VirtualTableAddress::NPCInstance:
			ref = sol::make_object_userdata(L, static_cast<const TES3::NPCInstance*>(this));
			break;
		case TES3::VirtualTableAddress::Probe:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Probe*>(this));
			break;
		case TES3::VirtualTableAddress::Quest:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Quest*>(this));
			break;
		case TES3::VirtualTableAddress::Race:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Race*>(this));
			break;
		case TES3::VirtualTableAddress::Reference:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Reference*>(this));
			break;
		case TES3::VirtualTableAddress::Region:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Region*>(this));
			break;
		case TES3::VirtualTableAddress::RepairTool:
			ref = sol::make_object_userdata(L, static_cast<const TES3::RepairTool*>(this));
			break;
		case TES3::VirtualTableAddress::Script:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Script*>(this));
			break;
		case TES3::VirtualTableAddress::Skill:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Skill*>(this));
			break;
		case TES3::VirtualTableAddress::Sound:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Sound*>(this));
			break;
		case TES3::VirtualTableAddress::SoundGenerator:
			ref = sol::make_object_userdata(L, static_cast<const TES3::SoundGenerator*>(this));
			break;
		case TES3::VirtualTableAddress::Spell:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Spell*>(this));
			break;
		case TES3::VirtualTableAddress::MagicSourceInstance:
			ref = sol::make_object_userdata(L, static_cast<const TES3::MagicSourceInstance*>(this));
			break;
		case TES3::VirtualTableAddress::Static:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Static*>(this));
			break;
		case TES3::VirtualTableAddress::Weapon:
			ref = sol::make_object_userdata(L, static_cast<const TES3::Weapon*>(this));
			break;
		}

		if (ref != sol::nil) {
			baseObjectCache[this] = ref;
		}
		else {
			mwse::log::getLog() << "[MWSE] WARNING: An unknown object type was identified with a virtual table address of 0x" << std::hex << (unsigned int)vTable.base << ". Report this to MWSE developers." << std::endl;
			mwse::lua::logStackTrace();
		}

		return ref;
	}

	void BaseObject::clearCachedLuaObject(const BaseObject* object) {
		if (baseObjectCache.empty()) {
			return;
		}
		
		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		// Clear any events that make use of this object.
		auto it = baseObjectCache.find(object);
		if (it != baseObjectCache.end()) {
			// Clear any events that make use of this object.
			mwse::lua::event::clearObjectFilter(it->second);

			// Null the userdata's internal pointer before removing our identity cache.
			mwse::lua::clearUserdataPointer(it->second);

			// Let people know that this object is invalidated.
			stateHandle.triggerEvent(new mwse::lua::event::ObjectInvalidatedEvent(it->second));

			// Remove it from the cache.
			baseObjectCache.erase(it);
		}
		
	}
	
	void BaseObject::clearCachedLuaObjects() {
		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		for (auto& item : baseObjectCache) {
			mwse::lua::clearUserdataPointer(item.second);
		}
		baseObjectCache.clear();
	}


#elif defined(SE_TARGETS_CS) && SE_TARGETS_CS == 1
	void BaseObject::setFlag80(bool set) {
		const auto BaseObject_setFlag80 = reinterpret_cast<void(__thiscall*)(BaseObject*, bool)>(0x4019E7);
		BaseObject_setFlag80(this, set);
	}

	bool BaseObject::search(std::string_view needle, const SearchSettings& settings, std::regex* regex) const {
		if (settings.id && se::string::complex_contains(getObjectID(), needle, settings, regex)) {
			return true;
		}

		return false;
	}

	bool BaseObject::searchWithInheritance(std::string_view needle, const SearchSettings& settings, std::regex* regex) const {
		switch (objectType) {
		case ObjectType::Birthsign:
			return static_cast<const se::cs::Birthsign*>(this)->search(needle, settings, regex);
		case ObjectType::Class:
			return static_cast<const se::cs::Class*>(this)->search(needle, settings, regex);
		case ObjectType::Faction:
			return static_cast<const se::cs::Faction*>(this)->search(needle, settings, regex);
		case ObjectType::Race:
			return static_cast<const se::cs::Race*>(this)->search(needle, settings, regex);
		case ObjectType::Script:
			return static_cast<const se::cs::Script*>(this)->search(needle, settings, regex);
		}

		// Fall back to just an ID search.
		return search(needle, settings, regex);
	}
#endif
}
