#include "TES3Object.h"

#include "TES3Util.h"

#include "TES3Activator.h"
#include "TES3Alchemy.h"
#include "TES3AnimationGroup.h"
#include "TES3Apparatus.h"
#include "TES3Armor.h"
#include "TES3Birthsign.h"
#include "TES3BodyPart.h"
#include "TES3Book.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3Clothing.h"
#include "TES3Container.h"
#include "TES3Creature.h"
#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"
#include "TES3Door.h"
#include "TES3Enchantment.h"
#include "TES3Faction.h"
#include "TES3GameFile.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3Ingredient.h"
#include "TES3Land.h"
#include "TES3LeveledList.h"
#include "TES3Light.h"
#include "TES3Lockpick.h"
#include "TES3MagicEffect.h"
#include "TES3MagicSourceInstance.h"
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
#include "TES3Weapon.h"
#include "TES3WorldController.h"

#include "BitUtil.h"
#include "MemoryUtil.h"

#include "LuaManager.h"
#include "LuaObjectInvalidatedEvent.h"

#include "Log.h"

namespace TES3 {
	void* BaseObject::operator new(size_t size) {
		return mwse::tes3::_new(size);
	}

	void BaseObject::operator delete(void* address) {
		mwse::tes3::_delete(address);
	}

	const auto BaseObject_dtor = reinterpret_cast<TES3::BaseObject * (__thiscall*)(TES3::BaseObject*)>(0x4F0CA0);
	void BaseObject::dtor() {
		clearCachedLuaObject(this);
		BaseObject_dtor(this);
	}

	bool BaseObject::getObjectModified() const {
		return (objectFlags & TES3::ObjectFlag::Modified);
	}

	void BaseObject::setObjectModified(bool modified) {
		vTable.base->setObjectModified(this, modified);
	}

	const char* BaseObject::getObjectID() const {
		return vTable.base->getObjectID(this);
	}

	const auto BaseObject_writeFileHeader = reinterpret_cast<bool(__thiscall*)(const BaseObject*, GameFile*)>(0x4EEE60);
	bool BaseObject::writeFileHeader(GameFile* file) const {
		return BaseObject_writeFileHeader(this, file);
	}

	BaseObject* BaseObject::getBaseObject() const {
		BaseObject* object = const_cast<BaseObject*>(this);

		if (object->objectType == ObjectType::Reference) {
			object = static_cast<Reference*>(object)->baseObject;
		}

		if (object->isActor() && static_cast<Actor*>(object)->isClone()) {
			object = static_cast<Actor*>(object)->getBaseActor();
		}

		return object;
	}

	bool BaseObject::isActor() const {
		switch (objectType) {
		case TES3::ObjectType::Container:
		case TES3::ObjectType::Creature:
		case TES3::ObjectType::NPC:
			return true;
		default:
			return false;
		}
	}

	const char* BaseObject::getSourceFilename() const {
		if (sourceMod) {
			return sourceMod->filename;
		}
		return nullptr;
	}

	bool BaseObject::getDisabled() const {
		return BIT_TEST(objectFlags, TES3::ObjectFlag::DisabledBit);
	}

	bool BaseObject::getDeleted() const {
		return BIT_TEST(objectFlags, TES3::ObjectFlag::DeleteBit);
	}

	bool BaseObject::getPersistent() const {
		return BIT_TEST(objectFlags, TES3::ObjectFlag::PersistentBit);
	}

	void BaseObject::setPersistent(bool value) {
		BIT_SET(objectFlags, TES3::ObjectFlag::PersistentBit, value);
	}

	bool BaseObject::getBlocked() const {
		return BIT_TEST(objectFlags, TES3::ObjectFlag::BlockedBit);
	}

	void BaseObject::setBlocked(bool value) {
		BIT_SET(objectFlags, TES3::ObjectFlag::BlockedBit, value);
	}

	bool BaseObject::getSupportsLuaData() const {
		// Gold does all kinds of funky things. No ItemData creation on it is allowed.
		if (objectType == ObjectType::Misc && static_cast<const Misc*>(this)->isGold()) {
			return false;
		}

		// Projectiles cannot have custom data, it breaks the equip interface.
		if (objectType == ObjectType::Weapon || objectType == ObjectType::Ammo) {
			return !static_cast<const Weapon*>(this)->isProjectile();
		}

		return true;
	}

	std::string BaseObject::toJson() const {
		std::ostringstream ss;
		ss << "\"tes3baseObject:" << getObjectID() << "\"";
		return std::move(ss.str());
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
		return TES3_isSourcelessObject(object) || sourcelessObjects.find(object) != sourcelessObjects.end();
	}

	void BaseObject::setSourcelessObject(const BaseObject* object) {
		if (!TES3_isSourcelessObject(object)) {
			sourcelessObjects.insert(object);
		}
	}

	static std::unordered_map<const BaseObject*, sol::object> baseObjectCache;

	bool BaseObject::hasCachedLuaObject() const {
		return baseObjectCache.find(this) != baseObjectCache.end();
	}

	sol::object BaseObject::getCachedLuaObject() const {
		auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
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

		auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();

		auto cacheHit = baseObjectCache.find(this);
		if (cacheHit != baseObjectCache.end()) {
			auto result = cacheHit->second;
			return result;
		}

		// Make sure we're looking at the main state.
		L = stateHandle.state;

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

		return ref;
	}

	void BaseObject::clearCachedLuaObject(const BaseObject* object) {
		auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		if (!baseObjectCache.empty()) {
			// Clear any events that make use of this object.
			auto it = baseObjectCache.find(object);
			if (it != baseObjectCache.end()) {
				// Let people know that this object is invalidated.
				stateHandle.triggerEvent(new mwse::lua::event::ObjectInvalidatedEvent(it->second));

				// Clear any events that make use of this object.
				mwse::lua::event::clearObjectFilter(it->second);

				// Remove it from the cache.
				baseObjectCache.erase(it);
			}
		}
	}

	void BaseObject::clearCachedLuaObjects() {
		auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		baseObjectCache.clear();
	}

	void Object::copy(const Object* from, int unknown) {
		vTable.object->copy(this, from, unknown);
	}

	void Object::setID(const char* id) {
		vTable.object->setID(this, id);
	}

	char* Object::getName() const {
		return vTable.object->getName(this);
	}

	char* Object::getIconPath() const {
		return vTable.object->getIconPath(this);
	}

	char* Object::getModelPath() const {
		return vTable.object->getModelPath(this);
	}

	Script* Object::getScript() const {
		return vTable.object->getScript(this);
	}

	Sound* Object::getSound() const {
		return vTable.object->getSound(this);
	}

	char* Object::getRaceID() const {
		return vTable.object->getRaceID(this);
	}

	char* Object::getClassID() const {
		return vTable.object->getClassID(this);
	}

	char* Object::getBirthsignID() const {
		return vTable.object->getBirthsignID(this);
	}

	Race* Object::getRace() const {
		return vTable.object->getRace(this);
	}

	Class* Object::getClass() const {
		return vTable.object->getClass(this);
	}

	Faction* Object::getFaction() const {
		return vTable.object->getFaction(this);
	}

	bool Object::isFemale() const {
		return vTable.object->isFemale(this);
	}

	int Object::getFactionRank() const {
		return vTable.object->getFactionRank(this);
	}

	int Object::getLevel() const {
		return vTable.object->getLevel(this);
	}

	void Object::setDispositionRaw(signed char value) {
		return vTable.object->setDispositionRaw(this, value);
	}

	int Object::modDisposition(signed int value) {
		return vTable.object->modDisposition(this, value);
	}

	int Object::getReputation() const {
		return vTable.object->getReputation(this);
	}

	int Object::setReputation(int reputation) {
		return vTable.object->setReputation(this, reputation);
	}

	int Object::getDispositionRaw() const {
		return vTable.object->getDispositionRaw(this);
	}

	signed char Object::modFactionIndex(signed char value) {
		return vTable.object->modFactionIndex(this, value);
	}

	int Object::getType() const {
		return vTable.object->getType(this);
	}

	char* Object::getTypeName() const {
		return vTable.object->getTypeName(this);
	}

	float Object::getWeight() const {
		return vTable.object->getWeight(this);
	}

	int Object::getValue() const {
		return vTable.object->getValue(this);
	}

	void Object::setDurability(int value) {
		return vTable.object->setDurability(this, value);
	}

	int Object::getDurability() const {
		return vTable.object->getDurability(this);
	}

	int Object::getMagicka() const {
		return vTable.object->getMagicka(this);
	}

	int Object::getFatigue() const {
		return vTable.object->getFatigue(this);
	}

	float Object::getQuality() const {
		return vTable.object->getQuality(this);
	}

	bool Object::isLeftPartOfPair() const {
		return vTable.object->isLeftPartOfPair(this);
	}

	bool Object::isEssential() const {
		return vTable.object->isEssential(this);
	}

	bool Object::isRespawn() const {
		return vTable.object->isRespawn(this);
	}

	int Object::getUses() const {
		return vTable.object->getUses(this);
	}

	Enchantment* Object::getEnchantment() const {
		return vTable.object->getEnchantment(this);
	}

	Enchantment* Object::setEnchantment(Enchantment* enchantment) {
		return vTable.object->setEnchantment(this, enchantment);
	}

	AIConfig* Object::getAIConfig() const {
		return vTable.object->getAIConfig(this);
	}

	bool Object::getAutoCalc() const {
		return vTable.object->getAutoCalc(this);
	}

	void Object::setAutoCalc(bool value) {
		vTable.object->setAutoCalc(this, value);
	}

	void Object::setModelPath(const char* path) {
		if (strnlen_s(path, 32) >= 32) {
			throw std::invalid_argument("Path cannot be more than 31 characters in length.");
		}

		vTable.object->setModelPath(this, path);
	}

	void Object::setName(const char* name) {
		if (strnlen_s(name, 32) >= 32) {
			throw std::invalid_argument("Name cannot be more than 31 characters in length.");
		}
		vTable.object->setName(this, name);
	}

	void Object::reevaluateEquipment() {
		vTable.object->reevaluateEquipment(this);
	}

	void Object::resetVisualNode(NI::Node* node) {
		vTable.object->resetVisualNode(this, node);
	}

	float Object::getScale() const {
		return vTable.object->getScale(this);
	}

	void Object::setScale(float value, bool cap) {
		vTable.object->setScale(this, value, cap);
	}

	bool Object::getIsLocationMarker() const {
		return vTable.object->isLocationMarker(this);
	}

	NI::Node * Object::getSceneGraphNode() {
		return vTable.object->getSceneGraphNode(this);
	}

	Object * Object::skipDeletedObjects() {
		TES3::Object * object = this;
		while (object && (object->objectFlags & TES3::ObjectFlag::Delete) == TES3::ObjectFlag::Delete)
		{
			object = object->nextInCollection;
		}
		return object;
	}

	ReferenceList* Object::getOwningCollection() {
		return owningCollection.asReferenceList;
	}

	void Object::setScale_lua(float scale) {
		setScale(scale);
	}

	//
	// PhysicalObject
	//

	IteratedList<BaseObject*> * PhysicalObject::getStolenList() {
		return vTable.physical->getStolenList(this);
	}

	const auto TES3_PhysicalObject_addStolenFlag = reinterpret_cast<void(__thiscall*)(PhysicalObject*, BaseObject*)>(0x4EFEE0);
	void PhysicalObject::addStolenFlag(BaseObject* stolenFrom) {
		TES3_PhysicalObject_addStolenFlag(this, stolenFrom);
	}

	const auto TES3_PhysicalObject_removeStolenFlag = reinterpret_cast<void(__thiscall*)(PhysicalObject*, BaseObject*)>(0x4EFF10);
	void PhysicalObject::removeStolenFlag(BaseObject* stolenFrom) {
		TES3_PhysicalObject_removeStolenFlag(this, stolenFrom);
	}

	const auto TES3_PhysicalObject_getStolenFlag = reinterpret_cast<bool(__thiscall*)(PhysicalObject*, BaseObject*)>(0x4EFEB0);
	bool PhysicalObject::getStolenFlag(BaseObject* stolenFrom) {
		return TES3_PhysicalObject_getStolenFlag(this, stolenFrom);
	}

	const auto TES3_PhysicalObject_getMobile = reinterpret_cast<MobileObject*(__thiscall*)(const PhysicalObject*)>(0x494E70);
	MobileObject* PhysicalObject::getMobile() const {
		return TES3_PhysicalObject_getMobile(this);
	}

	const auto TES3_PhysicalObject_createBoundingBox = reinterpret_cast<void(__thiscall*)(PhysicalObject*)>(0x4EEFC0);
	void PhysicalObject::createBoundingBox() {
		TES3_PhysicalObject_createBoundingBox(this);
	}

	BoundingBox* PhysicalObject::getOrCreateBoundingBox() {
		if (!boundingBox) {
			createBoundingBox();
		}
		return boundingBox;
	}

	Reference* PhysicalObject::getReference() const {
		if (auto thisRef = reinterpret_cast<Reference*>(referenceToThis); thisRef && thisRef->objectType == ObjectType::Reference) {
			return thisRef;
		}
		else {
			auto mobile = getMobile();
			if (mobile) {
				return mobile->reference;
			}
		}

		return nullptr;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::BaseObject)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Object)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::PhysicalObject)
