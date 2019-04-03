#include "TES3Reference.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaActivateEvent.h"

#include "TES3Util.h"

#include "NINode.h"

#include "TES3Actor.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3GameSetting.h"
#include "TES3ItemData.h"
#include "TES3MobileCreature.h"
#include "TES3MobilePlayer.h"
#include "TES3MobileProjectile.h"
#include "TES3NPC.h"

#define TES3_Reference_activate 0x4E9610
#define TES3_Reference_setActionFlag 0x4E55A0
#define TES3_Reference_clearActionFlag 0x4E55E0
#define TES3_Reference_testActionFlag 0x4E5520
#define TES3_Reference_setActionReference 0x4E5610
#define TES3_Reference_getActionReference 0x4E5650

#define TES3_Reference_addItemDataAttachment 0x4E5360

namespace TES3 {
	const auto TES3_Reference_setMobileActor = reinterpret_cast<MobileActor* (__thiscall*)(Reference*, MobileActor*)>(0x4E5770);
	const auto TES3_Reference_removeAttachment = reinterpret_cast<void(__thiscall*)(Reference*, Attachment*)>(0x4E4C10);

	void Reference::activate(Reference* activator, int unknown) {
		// If our event data says to block, don't let the object activate.
		sol::object response = mwse::lua::LuaManager::getInstance().triggerEvent(new mwse::lua::event::ActivateEvent(activator, this));
		if (response.get_type() == sol::type::table) {
			sol::table eventData = response;
			if (eventData["block"] == true) {
				return;
			}
		}

		reinterpret_cast<void(__thiscall *)(Reference*, Reference*, int)>(TES3_Reference_activate)(this, activator, unknown);
	}

	void Reference::setActionFlag(int flag) {
		reinterpret_cast<void(__thiscall *)(Reference*, int)>(TES3_Reference_setActionFlag)(this, flag);
	}

	void Reference::clearActionFlag(int flag) {
		reinterpret_cast<void(__thiscall *)(Reference*, int)>(TES3_Reference_clearActionFlag)(this, flag);
	}

	bool Reference::testActionFlag(int flag) {
		return (reinterpret_cast<signed char(__thiscall *)(Reference*, int)>(TES3_Reference_testActionFlag)(this, flag) != 0);
	}

	void Reference::setActionReference(Reference* reference) {
		reinterpret_cast<void (__thiscall *)(Reference*, Reference*)>(TES3_Reference_setActionReference)(this, reference);
	}

	Reference* Reference::getActionReference() {
		return reinterpret_cast<Reference* (__thiscall *)(Reference*)>(TES3_Reference_getActionReference)(this);
	}

	ItemDataAttachment* Reference::addItemDataAttachment(ItemData* data) {
		return reinterpret_cast<ItemDataAttachment* (__thiscall *)(Reference*, ItemData*)>(TES3_Reference_addItemDataAttachment)(this, data);
	}

	Vector3* Reference::getOrCreateOrientationFromAttachment() {
		return reinterpret_cast<Vector3* (__thiscall *)(Reference*)>(0x4E5970)(this);
	}

	Vector3* Reference::getPositionFromAttachment() {
		return reinterpret_cast<Vector3* (__thiscall *)(Reference*)>(0x4E58D0)(this);
	}

	LockAttachmentNode* Reference::getOrCreateLockNode() {
		return reinterpret_cast<LockAttachmentNode* (__thiscall *)(Reference*)>(0x4E7DF0)(this);
	}

	const auto TES3_Reference_getScriptVariables = reinterpret_cast<ScriptVariables*(__thiscall*)(Reference*)>(0x4E7020);
	ScriptVariables * Reference::getScriptVariables() {
		return TES3_Reference_getScriptVariables(this);
	}

	void Reference::removeAttachment(TES3::Attachment * attachment) {
		TES3_Reference_removeAttachment(this, attachment);
	}

	const auto TES3_Reference_updateEquipment = reinterpret_cast<void(__thiscall*)(Reference*)>(0x4E8B50);
	void Reference::updateEquipment() {
		TES3_Reference_updateEquipment(this);
	}

	void Reference::setPositionFromLua(sol::stack_object value) {
		// Is it a vector?
		if (value.is<Vector3*>()) {
			setPosition(value.as<Vector3*>());
		}
		// Allow a simple table to be provided.
		else if (value.get_type() == sol::type::table) {
			// Get the values from the table.
			sol::table positionTable = value.as<sol::table>();
			if (positionTable.size() == 3) {
				setPosition(positionTable[1], positionTable[2], positionTable[3]);
			}
		}
	}

	void Reference::setOrientationFromLua(sol::stack_object value) {
		// Is it a vector?
		if (value.is<Vector3*>()) {
			setOrientation(value.as<Vector3*>());
		}
		// Is it a matrix?
		else if (value.is<Matrix33*>()) {
			auto matrix = value.as<TES3::Matrix33*>();
			float x, y, z;
			matrix->toEulerZYX(&x, &y, &z);
			setOrientation(x, y, z);
		}
		// Allow a simple table to be provided.
		else if (value.get_type() == sol::type::table) {
			// Get the values from the table.
			sol::table positionTable = value.as<sol::table>();
			if (positionTable.size() == 3) {
				setOrientation(positionTable[1], positionTable[2], positionTable[3]);
			}
		}
	}

	Vector3 * Reference::getPosition() {
		return &position;
	}

	void Reference::setPosition(float x, float y, float z) {
		if (sceneNode) {
			sceneNode->localTranslate.x = x;
			sceneNode->localTranslate.y = y;
			sceneNode->localTranslate.z = z;
			sceneNode->propagatePositionChange();
		}

		// Set local position.
		position.x = x;
		position.y = y;
		position.z = z;

		// Sync position attachment to local position.
		auto attachment = static_cast<NewOrientationAttachment*>(getAttachment(AttachmentType::NewOrientation));
		if (attachment) {
			attachment->position = position;
		}

		setObjectModified(true);
	}

	void Reference::setPosition(Vector3* positionVec) {
		setPosition(positionVec->x, positionVec->y, positionVec->z);
	}

	Vector3 * Reference::getOrientation() {
		// NPCs and Creatures use the base orientation in the reference struct.
		ObjectType::ObjectType type = baseObject->objectType;
		if (type == ObjectType::NPC || type == ObjectType::Creature) {
			return &orientation;
		}

		// Everything else uses the positioning attachment.
		return getOrCreateOrientationFromAttachment();
	}

	void Reference::setOrientation(float x, float y, float z) {
		Vector3 * orientationPackage = getOrientation();
		orientationPackage->x = x;
		orientationPackage->y = y;
		orientationPackage->z = z;

		if (orientationPackage != &orientation) {
			orientation = *orientationPackage;
		}

		if (sceneNode) {
			Matrix33 tempOutArg;
			sceneNode->setLocalRotationMatrix(updateSceneMatrix(&tempOutArg));
			sceneNode->propagatePositionChange();
		}

		setObjectModified(true);
	}

	void Reference::setOrientation(Vector3 * value) {
		setOrientation(value->x, value->y, value->z);
	}

	const auto TES3_Reference_setTravelDestination = reinterpret_cast<TravelDestination*(__thiscall*)(Reference*, Vector3 *, Vector3*)>(0x4E7B80);
	TravelDestination * Reference::setTravelDestination(Vector3 * position, Vector3 * orientation, Cell * cell) {
		auto destination = TES3_Reference_setTravelDestination(this, position, orientation);
		destination->cell = cell;
		return destination;
	}

	Matrix33* Reference::updateSceneMatrix(Matrix33* matrix, bool unknown) {
		return reinterpret_cast<Matrix33* (__thiscall *)(Reference*, Matrix33*, bool)>(0x4E8450)(this, matrix, unknown);
	}

	bool Reference::getEmptyInventoryFlag() {
		return getBaseObjectFlag(TES3::ObjectFlag::EmptyInventory);
    }

    void Reference::setEmptyInventoryFlag(bool set) {
		setBaseObjectFlag(TES3::ObjectFlag::EmptyInventory, set);
    }

	Inventory * Reference::getInventory() {
		// Only actors have equipment.
		if (baseObject->objectType != ObjectType::Container &&
			baseObject->objectType != ObjectType::Creature &&
			baseObject->objectType != ObjectType::NPC) {
			return NULL;
		}

		return &reinterpret_cast<Actor*>(baseObject)->inventory;
	}

	Iterator<EquipmentStack> * Reference::getEquipment() {
		// Only actors have equipment.
		if (baseObject->objectType != ObjectType::Container &&
			baseObject->objectType != ObjectType::Creature &&
			baseObject->objectType != ObjectType::NPC) {
			return NULL;
		}

		return &reinterpret_cast<Actor*>(baseObject)->equipment;
	}

	bool Reference::clone() {
		// Check to make sure that the contained object is of the right type.
		ObjectType::ObjectType baseType = baseObject->objectType;
		if (baseType != ObjectType::Container && baseType != ObjectType::Creature && baseType != ObjectType::NPC) {
			return false;
		}

		// Check to see if the base object is not an instance.
		Actor * actor = reinterpret_cast<Actor*>(baseObject);
		if (!(actor->actorFlags & TES3::ActorFlag::IsBase)) {
			return false;
		}

		// Clone the object and set the reference (and its parent cell) as modified.
		actor->clone(this);
		baseObject->setObjectModified(true);
		setObjectModified(true);
		if (owningCollection.asReferenceList && owningCollection.asReferenceList->cell) {
			owningCollection.asReferenceList->cell->setObjectModified(true);
		}

		return true;
	}

	bool Reference::insertAttachment(Attachment* attachment) {
		// If there are no attachments, set this as the first.
		if (attachments == nullptr) {
			attachments = attachment;
			return true;
		}

		// Go through the attachments, and return false if we already have this attachment type.
		Attachment * tempAttachment = attachments;
		Attachment * lastAttachment = nullptr;
		while (tempAttachment) {
			if (tempAttachment->type == attachment->type) {
				return false;
			}
			lastAttachment = tempAttachment;
			tempAttachment = tempAttachment->next;
		}

		// Link the attachment.
		lastAttachment->next = attachment;
		return true;
	}

	Attachment * Reference::getAttachment(AttachmentType::AttachmentType type) {
		Attachment* attachment = attachments;
		while (attachment && attachment->type != type) {
			attachment = attachment->next;
		}
		return attachment;
	}

	MobileObject* Reference::getAttachedMobileObject() {
		auto attachment = getAttachment(AttachmentType::ActorData);
		if (attachment) {
			return static_cast<MobileActorAttachment*>(attachment)->data;
		}
		return nullptr;
	}

	MobileActor* Reference::getAttachedMobileActor() {
		return static_cast<MobileActor*>(getAttachedMobileObject());
	}

	MobileCreature* Reference::getAttachedMobileCreature() {
		auto mobile = getAttachedMobileActor();
		if (mobile == nullptr || mobile->actorType != MobileActorType::Creature) {
			return nullptr;
		}
		return static_cast<MobileCreature*>(mobile);
	}

	MobileNPC* Reference::getAttachedMobileNPC() {
		auto mobile = getAttachedMobileActor();
		if (mobile == nullptr || (mobile->actorType != MobileActorType::NPC && mobile->actorType != MobileActorType::Player)) {
			return nullptr;
		}
		return static_cast<MobileNPC*>(mobile);
	}

	MobileProjectile* Reference::getAttachedMobileProjectile() {
		return static_cast<MobileProjectile*>(getAttachedMobileObject());
	}

	ItemData* Reference::getAttachedItemData() {
		auto attachment = static_cast<TES3::ItemDataAttachment*>(getAttachment(TES3::AttachmentType::Variables));
		if (attachment) {
			return attachment->data;
		}
		return nullptr;
	}

	const auto TES3_Reference_getOrCreateAttachedItemData = reinterpret_cast<ItemData* (__thiscall*)(Reference*)>(0x4E7640);
	ItemData* Reference::getOrCreateAttachedItemData() {
		return TES3_Reference_getOrCreateAttachedItemData(this);
	}

	LockAttachmentNode* Reference::getAttachedLockNode() {
		auto attachment = static_cast<TES3::LockAttachment*>(getAttachment(TES3::AttachmentType::Lock));
		if (attachment) {
			return attachment->data;
		}
		return nullptr;
	}

}
