#include "TES3BodyPartManager.h"

#include "LuaManager.h"

#include "LuaBodyPartAssignedEvent.h"

#include "TES3ActorAnimationController.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace TES3 {
	const auto TES3_BodyPartManager_ctor = reinterpret_cast<BodyPartManager* (__thiscall*)(BodyPartManager*, NI::Node*, Reference*)>(0x472580);
	BodyPartManager* BodyPartManager::ctor(NI::Node* parentNode, Reference* ref) {
		TES3_BodyPartManager_ctor(this, parentNode, ref);
		reference = ref;
		return this;
	}

	const auto TES3_BodyPartManager_getActiveBodyPartForItem = reinterpret_cast<BodyPartManager::ActiveBodyPart * (__thiscall*)(BodyPartManager*, Item*)>(0x472DA0);
	BodyPartManager::ActiveBodyPart* BodyPartManager::getActiveBodyPartForItem(Item* item) {
		return TES3_BodyPartManager_getActiveBodyPartForItem(this, item);
	}

	const auto TES3_BodyPartManager_getActiveBodyPartNode = reinterpret_cast<NI::Node * (__thiscall*)(BodyPartManager*, BodyPartManager::ActiveBodyPart::Layer, BodyPartManager::ActiveBodyPart::Index)>(0x473030);
	NI::Node* BodyPartManager::getActiveBodyPartNode(ActiveBodyPart::Layer layer, ActiveBodyPart::Index index) {
		return TES3_BodyPartManager_getActiveBodyPartNode(this, layer, index);
	}

	const auto TES3_BodyPartManager_setActivePartData = reinterpret_cast<void(__thiscall*)(BodyPartManager*, BodyPartManager::ActiveBodyPart::Layer, BodyPartManager::ActiveBodyPart::Index, bool, NI::Node*)>(0x472EA0);
	void BodyPartManager::setActivePartData(ActiveBodyPart::Layer layer, ActiveBodyPart::Index index, bool overwriteData, NI::Node* node) {
		TES3_BodyPartManager_setActivePartData(this, layer, index, overwriteData, node);
	}

	const auto TES3_BodyPartManager_setBodyPartForItem = reinterpret_cast<void(__thiscall*)(BodyPartManager*, PhysicalObject*, BodyPartManager::ActiveBodyPart::Index, BodyPart*, int)>(0x473CB0);
	void BodyPartManager::setBodyPartForObject(PhysicalObject* object, ActiveBodyPart::Index index, BodyPart* bodyPart, bool isFirstPerson) {
		if (mwse::lua::event::BodyPartAssignedEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::BodyPartAssignedEvent(this, reference, object, index, bodyPart, isFirstPerson));
			if (eventData.valid()) {
				bodyPart = eventData.get_or("bodyPart", bodyPart);
				if (eventData.get_or("block", false)) {
					return;
				}
			}
		}

		TES3_BodyPartManager_setBodyPartForItem(this, object, index, bodyPart, isFirstPerson);
	}

	const auto TES3_BodyPartManager_setBodyPartByIdForItem = reinterpret_cast<void(__thiscall*)(BodyPartManager*, PhysicalObject*, BodyPartManager::ActiveBodyPart::Index, const char*, int)>(0x473C80);
	void BodyPartManager::setBodyPartByIdForObject(PhysicalObject* object, ActiveBodyPart::Index index, const char* bodyPartId, bool isFirstPerson) {
		TES3_BodyPartManager_setBodyPartByIdForItem(this, object, index, bodyPartId, isFirstPerson);
	}

	const auto TES3_BodyPartManager_removeEquippedLayers = reinterpret_cast<void(__thiscall*)(BodyPartManager*)>(0x472D70);
	void BodyPartManager::removeEquippedLayers() {
		TES3_BodyPartManager_removeEquippedLayers(this);
	}

	const auto TES3_BodyPartManager_updateForReference = reinterpret_cast<void(__thiscall*)(BodyPartManager*, Reference*)>(0x473EA0);
	void BodyPartManager::updateForReference(Reference* reference) {
		TES3_BodyPartManager_updateForReference(this, reference);

		// Force update opacity.
		auto mobile = reference->getAttachedMobileActor();
		if (mobile) {
			auto animController = mobile->getAnimationController();
			if (animController && animController->getOpacity() < 1.0f) {
				const auto oldAlpha = animController->getOpacity();
				animController->materialProperty->alpha = -1.0f;
				animController->setOpacity(oldAlpha);
			}
		}
	}

	BodyPartManager::ActiveBodyPart* BodyPartManager::getActiveBodyPart(ActiveBodyPart::Layer layer, ActiveBodyPart::Index index) {
		return &activeBodyParts[int(layer)][int(index)];
	}
}
