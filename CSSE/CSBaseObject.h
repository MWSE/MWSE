#pragma once

#include "CSDefines.h"

namespace se::cs {
	struct BaseObject_VirtualTable {
		void(__thiscall* destructor)(BaseObject*, signed char); // 0x0
		int(__thiscall* loadObjectSpecific)(BaseObject*, GameFile*); // 0x4
		int(__thiscall* saveRecordSpecific)(BaseObject*, GameFile*); // 0x8
		int(__thiscall* loadObject)(BaseObject*, GameFile*); // 0xC
		int(__thiscall* saveObject)(BaseObject*, GameFile*); // 0x10
		void(__thiscall* setObjectModified)(BaseObject*, bool); // 0x14
		int(__thiscall* setObjectFlag40)(BaseObject*, bool); // 0x18
		int(__thiscall* getCount)(const BaseObject*); // 0x1C
		const char* (__thiscall* getObjectID)(const BaseObject*); // 0x20
	};

	struct BaseObject {
		union {
			BaseObject_VirtualTable* baseObject;
			Object_VirtualTable* object;
			Actor_VirtualTable* actor;
		} vtbl; // 0x0
		ObjectType::ObjectType objectType; // 0x4
		unsigned int flags; // 0x8
		GameFile* sourceFile; // 0xC

		const char* getObjectID() const;

		bool isFromMaster() const;
		bool getModified() const;
		void setModified(bool modified);
		bool getDeleted() const;
		bool getPersists() const;
		bool getBlocked() const;

		void setFlag80(bool set);

		struct SearchSettings {
			bool use_regex = false;
			bool case_sensitive = false;
			bool id = true;
			bool name = true;
			bool icon_path = true;
			bool model_path = true;
			bool enchantment_id = true;
			bool script_id = true;
			bool book_text = true;
			bool faction = true;
			bool effect = true;
		};

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
		bool searchWithInheritance(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(BaseObject) == 0x10, "TES3::BaseObject failed size validation");
	static_assert(sizeof(BaseObject_VirtualTable) == 0x24, "TES3::BaseObject_VirtualTable failed size validation");
}
