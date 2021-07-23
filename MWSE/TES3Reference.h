#pragma once

#include "TES3ScriptLua.h"

#include "TES3Defines.h"
#include "TES3Attachment.h"
#include "TES3Vectors.h"

#include "NILight.h"

namespace TES3 {
	struct Reference : Object {
		struct ReferenceData {
			PhysicalObject* baseObject; // 0x0
			Vector3 orientation; // 0x4
			Vector3 position; // 0x10
			Attachment* attachments; // 0x1C
			unsigned int sourceID; // 0x20
			unsigned int targetID; // 0x24
		};
		// Backwards compatibility union to provide direct access into ReferenceData.
		union {
			ReferenceData referenceData; // 0x28
			struct {
				PhysicalObject* baseObject; // 0x28
				Vector3 orientation; // 0x2c
				Vector3 position; // 0x38
				Attachment* attachments; // 0x44
				unsigned int sourceID; // 0x48
				unsigned int targetID; // 0x4C
			};
		};

		//
		// Basic operators.
		//

		Reference();
		~Reference();

		//
		// Other related this-call functions.
		//

		void ctor();
		void dtor();

		void activate(Reference* activator, int unknown = 1);
		void setActionFlag(int);
		void clearActionFlag(int);
		bool testActionFlag(int);
		void setActionReference(Reference*);
		Reference * getActionReference();

		ItemDataAttachment* addItemDataAttachment(ItemData*);
		Vector3* getOrCreateOrientationFromAttachment();
		Vector3* getPositionFromAttachment();
		LockAttachmentNode* getOrCreateLockNode();
		Reference* getLeveledBaseReference();
		ScriptVariables * getScriptVariables();
		void removeAttachment(TES3::Attachment* attachment);
		void removeAllAttachments();
		void ensureScriptDataIsInstanced();

		void detachDynamicLightFromAffectedNodes();
		void deleteDynamicLightAttachment(sol::optional<bool> removeLightFromParent = false);

		void setModelPath(const char* path, bool temporary = false);
		void reloadAnimation(const char* path);
		bool updateBipedParts();

		//
		// Other utility functions.
		//

		bool enable();
		bool disable();
		bool getDisabled() const;

		void setDeleted(bool deleted);
		void setDeletedWithSafety();

		bool getNoCollision() const;
		void setNoCollision(bool set, bool updateCells = true);
		void setNoCollision_lua(bool set, sol::optional<bool> updateCells);

		void setReferenceActive(bool skipDeleted = true);
		void setReferenceInactive(bool skipDeleted = true);

		Vector3 * getPosition();
		void setPosition(const Vector3 * newPosition);

		Vector3 * getOrientation();
		void setOrientation(const Vector3 * newOrientation);

		float getFacing();
		void setFacing(float facing);

		float getAngleToReference(Reference* reference);

		TravelDestination * setTravelDestination(const Vector3 * position, const Vector3 * orientation, Cell * cell = nullptr);

		Matrix33* updateSceneMatrix(Matrix33* matrix, bool eulerXYZ = false);

		Inventory * getInventory();
		IteratedList<EquipmentStack*> * getEquipment();

		void relocate(Cell * cell, const Vector3 * position, float rotation);
		void relocateNoRotation(Cell* cell, const Vector3* position);
		bool clone();
		bool onCloseInventory();

		bool insertAttachment(Attachment* attachment);
		Attachment* getAttachment(AttachmentType::AttachmentType type) const;
		MobileObject* getAttachedMobileObject() const;
		MobileActor* getAttachedMobileActor() const;
		MobileCreature* getAttachedMobileCreature() const;
		MobileNPC* getAttachedMobileNPC() const;
		MobileProjectile* getAttachedMobileProjectile() const;
		ItemData* getAttachedItemData() const;
		void setAttachedItemData(ItemData * itemData);
		ItemData* getOrCreateAttachedItemData();
		LockAttachmentNode* getAttachedLockNode();
		AnimationData* getAttachedAnimationData() const;
		BodyPartManager* getAttachedBodyPartManager() ;
		TravelDestination* getAttachedTravelDestination() const;

		LightAttachmentNode* getAttachedDynamicLight();
		LightAttachmentNode* getOrCreateAttachedDynamicLight_lua(sol::optional<NI::PointLight*> light, sol::optional<float> value);
		NI::Pointer<NI::Light> getAttachedNiLight();

		bool isLeveledSpawn();

		void setDynamicLighting();
		void updateLighting();

		bool getEmptyInventoryFlag();
		void setEmptyInventoryFlag(bool);

		void attemptUnlockDisarm(MobileNPC * disarmer, Item * tool, ItemData * itemData = nullptr);

		int getStackSize();
		void setStackSize(int count);
		
		bool hasValidBaseObject() const;

		// Override for references to raise an event when their scene node is created.
		NI::Node * getSceneGraphNode();

		unsigned int getSourceModId() const;
		unsigned int getSourceFormId() const;
		unsigned int getTargetModId() const;
		unsigned int getTargetFormId() const;

		sol::optional<bool> isDead() const;

		//
		// Lua interface functions.
		//

		Cell* getCell() const;

		void setPositionFromLua(sol::stack_object value);
		void setOrientationFromLua(sol::stack_object value);

		// Return a table of name-keyed attachments for this object.
		sol::table getAttachments_lua(sol::this_state ts);

		bool getSupportsLuaData() const;
		sol::table getLuaTable();
		sol::table getLuaTempTable();

		// For lua activation, reverse param order.
		void activate_lua(Reference* target);

		std::shared_ptr<mwse::lua::ScriptContext> getContext_lua();

		void updateSceneGraph_lua();

		Reference* getThis();

	};
	static_assert(sizeof(Reference) == 0x50, "TES3::Reference failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Reference)
