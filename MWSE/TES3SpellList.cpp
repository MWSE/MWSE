#include "TES3SpellList.h"

#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Spell.h"

namespace TES3 {
	const auto TES3_SpellList_addByObject = reinterpret_cast<bool(__thiscall*)(SpellList*, Spell*)>(0x4AC880);
	bool SpellList::add(Spell* spell) {
		return TES3_SpellList_addByObject(this, spell);
	}

	const auto TES3_SpellList_addById = reinterpret_cast<bool(__thiscall*)(SpellList*, const char*)>(0x4AC750);
	bool SpellList::add(const char* id) {
		return TES3_SpellList_addById(this, id);
	}

	bool SpellList::add(std::string_view id) {
		return add(id.data());
	}

	const auto TES3_SpellList_removeByObject = reinterpret_cast<void(__thiscall*)(SpellList*, Spell*)>(0x4ACBE0);
	bool SpellList::remove(Spell* spell) {
		size_t previousSize = list.size();
		TES3_SpellList_removeByObject(this, spell);
		return list.size() < previousSize;
	}

	bool SpellList::remove(const char* id) {
		for (auto itt = list.head; itt != NULL; itt = itt->next) {
			const char* thisId = itt->data->getObjectID();
			if (_strcmpi(thisId, id) == 0) {
				return remove(itt->data);
			}
		}
		return false;
	}

	bool SpellList::remove(std::string_view id) {
		return remove(id.data());
	}

	const auto TES3_SpellList_containsObject = reinterpret_cast<bool(__thiscall*)(SpellList*, Spell*)>(0x4ACB50);
	bool SpellList::contains(Spell* spell) {
		return TES3_SpellList_containsObject(this, spell);
	}

	bool SpellList::contains(const char* id) {
		Spell* spell = TES3::DataHandler::get()->nonDynamicData->getSpellById(id);
		if (spell == nullptr) {
			return false;
		}

		return contains(spell);
	}

	bool SpellList::contains(std::string_view id) {
		return contains(id.data());
	}

	const auto TES3_SpellList_getCheapest = reinterpret_cast<Spell * (__thiscall*)(SpellList*)>(0x4ACB90);
	Spell* SpellList::getCheapest() {
		return TES3_SpellList_getCheapest(this);
	}

	bool SpellList::add_lua(sol::object value) {
		if (value.is<TES3::Spell*>()) {
			return add(value.as<TES3::Spell*>());
		}
		else if (value.is<const char*>()) {
			return add(value.as<const char*>());
		}
		return false;
	}

	bool SpellList::remove_lua(sol::object value) {
		if (value.is<TES3::Spell*>()) {
			return remove(value.as<TES3::Spell*>());
		}
		else if (value.is<const char*>()) {
			return remove(value.as<const char*>());
		}
		return false;
	}

	bool SpellList::contains_lua(sol::object value) {
		if (value.is<TES3::Spell*>()) {
			return contains(value.as<TES3::Spell*>());
		}
		else if (value.is<const char*>()) {
			return contains(value.as<const char*>());
		}
		return false;
	}

	bool SpellList::containsType(SpellCastType::value_type type) {
		for (auto itt = list.head; itt != NULL; itt = itt->next) {
			if (itt->data->castType == type) {
				return true;
			}
		}
		return false;
	}
}
