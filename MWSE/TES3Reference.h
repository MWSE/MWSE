#pragma once

#include "sol.hpp"

#include "TES3Defines.h"
#include "TES3Attachment.h"
#include "TES3Vectors.h"

namespace TES3 {
	struct Reference : Object {
		PhysicalObject * baseObject; // 0x28
		Vector3 orientation; // 0x2c
		Vector3 position; // 0x38
		Attachment * attachments; // 0x44
		int sourceID; // 0x48
		int targetID; // 0x4C

		//
		// Other related this-call functions.
		//

		__declspec(dllexport) void activate(Reference* activator, int unknown = 1);
		__declspec(dllexport) void setActionFlag(int);
		__declspec(dllexport) void clearActionFlag(int);
		__declspec(dllexport) bool testActionFlag(int);
		__declspec(dllexport) void setActionReference(Reference*);
		__declspec(dllexport) Reference * getActionReference();

		__declspec(dllexport) ItemDataAttachment* addItemDataAttachment(ItemData*);
		__declspec(dllexport) Vector3* getOrCreateOrientationFromAttachment();
		__declspec(dllexport) Vector3* getPositionFromAttachment();
		__declspec(dllexport) LockAttachmentNode* getOrCreateLockNode();
		__declspec(dllexport) ScriptVariables * getScriptVariables();
		__declspec(dllexport) void removeAttachment(TES3::Attachment * attachment);

		__declspec(dllexport) void updateEquipment();

		//
		// Other utility functions.
		//

		__declspec(dllexport) Vector3 * getPosition();
		__declspec(dllexport) void setPosition(float x, float y, float z);
		__declspec(dllexport) void setPosition(Vector3* positionVec);

		__declspec(dllexport) Vector3 * getOrientation();
		__declspec(dllexport) void setOrientation(float x, float y, float z);
		__declspec(dllexport) void setOrientation(Vector3* value);

		__declspec(dllexport) TravelDestination * setTravelDestination(Vector3 * position, Vector3 * orientation, Cell * cell = nullptr);

		__declspec(dllexport) Matrix33* updateSceneMatrix(Matrix33* matrix, bool unknown = true);

		__declspec(dllexport) Inventory * getInventory();
		__declspec(dllexport) Iterator<EquipmentStack> * getEquipment();

		__declspec(dllexport) bool clone();

		__declspec(dllexport) bool insertAttachment(Attachment* attachment);
		__declspec(dllexport) Attachment* getAttachment(AttachmentType::AttachmentType type);
		__declspec(dllexport) MobileObject* getAttachedMobileObject();
		__declspec(dllexport) MobileActor* getAttachedMobileActor();
		__declspec(dllexport) MobileCreature* getAttachedMobileCreature();
		__declspec(dllexport) MobileNPC* getAttachedMobileNPC();
		__declspec(dllexport) MobileProjectile* getAttachedMobileProjectile();
		__declspec(dllexport) ItemData* getAttachedItemData();
		__declspec(dllexport) ItemData* getOrCreateAttachedItemData();
		__declspec(dllexport) LockAttachmentNode* getAttachedLockNode();

		bool getEmptyInventoryFlag();
		void setEmptyInventoryFlag(bool);

		//
		// Lua interface functions.
		//

		void setPositionFromLua(sol::stack_object value);
		void setOrientationFromLua(sol::stack_object value);

		// Return a table (or nil) of attachments for this object.
		sol::object getAttachments();

		sol::table getLuaTable();

	};
	static_assert(sizeof(Reference) == 0x50, "TES3::Reference failed size validation");
}
