#include "CSBaseObject.h"

#include "CSBirthsign.h"
#include "CSClass.h"
#include "CSFaction.h"
#include "CSScript.h"
#include "CSRace.h"

#include "BitUtil.h"
#include "StringUtil.h"

namespace se::cs {
	const char* BaseObject::getObjectID() const {
		return vtbl.baseObject->getObjectID(this);
	}

	bool BaseObject::isFromMaster() const {
		return BIT_TEST(flags, TES3::ObjectFlag::FromMasterBit);
	}

	bool BaseObject::getModified() const {
		return BIT_TEST(flags, TES3::ObjectFlag::ModifiedBit);
	}

	void BaseObject::setModified(bool modified) {
		vtbl.baseObject->setObjectModified(this, modified);
	}

	bool BaseObject::getDeleted() const {
		return BIT_TEST(flags, TES3::ObjectFlag::DeleteBit);
	}

	void BaseObject::setDeleted(bool deleted) {
		const auto BaseObject_setDeleted = reinterpret_cast<void(__thiscall*)(BaseObject*, bool)>(0x547810);
		BaseObject_setDeleted(this, deleted);
	}

	bool BaseObject::getPersists() const {
		return BIT_TEST(flags, TES3::ObjectFlag::PersistentBit);
	}

	bool BaseObject::getBlocked() const {
		return BIT_TEST(flags, TES3::ObjectFlag::BlockedBit);
	}

	bool BaseObject::getScaleModifiedToOne() const {
		return BIT_TEST(flags, TES3::ObjectFlag::ScaleModifiedToOneBit);
	}

	void BaseObject::setScaleModifiedToOne(bool value) {
		BIT_SET(flags, TES3::ObjectFlag::ScaleModifiedToOneBit, value);
	}

	bool BaseObject::isMobileCapableActor() const {
		switch (objectType) {
		case TES3::ObjectType::Creature:
		case TES3::ObjectType::CreatureClone:
		case TES3::ObjectType::NPC:
		case TES3::ObjectType::NPCClone:
			return true;
		default:
			return false;
		}
	}

	void BaseObject::setFlag80(bool set) {
		const auto BaseObject_setFlag80 = reinterpret_cast<void(__thiscall*)(BaseObject*, bool)>(0x4019E7);
		BaseObject_setFlag80(this, set);
	}

	bool BaseObject::search(std::string_view needle, const SearchSettings& settings, std::regex* regex) const {
		if (settings.id && string::complex_contains(getObjectID(), needle, settings, regex)) {
			return true;
		}

		return false;
	}

	bool BaseObject::searchWithInheritance(std::string_view needle, const SearchSettings& settings, std::regex* regex) const {
		switch (objectType) {
		case TES3::ObjectType::Birthsign:
			return static_cast<const Birthsign*>(this)->search(needle, settings, regex);
		case TES3::ObjectType::Class:
			return static_cast<const Class*>(this)->search(needle, settings, regex);
		case TES3::ObjectType::Faction:
			return static_cast<const Faction*>(this)->search(needle, settings, regex);
		case TES3::ObjectType::Script:
			return static_cast<const Script*>(this)->search(needle, settings, regex);
		case TES3::ObjectType::Race:
			return static_cast<const Race*>(this)->search(needle, settings, regex);
		}

		// Fall back to just an ID search.
		return search(needle, settings, regex);
	}
}
