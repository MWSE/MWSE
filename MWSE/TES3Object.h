#pragma once

#include "TES3BaseObject.h"
#include "TES3Defines.h"

#include "NIDefines.h"

#include "LinkedObjectsList.h"
#include "NIIteratedList.h"

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
	using se::LinkedObjectList;

	//
	// The core building blocks of TES3 objects.
	//

	struct ObjectVirtualTable : BaseObjectVirtualTable {
		void (__thiscall* copy)(Object*, const Object*, bool); // 0x24
		void (__thiscall* setID)(Object*, const char*); // 0x28
		NI::Node* (__thiscall* getSceneGraphNode)(Object*); // 0x2C
		void* unknown_0x30;
		void* unknown_0x34;
		const char* (__thiscall* getName)(const Object*); // 0x38
		const char* (__thiscall* getIconPath)(const Object*); // 0x3C
		void* unknown_0x40;
		void* unknown_0x44;
		const char* (__thiscall* getModelPath)(const Object*); // 0x48
		Script* (__thiscall* getScript)(const Object*); // 0x4C
		Sound* (__thiscall* getSound)(const Object*); // 0x50
		const char* (__thiscall* getRaceID)(const Object*); // 0x54
		const char* (__thiscall* getClassID)(const Object*); // 0x58
		const char* (__thiscall* getFactionID)(const Object*); // 0x5C
		Race* (__thiscall* getRace)(const Object*); // 0x60
		Class* (__thiscall* getClass)(const Object*); // 0x64
		Faction* (__thiscall* getFaction)(const Object*); // 0x68
		bool (__thiscall* isFemale)(const Object*); // 0x6C
		int (__thiscall* getFactionRank)(const Object*); // 0x70
		int (__thiscall* getLevel)(const Object*); // 0x74
		void* unknown_0x78;
		void (__thiscall* setDispositionRaw)(Object*, int); // 0x7C
		int (__thiscall* modDisposition)(Object*, int); // 0x80
		int (__thiscall* getReputation)(const Object*); // 0x84
		void (__thiscall* setReputation)(Object*, int); // 0x88
		int (__thiscall* getDispositionRaw)(const Object*); // 0x8C
		void (__thiscall* modReputation)(Object*, int); // 0x90
		int (__thiscall* getType)(const Object*); // 0x94
		const char* (__thiscall* getTypeName)(const Object*); // 0x98
		float (__thiscall* getWeight)(const Object*); // 0x9C
		int (__thiscall* getValue)(const Object*); // 0xA0
		void (__thiscall* setDurability)(Object*, int); // 0xA4
		int (__thiscall* getDurability)(const Object*); // 0xA8
		int (__thiscall* getMagicka)(const Object*); // 0xAC
		int (__thiscall* getFatigue)(const Object*); // 0xB0
		float (__thiscall* getQuality)(const Object*); // 0xB4
		bool (__thiscall* isLeftPartOfPair)(const Object*); // 0xB8
		bool (__thiscall* isEssential)(const Object*); // 0xBC
		bool (__thiscall* isRespawn)(const Object*); // 0xC0
		void* unknown_0xC4;
		int (__thiscall* getUses)(const Object*); // 0xC8
		int (__thiscall* getEnchantCapacity)(const Object*);  // 0xCC
		Enchantment* (__thiscall* getEnchantment)(const Object*); // 0xD0
		Enchantment* (__thiscall* setEnchantment)(Object*, Enchantment*); // 0xD4
		AIConfig* (__thiscall* getAIConfig)(const Object*); // 0xD8
		NI::IteratedList<AIConfig>* (__thiscall* getAIConfigsList)(Object*); // 0xDC
		void (__thiscall* resolveInternalIDs)(Object*, NonDynamicData*); // 0xE0
		void* unknown_0xE4;
		bool (__thiscall* getAutoCalc)(const Object*); // 0xE8
		void (__thiscall* setAutoCalc)(Object*, bool); // 0xEC
		void* unknown_0xF0;
		NI::Node* (__thiscall* cloneNewSceneNode)(Object*, Reference*, bool); // 0xF4
		void (__thiscall* releaseClone)(Object*, Reference*); // 0xF8
		void (__thiscall* setModelPath)(Object*, const char*); // 0xFC
		void* unknown_0x100;
		void* unknown_0x104;
		bool (__thiscall* isLocationMarker)(const Object*); // 0x108
		void (__thiscall* setName)(Object*, const char*); // 0x10C
		void (__thiscall* reevaluateEquipment)(Object*); // 0x110
		void* unknown_0x114;
		void* unknown_0x118;
		void (__thiscall* resetVisualNode)(Object*, NI::Node*); // 0x11C
		float (__thiscall* getScale)(const Object*); // 0x120
		void (__thiscall* setScale)(Object*, float, bool); // 0x124
		void* unknown_0x128;
		int (__thiscall* unknown_0x12C)(Object*); // 0x12C
		bool (__thiscall* saveStolenFlags)(Object*, GameFile*); // 0x130
		bool (__thiscall* loadMesh)(Object*); // 0x134
		void (__thiscall* applyCollisionMode)(Object*, NI::Node*, int, int); // 0x138
	};
	static_assert(sizeof(ObjectVirtualTable) == 0x13C, "TES3::ObjectVirtualTable failed size validation");

	struct Object : BaseObject {
		NI::Pointer<NI::Node> sceneNode; // 0x10
		union {
			LinkedObjectList<Object>* asGenericList;
			LinkedObjectList<Spell>* asSpellList;
			ReferenceList* asReferenceList;
		} owningCollection; // 0x14
		void* referenceToThis; // 0x18
		Object* previousInCollection; // 0x1C
		Object* nextInCollection; // 0x20
		NI::Pointer<NI::Node> sceneCollisionRoot; // 0x24

		//
		// Function wrappers for our virtual table.
		//

		void copy(const Object* from, bool shareAIPackageConfig = false);
		void setID(const char* id);
		const char* getName() const;
		const char* getIconPath() const;
		const char* getModelPath() const;
		Script* getScript() const;
		Sound* getSound() const;
		const char* getRaceID() const;
		const char* getClassID() const;
		const char* getFactionID() const;
		Race* getRace() const;
		Class* getClass() const;
		Faction* getFaction() const;
		bool isFemale() const;
		int getFactionRank() const;
		int getLevel() const;
		void setDispositionRaw(int);
		int modDisposition(int);
		int getReputation() const;
		void setReputation(int);
		int getDispositionRaw() const;
		void modReputation(int);
		int getType() const;
		const char* getTypeName() const;
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
		Enchantment* getEnchantment() const;
		Enchantment* setEnchantment(Enchantment* enchantment);
		AIConfig* getAIConfig() const;
		bool getAutoCalc() const;
		void setAutoCalc(bool);
		void setModelPath(const char* path);
		void setName(const char* name);
		void reevaluateEquipment();
		void resetVisualNode(NI::Node* node = nullptr);
		float getScale() const;
		void setScale(float value, bool cap = false);

		bool getIsLocationMarker() const;

		bool getSupportsLuaData() const;

		NI::Node* getSceneGraphNode();
		bool loadMesh();

		//
		// Custom functions.
		//

		template <typename T>
		T* createCopy_lua(sol::optional<sol::table> params) const {
			auto created = new T();
			created->copy(this);
			finishCreateCopy_lua(created, params);	// May delete created and throw std::runtime_error.
			return created;
		}
		static void finishCreateCopy_lua(Object* created, sol::optional<sol::table> params);

		Object* skipDeletedObjects();
		ReferenceList* getOwningCollection() const;

		void setScale_lua(float scale);

		bool supportsActivate() const;

	};
	static_assert(sizeof(Object) == 0x28, "TES3::Object failed size validation");

	struct PhysicalObjectVirtualTable : ObjectVirtualTable {
		NI::Node* (__thiscall* cloneNewModelWithBodyParts)(PhysicalObject*, Reference*); // 0x13C
		bool (__thiscall* reloadBaseModel)(PhysicalObject*, const char*); // 0x140
		NI::IteratedList<BaseObject*>* (__thiscall* getStolenList)(PhysicalObject*); // 0x144
	};

	struct PhysicalObject : Object {
		NI::BoundingBox* boundingBox; // 0x28
		char* objectID; // 0x2C

		//
		// Function wrappers for our virtual table.
		//

		NI::IteratedList<BaseObject*>* getStolenList();

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

		NI::BoundingBox* getOrCreateBoundingBox();

		Reference* getReference() const;
		const std::vector<Reference*>& getReferences() const;

	};
	static_assert(sizeof(PhysicalObject) == 0x30, "TES3::PhysicalObject failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::BaseObject)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Object)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::PhysicalObject)

