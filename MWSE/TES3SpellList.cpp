#include "TES3SpellList.h"

#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Spell.h"

#define TES3_SpellList_addByObject 0x4AC880
#define TES3_SpellList_addById 0x4AC750
#define TES3_SpellList_removeByObject 0x4ACBE0
#define TES3_SpellList_containsObject 0x4ACB50
#define TES3_SpellList_getCheapest 0x4ACB90

namespace TES3 {
	bool SpellList::add(Spell* spell) {
		return reinterpret_cast<bool(__thiscall*)(SpellList*, Spell*)>(TES3_SpellList_addByObject)(this, spell);
	}

	bool SpellList::add(const char* id) {
		return reinterpret_cast<bool(__thiscall*)(SpellList*, const char*)>(TES3_SpellList_addById)(this, id);
	}

	bool SpellList::add(const std::string& id) {
		return add(id.c_str());
	}

	bool SpellList::remove(Spell* spell) {
		int previousSize = list.size();
		reinterpret_cast<void(__thiscall*)(SpellList*, Spell*)>(TES3_SpellList_removeByObject)(this, spell);
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

	bool SpellList::remove(const std::string& id) {
		return remove(id.c_str());
	}

	bool SpellList::contains(Spell* spell) {
		return reinterpret_cast<bool(__thiscall*)(SpellList*, Spell*)>(TES3_SpellList_containsObject)(this, spell);
	}

	bool SpellList::contains(const char* id) {
		Spell* spell = TES3::DataHandler::get()->nonDynamicData->getSpellById(id);
		if (spell == nullptr) {
			return false;
		}

		return contains(spell);
	}

	bool SpellList::contains(const std::string& id) {
		return contains(id.c_str());
	}

	Spell* SpellList::getCheapest() {
		return reinterpret_cast<Spell*(__thiscall*)(SpellList*)>(TES3_SpellList_getCheapest)(this);
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
