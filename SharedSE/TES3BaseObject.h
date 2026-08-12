#pragma once

#include "TES3Defines.h"

#if defined(SE_TARGETS_CS) && SE_TARGETS_CS == 1
#include "CSDefines.h"
#endif

namespace TES3 {
	
	//
	// Enums
	//

	namespace ObjectFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			FromMaster = 0x1,
			Modified = 0x2,
			LinksResolved = 0x8,
			NoCollision = 0x10,
			Delete = 0x20,
			Persistent = 0x400,
			Disabled = 0x800,
			SelectedByConsole = 0x1000,
			Blocked = 0x2000,
			EmptyInventory = 0x2000,
			ScaleModifiedToOne = 0x8000,
		};

		enum FlagBit {
			FromMasterBit = 0,
			ModifiedBit = 1,
			LinksResolvedBit = 3,
			NoCollisionBit = 4,
			DeleteBit = 5,
			PersistentBit = 10,
			DisabledBit = 11,
			SelectedByConsoleBit = 12,
			BlockedBit = 13,
			EmptyInventoryBit = 13,
			ScaleModifiedToOneBit = 15,
		};
	}

	//
	// The core building blocks of TES3 objects.
	//

	struct BaseObjectVirtualTable {
		void (__thiscall* deleting_dtor)(BaseObject*, char); // 0x0
		bool (__thiscall* loadObjectSpecific)(BaseObject*, GameFile*); // 0x4
		bool (__thiscall* saveRecordSpecific)(BaseObject*, GameFile*); // 0x8
		bool (__thiscall* loadObject)(BaseObject*, GameFile*); // 0xC
		bool (__thiscall* saveObject)(BaseObject*, GameFile*); // 0x10
		void (__thiscall* setModified)(BaseObject*, bool); // 0x14
		void (__thiscall* setFlagMovedRef)(BaseObject*, bool); // 0x18
		int (__thiscall* getCount)(const BaseObject*); // 0x1C
		const char* (__thiscall* getObjectID)(const BaseObject*); // 0x20
	};
	static_assert(sizeof(BaseObjectVirtualTable) == 0x24, "TES3::BaseObjectVirtualTable failed size validation");

	struct BaseObject {
		union {
			BaseObjectVirtualTable* base;
			ObjectVirtualTable* object;
			PhysicalObjectVirtualTable* physical;
			ActorVirtualTable* actor;
		} vTable; // 0x0
		ObjectType::ObjectType objectType; // 0x4
		unsigned int objectFlags; // 0x8
		GameFile* sourceFile; // 0xC

		static constexpr auto OBJECT_TYPE = ObjectType::Invalid;

		//
		// Function wrappers for our virtual table.
		//

		const char* getObjectID() const;
		const char* getSourceFilename() const;
		bool isFromMaster() const;
		bool getModified() const;
		void setModified(bool modified);
		bool getDisabled() const;
		bool getDeleted() const;
		void setDeleted(bool deleted);
		bool getPersistent() const;
		void setPersistent(bool value);
		bool getBlocked() const;
		void setBlocked(bool value);
		bool getScaleModifiedToOne() const;
		void setScaleModifiedToOne(bool value);

		bool isMobileCapableActor() const;

#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1

		//
		// Basic operators.
		//

		static void* operator new(size_t size);
		static void operator delete(void* adress);

		// The address of a destructor can't be taken, so we need to put all the logic here.
		void dtor();

		//
		// Other related this-call functions.
		//

		bool writeFileHeader(GameFile* file) const;

		//
		// Custom functions.
		//

		BaseObject* getBaseObject();
		BaseObject const* getBaseObject() const;
		bool isPhysicalObject() const;
		PhysicalObject* asPhysicalObject();
		PhysicalObject const* asPhysicalObject() const;
		bool isActor() const;
		bool isItem() const;
		bool isWeaponOrAmmo() const;
		bool supportsActivate() const;
		
		bool getLinksResolved() const;
		void setLinksResolved(bool value);
		
		bool getUpdatesCollisionGroups() const;
		bool getSourceless() const;
		void setSourceless(bool sourceless) const;

		static bool __stdcall isSourcelessObject(const BaseObject* object);
		static void setSourcelessObject(const BaseObject* object);

		bool getSupportsLuaData() const;
		
		std::string toJson() const;

		// Storage for cached userdata.
		bool hasCachedLuaObject() const;
		sol::object getCachedLuaObject() const;
		sol::object getOrCreateLuaObject(lua_State* L) const;
		static void clearCachedLuaObject(const BaseObject* object);
		static void clearCachedLuaObjects();

#elif defined(SE_TARGETS_CS) && SE_TARGETS_CS == 1
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
			bool training = true;
		};

		bool search(std::string_view needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
		bool searchWithInheritance(std::string_view needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
#endif
	};
	static_assert(sizeof(BaseObject) == 0x10, "TES3::BaseObject failed size validation");
}
