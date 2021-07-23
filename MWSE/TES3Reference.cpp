#include "TES3Reference.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaActivateEvent.h"
#include "LuaBodyPartsUpdatedEvent.h"
#include "LuaDisarmTrapEvent.h"
#include "LuaPickLockEvent.h"
#include "LuaReferenceActivatedEvent.h"
#include "LuaReferenceDeactivatedEvent.h"
#include "LuaReferenceSceneNodeCreatedEvent.h"

#include "MemoryUtil.h"

#include "NINode.h"
#include "NIPointLight.h"

#include "TES3Actor.h"
#include "TES3AIData.h"
#include "TES3AudioController.h"
#include "TES3BodyPartManager.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3Container.h"
#include "TES3Creature.h"
#include "TES3Game.h"
#include "TES3GameSetting.h"
#include "TES3ItemData.h"
#include "TES3Light.h"
#include "TES3Misc.h"
#include "TES3MobileCreature.h"
#include "TES3MobilePlayer.h"
#include "TES3MobileProjectile.h"
#include "TES3MobController.h"
#include "TES3NPC.h"
#include "TES3WorldController.h"

#include "TES3UIManager.h"

#include "BitUtil.h"

#define TES3_Reference_activate 0x4E9610
#define TES3_Reference_setActionFlag 0x4E55A0
#define TES3_Reference_clearActionFlag 0x4E55E0
#define TES3_Reference_testActionFlag 0x4E5520
#define TES3_Reference_setActionReference 0x4E5610
#define TES3_Reference_getActionReference 0x4E5650

#define TES3_Reference_addItemDataAttachment 0x4E5360

namespace TES3 {
	Reference::Reference() {
		// Do not add code here. Always place new logic in the ctor function, which gets called in Morrowind.exe.
		ctor();
	}

	Reference::~Reference() {
		// Do not add code here. Always place new logic in the dtor function, which gets called in Morrowind.exe.
		dtor();
	}

	const auto TES3_Reference_ctor = reinterpret_cast<void(__thiscall*)(Reference*)>(0x4E4510);
	void Reference::ctor() {
		TES3_Reference_ctor(this);
	}

	const auto TES3_Reference_dtor = reinterpret_cast<void(__thiscall*)(Reference*)>(0x4E45C0);
	void Reference::dtor() {
		if (TES3::Game::previousPlayerTarget == this) {
			TES3::Game::previousPlayerTarget = nullptr;
		}

		TES3_Reference_dtor(this);
	}

	void Reference::activate(Reference* activator, int unknown) {
		// If our event data says to block, don't let the object activate.
		if (mwse::lua::event::ActivateEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object response = stateHandle.triggerEvent(new mwse::lua::event::ActivateEvent(activator, this));
			if (response.get_type() == sol::type::table) {
				sol::table eventData = response;
				if (eventData.get_or("block", false)) {
					return;
				}
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

	const auto TES3_Reference_getLeveledBaseReference = reinterpret_cast<Reference* (__thiscall*)(Reference*)>(0x4E7EE0);
	Reference* Reference::getLeveledBaseReference() {
		return TES3_Reference_getLeveledBaseReference(this);
	}

	const auto TES3_Reference_getScriptVariables = reinterpret_cast<ScriptVariables*(__thiscall*)(Reference*)>(0x4E7020);
	ScriptVariables * Reference::getScriptVariables() {
		return TES3_Reference_getScriptVariables(this);
	}

	const auto TES3_Reference_removeAttachment = reinterpret_cast<void(__thiscall*)(Reference*, Attachment*)>(0x4E4C10);
	void Reference::removeAttachment(TES3::Attachment * attachment) {
		TES3_Reference_removeAttachment(this, attachment);
	}

	const auto TES3_Reference_removeAllAttachments = reinterpret_cast<ScriptVariables * (__thiscall*)(Reference*)>(0x4E4A10);
	void Reference::removeAllAttachments() {
		TES3_Reference_removeAllAttachments(this);
	}

	const auto TES3_Reference_ensureScriptDataIsInstanced = reinterpret_cast<void(__thiscall*)(Reference*)>(0x4E7050);
	void Reference::ensureScriptDataIsInstanced() {
		TES3_Reference_ensureScriptDataIsInstanced(this);
	}

	const auto TES3_Reference_detachDynamicLightFromAffectedNodes = reinterpret_cast<void(__thiscall*)(Reference*)>(0x4EBA00);
	void Reference::detachDynamicLightFromAffectedNodes() {
		TES3_Reference_detachDynamicLightFromAffectedNodes(this);
	}

	const auto TES3_Reference_deleteDynamicLightAttachment = reinterpret_cast<void(__thiscall*)(Reference*)>(0x4E50F0);
	void Reference::deleteDynamicLightAttachment(sol::optional<bool> removeLightFromParent) {
		if (removeLightFromParent.value_or(false)) {
			auto attachedLight = getAttachedDynamicLight();
			if (attachedLight) {
				auto light = attachedLight->light;
				light->parentNode->detachChildHandled(light);
			}
		}
		detachDynamicLightFromAffectedNodes();
		TES3_Reference_deleteDynamicLightAttachment(this);
	}

	LightAttachmentNode* Reference::getAttachedDynamicLight() {
		auto attachment = static_cast<TES3::LightAttachment*>(getAttachment(TES3::AttachmentType::Light));
		return attachment ? attachment->data : nullptr;
	}

	LightAttachmentNode* Reference::getOrCreateAttachedDynamicLight_lua(sol::optional<NI::PointLight*> light_arg, sol::optional<float> phase_arg) {
		auto attachmentNode = getAttachedDynamicLight();
		if (attachmentNode) {
			return attachmentNode;
		}

		// Convenience feature. Create an easily visible light by default.
		NI::Pointer<NI::PointLight> light = light_arg.value_or(nullptr);
		if (light == nullptr) {
			light = NI::PointLight::create();
			light->setRadius(512);
		}

		// If the light is not part of the scene graph yet, automatically attach the light.
		// It can be placed under the attachLight subnode, as in light entities, or the scene node otherwise.
		if (light->parentNode == nullptr && sceneNode != nullptr) {
			auto attachPoint = sceneNode->getObjectByNameAndType<NI::Node>("AttachLight");
			if (attachPoint == nullptr) {
				attachPoint = sceneNode;
			}
			attachPoint->attachChild(light, true);
			attachPoint->update();
		}

		auto attachment = mwse::tes3::_new<TES3::LightAttachment>();
		attachment->type = TES3::AttachmentType::Light;
		attachment->next = nullptr;

		attachmentNode = mwse::tes3::_new<TES3::LightAttachmentNode>();
		memset(attachmentNode, 0, sizeof(TES3::LightAttachmentNode));
		attachmentNode->light = light;
		attachmentNode->flickerPhase = phase_arg.value_or(0.0f);
		attachment->data = attachmentNode;
		
		insertAttachment(attachment);

		TES3::DataHandler::get()->setDynamicLightingForReference(this);

		return attachment->data;
	}

	NI::Pointer<NI::Light> Reference::getAttachedNiLight() {
		auto dynamicLight = getAttachedDynamicLight();
		if (dynamicLight) {
			return dynamicLight->light;
		}
		return nullptr;
	}

	bool Reference::isLeveledSpawn() {
		return getLeveledBaseReference() != nullptr;
	}

	void Reference::setDynamicLighting() {
		auto dataHandler = TES3::DataHandler::get();

		dataHandler->setDynamicLightingForReference(this);
	}

	void Reference::updateLighting() {
		auto dataHandler = TES3::DataHandler::get();

		// Ensure the reference receives scene lighting.
		dataHandler->updateLightingForReference(this);
	}

	const auto TES3_Reference_updateBipedParts = reinterpret_cast<bool (__thiscall*)(Reference*)>(0x4E8B50);
	bool Reference::updateBipedParts() {
		bool result = TES3_Reference_updateBipedParts(this);

		auto actor = getAttachedMobileActor();
		if (actor && mwse::lua::event::BodyPartsUpdatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::BodyPartsUpdatedEvent(this, actor));
		}

		return result;
	}


	const auto TES3_MobilePlayer_sub566500 = reinterpret_cast<void(__thiscall*)(MobilePlayer*)>(0x566500);
	const auto TES3_ModelLoader_loadAnimKF = reinterpret_cast<TES3::KeyframeDefinition * (__thiscall*)(void*, const char*, const char*)>(0x4EE200);

	void Reference::setModelPath(const char* path, bool temporary) {
		auto baseObject = static_cast<TES3::Object*>(getBaseObject());
		char** modelSlot = nullptr;
		char* oldModel = nullptr;

		if (temporary) {
			// Save original model path. getModelPath() does not return the raw path that we want to temporarily modify.
			if (baseObject->objectType == ObjectType::NPC) {
				modelSlot = &static_cast<TES3::NPC*>(baseObject)->model;
			}
			else if (baseObject->objectType == ObjectType::Creature) {
				modelSlot = &static_cast<TES3::Creature*>(baseObject)->model;
			}
			else if (baseObject->objectType == ObjectType::Container) {
				modelSlot = &static_cast<TES3::Container*>(baseObject)->model;
			}
			if (modelSlot) {
				std::swap(*modelSlot, oldModel);
			}
		}

		baseObject->setModelPath(path);

		// Update model if it is currently part of the scenegraph.
		if (sceneNode) {
			reloadAnimation(path);
		}

		// Reset model path if desired.
		if (modelSlot) {
			std::swap(*modelSlot, oldModel);
			if (oldModel) {
				mwse::tes3::free(oldModel);
			}
		}
	}

	void Reference::reloadAnimation(const char* path) {
		auto parentNode = sceneNode->parentNode;

		resetVisualNode();
		auto node = getSceneGraphNode();

		parentNode->attachChild(node, true);
		parentNode->update();

		node->updateEffects();
		node->updateProperties();

		auto worldController = TES3::WorldController::get();
		auto macp = worldController->getMobilePlayer();
		auto mobile = getAttachedMobileActor();
		if (mobile != nullptr) {
			if (mobile->actorType == TES3::MobileActorType::Player) {
				auto firstPersonRef = macp->firstPersonReference;

				if (firstPersonRef->sceneNode) {
					if (this == firstPersonRef && path != nullptr) {
						auto animData = getAttachedAnimationData();
						auto modelLoader = TES3::DataHandler::get()->nonDynamicData->meshData;
						auto keyframes = TES3_ModelLoader_loadAnimKF(modelLoader, path, "MWSE Anim");

						if (animData && keyframes) {
							animData->setOverrideLayerKeyframes(keyframes);
						}
					}
					else {
						auto parent = firstPersonRef->sceneNode->parentNode;
						firstPersonRef->resetVisualNode();
						auto firstPersonSceneNode = firstPersonRef->getSceneGraphNode();
						parent->attachChild(firstPersonSceneNode, true);
						firstPersonSceneNode->updateEffects();
						firstPersonSceneNode->updateProperties();
						firstPersonSceneNode->update();
					}
				}

				if (macp->is3rdPerson()) {
					firstPersonRef->getSceneGraphNode()->setAppCulled(true);
				}
				else {
					macp->reference->getSceneGraphNode()->setAppCulled(true);
				}

				macp->aiPlanner->assignMobileActor(macp);
				worldController->mobController->addPlayerAsCollider();
				TES3_MobilePlayer_sub566500(macp);
			}
			else {
				mobile->vTable.mobileObject->enterLeaveSimulation(mobile, true);
				TES3::WorldController::get()->mobController->addMob(this);
			}
		}
	}

	Cell* Reference::getCell() const {
		// Handle case for the player.
		auto macp = TES3::WorldController::get()->getMobilePlayer();
		if (macp && macp->reference == this) {
			return TES3::DataHandler::get()->currentCell;
		}

		if (owningCollection.asReferenceList == nullptr) {
			return nullptr;
		}

		return owningCollection.asReferenceList->cell;
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
				Vector3 pos(positionTable[1], positionTable[2], positionTable[3]);
				setPosition(&pos);
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
			Vector3 euler;
			matrix->toEulerZYX(&euler.x, &euler.y, &euler.z);
			setOrientation(&euler);
		}
		// Allow a simple table to be provided.
		else if (value.get_type() == sol::type::table) {
			// Get the values from the table.
			sol::table positionTable = value.as<sol::table>();
			if (positionTable.size() == 3) {
				Vector3 ori(positionTable[1], positionTable[2], positionTable[3]);
				setOrientation(&ori);
			}
		}
	}

	bool Reference::enable() {
		// Make sure we're not already enabled.
		if (!getDisabled()) {
			return false;
		}
		BIT_SET_OFF(objectFlags, ObjectFlag::DisabledBit);

		auto dataHandler = TES3::DataHandler::get();

		// Don't cull the scene node.
		auto sceneNode = getSceneGraphNode();
		if (sceneNode) {
			sceneNode->setAppCulled(false);
		}

		// Enable simulation for creatures/NPCs.
		if (baseObject->objectType == TES3::ObjectType::Creature || baseObject->objectType == TES3::ObjectType::NPC) {
			TES3::WorldController::get()->mobController->addMob(this);
			getAttachedMobileActor()->enterLeaveSimulationByDistance();
		}
		// Activators, containers, and statics need collision.
		else if (baseObject->objectType == TES3::ObjectType::Activator || baseObject->objectType == TES3::ObjectType::Container || baseObject->objectType == TES3::ObjectType::Static) {
			dataHandler->updateCollisionGroupsForActiveCells();
		}
		// Lights need to be configured.
		else if (baseObject->objectType == TES3::ObjectType::Light) {
			dataHandler->setDynamicLightingForReference(this);

			// Non-carryable lights also need collision.
			if (!static_cast<TES3::Light*>(baseObject)->getCanCarry()) {
				dataHandler->updateCollisionGroupsForActiveCells();
			}
		}

		// Ensure the reference receives scene lighting.
		dataHandler->updateLightingForReference(this);

		// Finally flag as modified.
		setObjectModified(true);

		return true;
	}

	bool Reference::disable() {
		// Make sure we're not already disabled.
		if (getDisabled()) {
			return false;
		}
		BIT_SET_ON(objectFlags, ObjectFlag::DisabledBit);

		auto dataHandler = TES3::DataHandler::get();

		// Cull the scene node.
		auto sceneNode = getSceneGraphNode();
		if (sceneNode) {
			sceneNode->setAppCulled(true);
		}

		// Leave simulation if we have a mobile.
		if (baseObject->objectType == TES3::ObjectType::Creature || baseObject->objectType == TES3::ObjectType::NPC) {
			auto mact = getAttachedMobileObject();
			if (mact) {
				mact->enterLeaveSimulation(false);
				TES3::WorldController::get()->mobController->removeMob(this);
			}
		}
		// Update lights for objects.
		else if (baseObject->objectType == TES3::ObjectType::Light) {
			detachDynamicLightFromAffectedNodes();

			// Also update collision.
			dataHandler->updateCollisionGroupsForActiveCells();
		}
		// Update collision for everything else.
		else {
			dataHandler->updateCollisionGroupsForActiveCells();
		}

		// Clean up any sounds.
		auto sound = baseObject->getSound();
		if (sound) {
			dataHandler->removeSound(sound, this);
		}

		// Finally flag as modified.
		setObjectModified(true);

		return true;
	}

	bool Reference::getDisabled() const {
		return BIT_TEST(objectFlags, ObjectFlag::DisabledBit);
	}

	void Reference::setDeleted(bool deleted) {
		// Deactivate the reference if needed.
		if (objectType == ObjectType::Reference) {
			// Are we marking a reference deleted in an active cell?
			if (getCell()->getCellActive()) {
				if (deleted) {
					setReferenceInactive();
				}
				else if (!deleted) {
					setReferenceActive(false);
				}
			}
		}

		BIT_SET(objectFlags, ObjectFlag::DeleteBit, deleted);
	}

	bool Reference::getNoCollision() const {
		return BIT_TEST(objectFlags, ObjectFlag::NoCollisionBit);
	}

	void Reference::setNoCollision(bool set, bool updateCollisions) {
		if (getNoCollision() == set) {
			return;
		}

		BIT_SET(objectFlags, ObjectFlag::NoCollisionBit, set);

		if (updateCollisions) {
			TES3::DataHandler::get()->updateCollisionGroupsForActiveCells();
		}
	}

	void Reference::setNoCollision_lua (bool set, sol::optional<bool> updateCollisions) {
		setNoCollision(set, updateCollisions.value_or(true));
	}

	void Reference::setDeletedWithSafety() {
		disable();

		if (baseObject) {
			// This always seems to return 0 and do nothing.
			// But we'll keep it for consistency.
			baseObject->vTable.object->unknown_0x12C(baseObject);
		}

		removeAllAttachments();
		setScale(1.0f);
		setDeleted(true);
	}

	Vector3 * Reference::getPosition() {
		return &position;
	}

	void Reference::setPosition(const Vector3 * newPosition) {
		// Check if the target position is in a different cell.
		Cell * relocateCell = nullptr;
		if (owningCollection.asReferenceList) {
			auto cell = this->owningCollection.asReferenceList->cell;
			if (cell && !cell->isPointInCell(newPosition->x, newPosition->y)) {
				int cellX = Cell::toGridCoord(newPosition->x), cellY = Cell::toGridCoord(newPosition->y);
				relocateCell = DataHandler::get()->nonDynamicData->getCellByGrid(cellX, cellY);
			}
		}
		
		if (relocateCell) {
			// Use relocation helper function.
			// Note that relocate only saves correctly for persistent or player owned refs.
			relocateNoRotation(relocateCell, newPosition);

			// Script item data needs to be instanced if the item is now active but has not been seen before.
			ensureScriptDataIsInstanced();
		}
		else {
			// Reference has not changed cell, allowing a lower impact update.
			position = *newPosition;

			if (sceneNode) {
				sceneNode->localTranslate = position;
				sceneNode->update();
			}
		}

		// Sync position attachment to local position.
		auto attachment = static_cast<NewOrientationAttachment*>(getAttachment(AttachmentType::NewOrientation));
		if (attachment) {
			attachment->position = position;
		}

		setObjectModified(true);
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

	void Reference::setOrientation(const Vector3 * newOrientation) {
		// Orientation uses Euler ZYX angles.
		Vector3 * orientationPackage = getOrientation();
		*orientationPackage = *newOrientation;

		if (orientationPackage != &orientation) {
			orientation = *orientationPackage;
		}

		if (sceneNode) {
			Matrix33 tempOutArg;
			sceneNode->setLocalRotationMatrix(updateSceneMatrix(&tempOutArg, false));
			sceneNode->update();
		}

		setObjectModified(true);
	}

	float Reference::getFacing() {
		return getOrientation()->z;
	}

	void Reference::setFacing(float rotation) {
		Vector3 orientation(0, 0, rotation);
		setOrientation(&orientation);
	}

	float Reference::getAngleToReference(Reference* reference) {
		auto rotation = getFacing();
		Vector3 forward(sinf(rotation), cosf(rotation), 0.0f);
		return (*reference->getPosition() - *getPosition()).angle(&forward);
	}

	const auto TES3_Reference_setTravelDestination = reinterpret_cast<TravelDestination*(__thiscall*)(Reference*, const Vector3 *, const Vector3*)>(0x4E7B80);
	TravelDestination * Reference::setTravelDestination(const Vector3 * position, const Vector3 * orientation, Cell * cell) {
		auto destination = TES3_Reference_setTravelDestination(this, position, orientation);
		destination->cell = cell;
		return destination;
	}

	Matrix33* Reference::updateSceneMatrix(Matrix33* matrix, bool eulerXYZ) {
		return reinterpret_cast<Matrix33* (__thiscall *)(Reference*, Matrix33*, bool)>(0x4E8450)(this, matrix, eulerXYZ);
	}

	bool Reference::getEmptyInventoryFlag() {
		return BIT_TEST(objectFlags, ObjectFlag::EmptyInventoryBit);
    }

    void Reference::setEmptyInventoryFlag(bool set) {
		BIT_SET(objectFlags, ObjectFlag::EmptyInventoryBit, set);
    }

	void Reference::attemptUnlockDisarm(MobileNPC * disarmer, Item * tool, ItemData * toolItemData) {
		if (baseObject->objectType != ObjectType::Door && baseObject->objectType != ObjectType::Container) {
			return;
		}

		auto dataHandler = DataHandler::get();
		auto ndd = dataHandler->nonDynamicData;
		auto worldController = TES3::WorldController::get();

		auto lockData = getAttachedLockNode();

		if (tool->objectType == ObjectType::Probe) {
			if (toolItemData) {
				toolItemData->condition--;
			}

			float chance = 0.0f;

			if (lockData && lockData->trap) {
				// chance = (security + agility/5 + luck/10) * quality * fatigueTerm + trapCost * fTrapCostMult
				chance =
					(disarmer->getSkillValue(SkillID::Security)
						+ disarmer->attributes[Attribute::Agility].getCurrent() * 0.2f
						+ disarmer->attributes[Attribute::Luck].getCurrent() * 0.1f)
					* tool->getQuality()
					* disarmer->getFatigueTerm()
					+ lockData->trap->magickaCost * ndd->GMSTs[GMST::fTrapCostMult]->value.asFloat;
			}

			if (mwse::lua::event::DisarmTrapEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table result = stateHandle.triggerEvent(new mwse::lua::event::DisarmTrapEvent(this, lockData, disarmer, tool, toolItemData, chance, lockData && lockData->trap));
				if (result.valid()) {
					if (result.get_or("block", false)) {
						return;
					}
					chance = result["chance"];

					if (result["clearTarget"]) {
						Game::get()->clearTarget();
					}
				}
			}

			if (lockData && lockData->trap) {
				if (chance <= 0 || chance <= (mwse::tes3::rand() % 100)) {
					dataHandler->addSoundById("Disarm Trap Fail", this, 0, worldController->audioController->getMixVolume(AudioMixType::Effects) * 250);
					if (chance <= 0) {
						TES3::UI::showMessageBox(ndd->GMSTs[GMST::sTrapImpossible]->value.asString);
					}
					else {
						TES3::UI::showMessageBox(ndd->GMSTs[GMST::sTrapFail]->value.asString);
					}
				}
				else {
					lockData->trap = nullptr;
					setObjectModified(true);
					Game::get()->clearTarget();
					dataHandler->addSoundById("Disarm Trap", this, 0, worldController->audioController->getMixVolume(AudioMixType::Effects) * 250);

					auto macp = worldController->getMobilePlayer();
					if (macp == disarmer) {
						macp->exerciseSkill(SkillID::Security, ndd->skills[SkillID::Security].progressActions[0]);
						TES3::UI::showMessageBox(ndd->GMSTs[GMST::sTrapSuccess]->value.asString);
					}
				}
			}
		}
		else if (tool->objectType == ObjectType::Lockpick) {

			float chance = 0.0f;

			if (lockData && lockData->lockLevel > 0) {

				if (toolItemData) {
					toolItemData->condition--;
				}

				// chance = (security + agility/5 + luck/10) * quality * fatigueTerm + lockLevel * fPickLockMult
				chance =
					(disarmer->getSkillValue(SkillID::Security)
						+ disarmer->attributes[Attribute::Agility].getCurrent() * 0.2f
						+ disarmer->attributes[Attribute::Luck].getCurrent() * 0.1f)
					* tool->getQuality()
					* disarmer->getFatigueTerm()
					+ lockData->lockLevel * ndd->GMSTs[GMST::fPickLockMult]->value.asFloat;
			}

			if (mwse::lua::event::PickLockEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table result = stateHandle.triggerEvent(new mwse::lua::event::PickLockEvent(this, lockData, disarmer, tool, toolItemData, chance, lockData && (lockData->lockLevel > 0)));
				if (result.valid()) {
					if (result.get_or("block", false)) {
						return;
					}
					chance = result["chance"];
						
					if (result["clearTarget"]) {
						Game::get()->clearTarget();
					}
				}
			}

			if (lockData && lockData->lockLevel > 0) {
				if (chance <= 0 || chance <= (mwse::tes3::rand() % 100)) {
					dataHandler->addSoundById("Open Lock Fail", this, 0, worldController->audioController->getMixVolume(AudioMixType::Effects) * 250);
					if (chance <= 0) {
						TES3::UI::showMessageBox(ndd->GMSTs[GMST::sLockImpossible]->value.asString);
					}
					else {
						TES3::UI::showMessageBox(ndd->GMSTs[GMST::sLockFail]->value.asString);
					}
				}
				else {
					lockData->locked = false;
					setObjectModified(true);
					Game::get()->clearTarget();
					dataHandler->addSoundById("Open Lock", this, 0, worldController->audioController->getMixVolume(AudioMixType::Effects) * 250);

					auto macp = worldController->getMobilePlayer();
					if (macp == disarmer) {
						macp->exerciseSkill(SkillID::Security, ndd->skills[SkillID::Security].progressActions[1]);
						TES3::UI::showMessageBox(ndd->GMSTs[GMST::sLockSuccess]->value.asString);
					}
				}
			}
		}
	}

	int Reference::getStackSize() {
		TES3::ItemData* itemData = getAttachedItemData();
		return itemData ? itemData->count : 1;
	}

	void Reference::setStackSize(int count) {
		getOrCreateAttachedItemData()->count = count;
	}

	bool Reference::hasValidBaseObject() const {
		return this != nullptr
			&& uint32_t(vTable.object) != TES3::VirtualTableAddress::BaseObject
			&& baseObject != nullptr
			&& uint32_t(baseObject->vTable.object) != TES3::VirtualTableAddress::BaseObject;
	}

	void Reference::setReferenceActive(bool skipDeleted) {
		if (skipDeleted && getDeleted()) {
			return;
		}

		if (mwse::lua::event::ReferenceActivatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::ReferenceActivatedEvent(this));
		}
	}

	void Reference::setReferenceInactive(bool skipDeleted) {
		if (skipDeleted && getDeleted()) {
			return;
		}

		if (mwse::lua::event::ReferenceDeactivatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::ReferenceDeactivatedEvent(this));
		}
	}

	const auto TES3_Reference_getSceneGraphNode = reinterpret_cast<NI::Node*(__thiscall*)(Reference*)>(0x4E81A0);
	NI::Node * Reference::getSceneGraphNode() {
		auto previousNode = sceneNode;
		auto newNode = TES3_Reference_getSceneGraphNode(this);

		if (mwse::lua::event::ReferenceSceneNodeCreatedEvent::getEventEnabled() && hasValidBaseObject() && previousNode != newNode) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::ReferenceSceneNodeCreatedEvent(this));
		}

		return newNode;
	}

#if MWSE_RAISED_FILE_LIMIT
	// New offsets and masks.
	constexpr DWORD ModBits = 10;
	constexpr DWORD FormBits = sizeof(DWORD) * CHAR_BIT - ModBits;
	constexpr DWORD ModMask = ((1 << ModBits) - 1) << FormBits;
	constexpr DWORD FormMask = (1 << FormBits) - 1;
	constexpr DWORD ModCount = 1 << ModBits;
#else
	// Vanilla offsets and masks.
	constexpr DWORD ModBits = 8;
	constexpr DWORD FormBits = sizeof(DWORD) * CHAR_BIT - ModBits;
	constexpr DWORD ModMask = ((1 << ModBits) - 1) << FormBits;
	constexpr DWORD FormMask = (1 << FormBits) - 1;
	constexpr DWORD ModCount = 1 << ModBits;
#endif

	unsigned int Reference::getSourceModId() const {
		return sourceID >> ModBits;
	}

	unsigned int Reference::getSourceFormId() const {
		return sourceID & FormMask;
	}

	unsigned int Reference::getTargetModId() const {
		return targetID >> ModBits;
	}

	unsigned int Reference::getTargetFormId() const {
		return targetID & FormMask;
	}

	sol::optional<bool> Reference::isDead() const {
		auto mobile = getAttachedMobileActor();
		if (mobile) {
			return mobile->isDead();
		}

		switch (baseObject->objectType) {
		case ObjectType::Creature:
			return static_cast<Creature*>(getBaseObject())->health <= 1;
		case ObjectType::NPC:
			return static_cast<NPC*>(getBaseObject())->health <= 1;
		}

		return {};
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

	IteratedList<EquipmentStack*> * Reference::getEquipment() {
		// Only actors have equipment.
		if (baseObject->objectType != ObjectType::Container &&
			baseObject->objectType != ObjectType::Creature &&
			baseObject->objectType != ObjectType::NPC) {
			return NULL;
		}

		return &reinterpret_cast<Actor*>(baseObject)->equipment;
	}

	const auto TES3_game_relocateReference = reinterpret_cast<void(__cdecl*)(Reference*, Cell*, const Vector3*, float)>(0x50EDD0);
	void Reference::relocate(Cell * cell, const Vector3 * position, float rotation) {
		// Store old cell.
		const auto oldCell = getCell();

		// Fire off original function.
		TES3_game_relocateReference(this, cell, position, rotation);
		
		// Determine if cell active state changed.
		const auto oldCellActive = oldCell ? oldCell->getCellActive() : false;
		const auto newCellActive = cell->getCellActive();
		if (!oldCellActive && newCellActive) {
			setReferenceActive();
		}
		else if (oldCellActive && !newCellActive) {
			setReferenceInactive();
		}
	}

	void Reference::relocateNoRotation(Cell* cell, const Vector3* position) {
		const auto z = orientation.z;
		relocate(cell, position, z);
		orientation.z = z;
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

		// Clone the object and set the reference (and implicitly its parent cell) as modified.
		actor->clone(this);
		baseObject->setObjectModified(true);
		setObjectModified(true);

		return true;
	}

	bool Reference::onCloseInventory() {
		// Check to make sure that the contained object is of the right type.
		ObjectType::ObjectType baseType = baseObject->objectType;
		if (baseType != ObjectType::Container && baseType != ObjectType::Creature && baseType != ObjectType::NPC) {
			return false;
		}

		Actor* actor = reinterpret_cast<Actor*>(baseObject);
		actor->vTable.actor->onCloseInventory(actor, this, 0);
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

	Attachment * Reference::getAttachment(AttachmentType::AttachmentType type) const {
		Attachment* attachment = attachments;
		while (attachment && attachment->type != type) {
			attachment = attachment->next;
		}
		return attachment;
	}

	MobileObject* Reference::getAttachedMobileObject() const {
		auto attachment = getAttachment(AttachmentType::ActorData);
		if (attachment) {
			return static_cast<MobileActorAttachment*>(attachment)->data;
		}
		return nullptr;
	}

	MobileActor* Reference::getAttachedMobileActor() const {
		return static_cast<MobileActor*>(getAttachedMobileObject());
	}

	MobileCreature* Reference::getAttachedMobileCreature() const {
		auto mobile = getAttachedMobileActor();
		if (mobile == nullptr || mobile->actorType != MobileActorType::Creature) {
			return nullptr;
		}
		return static_cast<MobileCreature*>(mobile);
	}

	MobileNPC* Reference::getAttachedMobileNPC() const {
		auto mobile = getAttachedMobileActor();
		if (mobile == nullptr || (mobile->actorType != MobileActorType::NPC && mobile->actorType != MobileActorType::Player)) {
			return nullptr;
		}
		return static_cast<MobileNPC*>(mobile);
	}

	MobileProjectile* Reference::getAttachedMobileProjectile() const {
		return static_cast<MobileProjectile*>(getAttachedMobileObject());
	}

	ItemData* Reference::getAttachedItemData() const {
		auto attachment = static_cast<TES3::ItemDataAttachment*>(getAttachment(TES3::AttachmentType::Variables));
		if (attachment) {
			return attachment->data;
		}
		return nullptr;
	}

	const auto TES3_Reference_setAttachedItemData = reinterpret_cast<void(__thiscall*)(Reference*, ItemData*)>(0x4E5360);
	void Reference::setAttachedItemData(ItemData * itemData) {
		if (itemData) {
			TES3_Reference_setAttachedItemData(this, itemData);
		}
		else {
			auto itemDataAttachment = getAttachment(AttachmentType::Variables);
			if (itemDataAttachment) {
				removeAttachment(itemDataAttachment);
			}
		}
	}

	ItemData* Reference::getOrCreateAttachedItemData() {
		auto itemData = getAttachedItemData();

		if (itemData == nullptr) {
			itemData = ItemData::createForObject(baseObject);
			setAttachedItemData(itemData);
		}

		return itemData;
	}

	LockAttachmentNode* Reference::getAttachedLockNode() {
		auto attachment = static_cast<TES3::LockAttachment*>(getAttachment(TES3::AttachmentType::Lock));
		if (attachment) {
			return attachment->data;
		}
		return nullptr;
	}
	
	AnimationData* Reference::getAttachedAnimationData() const {
		auto attachment = static_cast<TES3::AnimationAttachment*>(getAttachment(TES3::AttachmentType::Animation));
		if (attachment) {
			return attachment->data;
		}
		return nullptr;
	}

	BodyPartManager* Reference::getAttachedBodyPartManager() {
		auto attachment = static_cast<TES3::BodyPartManagerAttachment*>(getAttachment(TES3::AttachmentType::BodyPartManager));
		if (attachment) {
			return attachment->data;
		}
		return nullptr;
	}

	TravelDestination* Reference::getAttachedTravelDestination() const {
		auto attachment = static_cast<TES3::TravelDestinationAttachment*>(getAttachment(TES3::AttachmentType::TravelDestination));
		if (attachment) {
			return attachment->data;
		}
		return nullptr;
	}

	sol::table Reference::getAttachments_lua(sol::this_state ts) {
		sol::state_view state = ts;

		sol::table result = state.create_table();

		Attachment* attachment = attachments;
		while (attachment) {
			switch (attachment->type) {
			case AttachmentType::BodyPartManager:
				result["bodyPartManager"] = reinterpret_cast<BodyPartManagerAttachment*>(attachment)->data;
				break;
			case AttachmentType::Light:
				result["light"] = reinterpret_cast<LightAttachment*>(attachment)->data;
				break;
			case AttachmentType::Lock:
				result["lock"] = reinterpret_cast<LockAttachment*>(attachment)->data;
				break;
			case AttachmentType::LeveledBaseReference:
				result["leveledBase"] = reinterpret_cast<LeveledBaseReferenceAttachment*>(attachment)->data;
				break;
			case AttachmentType::TravelDestination:
				result["travelDestination"] = reinterpret_cast<TravelDestinationAttachment*>(attachment)->data;
				break;
			case AttachmentType::Variables:
				result["variables"] = reinterpret_cast<ItemDataAttachment*>(attachment)->data;
				break;
			case AttachmentType::ActorData:
				result["actor"] = reinterpret_cast<MobileActorAttachment*>(attachment)->data;
				break;
			}

			attachment = attachment->next;
		}

		return result;
	}

	bool Reference::getSupportsLuaData() const {
		auto itemData = getAttachedItemData();

		// Prevent adding a lua table if there's more than one item involved.
		if (itemData && itemData->count > 1) {
			return false;
		}

		// Does the base object support it?
		if (!baseObject->getSupportsLuaData()) {
			return false;
		}

		return true;
	}

	sol::table Reference::getLuaTable() {
		if (!getSupportsLuaData()) {
			return sol::nil;
		}

		return getOrCreateAttachedItemData()->getOrCreateLuaDataTable();
	}

	sol::table Reference::getLuaTempTable() {
		if (!getSupportsLuaData()) {
			return sol::nil;
		}

		return getOrCreateAttachedItemData()->getOrCreateLuaTempDataTable();
	}

	void Reference::activate_lua(Reference* target) {
		target->activate(this);
	}

	std::shared_ptr<mwse::lua::ScriptContext> Reference::getContext_lua() {
		if (baseObject->getScript() == nullptr) {
			return nullptr;
		}

		auto variables = getAttachedItemData();
		if (variables == nullptr) {
			return nullptr;
		}

		return std::make_shared<mwse::lua::ScriptContext>(variables->script, variables->scriptData);
	}

	void Reference::updateSceneGraph_lua() {
		Matrix33 tempOutArg;
		sceneNode->setLocalRotationMatrix(updateSceneMatrix(&tempOutArg));
		sceneNode->update();
		setObjectModified(true);
	}

	Reference* Reference::getThis() {
		return this;
	}

}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Reference)
