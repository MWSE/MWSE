#pragma once

#include "NIDefines.h"
#include "TES3Defines.h"

#include "TES3IteratedList.h"
#include "TES3LinkedObjectsList.h"

#include "NINode.h"

// Must be added to header files that declare Ni types that can be derived.
#define MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(T) \
int sol_lua_push(sol::types<T>, lua_State* L, const T& obj); \
int sol_lua_push(sol::types<T*>, lua_State* L, const T* obj);

// Must be added to source files that declare Ni types that can be derived.
#define MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(T) \
int sol_lua_push(sol::types<T>, lua_State* L, const T& obj) { return obj.getOrCreateLuaObject(L).push(L); } \
int sol_lua_push(sol::types<T*>, lua_State* L, const T* obj) { return obj->getOrCreateLuaObject(L).push(L); }

namespace TES3 {

	//
	// Object types. They are char[4], or can be interpreted as a 32-bit integer.
	//

	namespace ObjectType {
		enum ObjectType {
			Invalid = 0,
			Activator = 'ITCA',
			Alchemy = 'HCLA',
			Ammo = 'OMMA',
			AnimationGroup = 'GINA',
			Apparatus = 'APPA',
			Armor = 'OMRA',
			Birthsign = 'NGSB',
			Bodypart = 'YDOB',
			Book = 'KOOB',
			Cell = 'LLEC',
			Class = 'SALC',
			Clothing = 'TOLC',
			Container = 'TNOC',
			Creature = 'AERC',
			CreatureClone = 'CERC',
			Dialogue = 'LAID',
			DialogueInfo = 'OFNI',
			Door = 'ROOD',
			Enchantment = 'HCNE',
			Faction = 'TCAF',
			GameSetting = 'TSMG',
			Global = 'BOLG',
			Ingredient = 'RGNI',
			Land = 'DNAL',
			LandTexture = 'XETL',
			LeveledCreature = 'CVEL',
			LeveledItem = 'IVEL',
			Light = 'HGIL',
			Lockpick = 'KCOL',
			MagicEffect = 'FEGM',
			MagicSourceInstance = 'LLPS',
			Misc = 'CSIM',
			MobileCreature = 'RCAM',
			MobileNPC = 'HCAM',
			MobileObject = 'TCAM',
			MobilePlayer = 'PCAM',
			MobileProjectile = 'JRPM',
			MobileSpellProjectile = 'PSPM',
			NPC = '_CPN',
			NPCClone = 'CCPN',
			PathGrid = 'DRGP',
			Probe = 'BORP',
			Quest = 'SEUQ',
			Race = 'ECAR',
			Reference = 'RFER',
			Region = 'NGER',
			Repair = 'APER',
			Script = 'TPCS',
			Skill = 'LIKS',
			Sound = 'NUOS',
			SoundGenerator = 'GDNS',
			Spell = 'LEPS',
			Static = 'TATS',
			TES3 = '3SET',
			Weapon = 'PAEW',
		};
	}

	//
	// Enums
	//

	namespace ObjectFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			Modified = 0x2,
			LinksResolved = 0x8,
			NoCollision = 0x10,
			Delete = 0x20,
			Persistent = 0x400,
			Disabled = 0x800,
			SelectedByConsole = 0x1000,
			Blocked = 0x2000,
			EmptyInventory = 0x2000,
		};

		enum FlagBit {
			ModifiedBit = 1,
			LinksResolvedBit = 3,
			NoCollisionBit = 4,
			DeleteBit = 5,
			PersistentBit = 10,
			DisabledBit = 11,
			SelectedByConsoleBit = 12,
			BlockedBit = 13,
			EmptyInventoryBit = 13,
		};
	}

	//
	// The core building blocks of TES3 objects.
	//

	struct BaseObjectVirtualTable {
		void (__thiscall * destructor)(BaseObject*, signed char); // 0x0
		int (__thiscall * loadObjectSpecific)(BaseObject*, GameFile*); // 0x4
		int (__thiscall * saveRecordSpecific)(BaseObject*, GameFile*); // 0x8
		int (__thiscall * loadObject)(BaseObject*, GameFile*); // 0xC
		int (__thiscall * saveObject)(BaseObject*, GameFile*); // 0x10
		void (__thiscall * setObjectModified)(BaseObject*, bool); // 0x14
		int (__thiscall * setObjectFlag40)(BaseObject*, unsigned char); // 0x18
		void * unknown_0x1C;
		const char * (__thiscall * getObjectID)(const BaseObject*); // 0x20
	};
	static_assert(sizeof(BaseObjectVirtualTable) == 0x24, "TES3::BaseObjectVirtualTable failed size validation");

	struct ObjectVirtualTable : BaseObjectVirtualTable {
		void (__thiscall * copy)(Object*, const Object*, int); // 0x24
		void (__thiscall * setID)(BaseObject*, const char*); // 0x28
		NI::Node * (__thiscall * getSceneGraphNode)(BaseObject*); // 0x2C
		void * unknown_0x30;
		void * unknown_0x34;
		char * (__thiscall * getName)(const BaseObject*); // 0x38
		char * (__thiscall * getIconPath)(const BaseObject*); // 0x3C
		void * unknown_0x40;
		void * unknown_0x44;
		char* (__thiscall * getModelPath)(const BaseObject*); // 0x48
		Script * (__thiscall * getScript)(const BaseObject*); // 0x4C
		Sound * (__thiscall * getSound)(const BaseObject*); // 0x50
		char * (__thiscall * getRaceID)(const BaseObject*); // 0x54
		char * (__thiscall * getClassID)(const BaseObject*); // 0x58
		char * (__thiscall * getBirthsignID)(const BaseObject*); // 0x5C
		Race * (__thiscall * getRace)(const BaseObject*); // 0x60
		Class * (__thiscall * getClass)(const BaseObject*); // 0x64
		Faction * (__thiscall * getFaction)(const BaseObject*); // 0x68
		bool (__thiscall * isFemale)(const BaseObject*); // 0x6C
		int (__thiscall * getFactionRank)(const BaseObject*); // 0x70
		int (__thiscall * getLevel)(const BaseObject*); // 0x74
		void * unknown_0x78;
		void (__thiscall * setDispositionRaw)(BaseObject*, int); // 0x7C
		int (__thiscall * modDisposition)(BaseObject*, signed int); // 0x80
		int (__thiscall * getReputation)(const BaseObject*); // 0x84
		int (__thiscall * setReputation)(BaseObject*, int); // 0x88
		int (__thiscall * getDispositionRaw)(const BaseObject*); // 0x8C
		signed char(__thiscall * modFactionIndex)(BaseObject*, signed char); // 0x90
		int (__thiscall * getType)(const BaseObject*); // 0x94
		char * (__thiscall * getTypeName)(const BaseObject*); // 0x98
		float (__thiscall * getWeight)(const BaseObject*); // 0x9C
		int (__thiscall * getValue)(const BaseObject*); // 0xA0
		void (__thiscall * setDurability)(BaseObject*, int); // 0xA4
		int (__thiscall * getDurability)(const BaseObject*); // 0xA8
		int (__thiscall * getMagicka)(const BaseObject*); // 0xAC
		int (__thiscall * getFatigue)(const BaseObject*); // 0xB0
		float (__thiscall * getQuality)(const BaseObject*); // 0xB4
		bool (__thiscall * isLeftPartOfPair)(const BaseObject*); // 0xB8
		bool (__thiscall * isEssential)(const BaseObject*); // 0xBC
		bool (__thiscall * isRespawn)(const BaseObject*); // 0xC0
		void * unknown_0xC4;
		int (__thiscall * getUses)(const BaseObject*); // 0xC8
		int (__thiscall * getEnchantCapacity)(const BaseObject*);  // 0xCC
		Enchantment * (__thiscall * getEnchantment)(const BaseObject*); // 0xD0
		Enchantment * (__thiscall * setEnchantment)(BaseObject*, Enchantment*); // 0xD4
		AIConfig * (__thiscall * getAIConfig)(const BaseObject*); // 0xD8
		void * (__thiscall * getAIPackageList)(const BaseObject*); // 0xDC
		void * resolveInternalIDs; // 0xE0
		void * unknown_0xE4;
		unsigned char (__thiscall * getAutoCalc)(const BaseObject*); // 0xE8
		void (__thiscall * setAutoCalc)(BaseObject*, unsigned char); // 0xEC
		void * unknown_0xF0;
		void * unknown_0xF4;
		void * unknown_0xF8;
		char* (__thiscall * setModelPath)(BaseObject*, const char*); // 0xFC
		void * unknown_0x100;
		void * unknown_0x104;
		bool (__thiscall* isLocationMarker)(const BaseObject*); // 0x108
		char* (__thiscall * setName)(BaseObject*, const char*); // 0x10C
		void (__thiscall* reevaluateEquipment)(BaseObject*); // 0x110
		void * unknown_0x114;
		void * unknown_0x118;
		void(__thiscall* resetVisualNode)(BaseObject*, NI::Node*); // 0x11C
		float (__thiscall * getScale)(const BaseObject*); // 0x120
		void (__thiscall * setScale)(BaseObject*, float, bool); // 0x124
		void * unknown_0x128;
		int(__thiscall* unknown_0x12C)(BaseObject*); // 0x12C
		void * unknown_0x130;
		void * loadModel; // 0x134
		void * unknown_0x138;
	};
	static_assert(sizeof(ObjectVirtualTable) == 0x13C, "TES3::ObjectVirtualTable failed size validation");

	struct BaseObject {
		union {
			BaseObjectVirtualTable * base;
			ObjectVirtualTable * object;
			PhysicalObjectVirtualTable * physical;
			ActorVirtualTable * actor;
		} vTable; // 0x0
		ObjectType::ObjectType objectType; // 0x4
		unsigned int objectFlags; // 0x8
		GameFile * sourceMod; // 0xC

		//
		// Basic operators.
		//

		static void *operator new(size_t size);
		static void operator delete(void *block);

		// The address of a destructor can't be taken, so we need to put all the logic here.
		void dtor();

		//
		// Function wrappers for our virtual table.
		//

		bool getObjectModified() const;
		void setObjectModified(bool);
		const char* getObjectID() const;

		//
		// Other related this-call functions.
		//

		bool writeFileHeader(GameFile* file) const;

		//
		// Custom functions.
		//

		BaseObject * getBaseObject() const;

		bool isActor() const;
		const char* getSourceFilename() const;

		bool getDisabled() const;
		bool getDeleted() const;

		bool getPersistent() const;
		void setPersistent(bool value);

		bool getBlocked() const;
		void setBlocked(bool value);

		bool getSupportsLuaData() const;

		std::string toJson() const;

		bool getSourceless() const;
		void setSourceless(bool sourceless) const;

		static bool __stdcall isSourcelessObject(const BaseObject* object);
		static void setSourcelessObject(const BaseObject* object);

		// Storage for cached userdata.
		bool hasCachedLuaObject() const;
		sol::object getCachedLuaObject() const;
		sol::object getOrCreateLuaObject(lua_State* L) const;
		static void clearCachedLuaObject(const BaseObject* object);
		static void clearCachedLuaObjects();

	};
	static_assert(sizeof(BaseObject) == 0x10, "TES3::BaseObject failed size validation");

	struct Object : BaseObject {
		NI::Node * sceneNode; // 0x10
		union {
			LinkedObjectList<Spell> * asSpellList;
			ReferenceList * asReferenceList;
		} owningCollection; // 0x14
		void * referenceToThis; // 0x18
		Object * previousInCollection; // 0x1C
		Object * nextInCollection; // 0x20
		NI::Pointer<NI::Node> sceneCollisionRoot; // 0x24

		//
		// Function wrappers for our virtual table.
		//

		void copy(const Object* from, int unknown = 0);
		void setID(const char*);
		char * getName() const;
		char * getIconPath() const;
		char * getModelPath() const;
		Script * getScript() const;
		Sound * getSound() const;
		char * getRaceID() const;
		char * getClassID() const;
		char * getBirthsignID() const;
		Race * getRace() const;
		Class * getClass() const;
		Faction * getFaction() const;
		bool isFemale() const;
		int getFactionRank() const;
		int getLevel() const;
		void setDispositionRaw(signed char);
		int modDisposition(signed int);
		int getReputation() const;
		int setReputation(int);
		int getDispositionRaw() const;
		signed char modFactionIndex(signed char);
		int getType() const;
		char * getTypeName() const;
		float getWeight() const;
		int getValue() const;
		void setDurability(int);
		int getDurability() const;
		int getMagicka() const;
		int getFatigue() const;
		float getQuality() const;
		bool isLeftPartOfPair() const;
		bool isEssential() const;
		bool isRespawn() const;
		int getUses() const;
		Enchantment * getEnchantment() const;
		Enchantment * setEnchantment(Enchantment*);
		AIConfig * getAIConfig() const;
		bool getAutoCalc() const;
		void setAutoCalc(bool);
		void setModelPath(const char*);
		void setName(const char*);
		void reevaluateEquipment();
		void resetVisualNode(NI::Node* node = nullptr);
		float getScale() const;
		void setScale(float value, bool cap = false);

		bool getIsLocationMarker() const;

		NI::Node * getSceneGraphNode();

		//
		// Custom functions.
		//

		Object * skipDeletedObjects();
		ReferenceList* getOwningCollection();

		void setScale_lua(float scale);

	};
	static_assert(sizeof(Object) == 0x28, "TES3::Object failed size validation");

	struct PhysicalObjectVirtualTable : ObjectVirtualTable {
		void * unknown_0x13C;
		void * unknown_0x140;
		IteratedList<BaseObject*> * (__thiscall * getStolenList)(PhysicalObject*); // 0x144
	};

	struct PhysicalObject : Object {
		BoundingBox * boundingBox; // 0x28
		char * objectID; // 0x2C

		//
		// Function wrappers for our virtual table.
		//

		IteratedList<BaseObject*>* getStolenList();

		//
		// Other related this-call functions.
		//

		void addStolenFlag(BaseObject* stolenFrom);
		void removeStolenFlag(BaseObject* stolenFrom);
		bool getStolenFlag(BaseObject* stolenFrom);

		MobileObject* getMobile() const;

		void createBoundingBox();

		//
		// Custom functions.
		//

		BoundingBox* getOrCreateBoundingBox();

		Reference* getReference() const;

	};
	static_assert(sizeof(PhysicalObject) == 0x30, "TES3::PhysicalObject failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::BaseObject)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Object)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::PhysicalObject)

