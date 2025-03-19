#include "CSReference.h"

#include "NINode.h"

namespace se::cs {
	Attachment* Reference::getAttachment(Attachment::Type type) const {
		auto attachment = firstAttachment;
		while (attachment) {
			if (attachment->type == type) {
				return attachment;
			}
			attachment = attachment->next;
		}
		return nullptr;
	}

	LightAttachmentNode* Reference::getLightAttachment() const {
		const auto Reference_getLightAttachment = reinterpret_cast<LightAttachmentNode*(__thiscall*)(const Reference*)>(0x4043EA);
		return Reference_getLightAttachment(this);
	}

	TravelDestination* Reference::getTravelDestination() const {
		const auto Reference_getTravelDestination = reinterpret_cast<TravelDestination * (__thiscall*)(const Reference*)>(0x53D950);
		return Reference_getTravelDestination(this);
	}

	Reference* Reference::getDoorMarkerBackReference() const {
		const auto Reference_getAttachment7 = reinterpret_cast<Reference * (__thiscall*)(const Reference*)>(0x53F3B0);
		return Reference_getAttachment7(this);
	}

	void Reference::setAsEdited() const {
		const auto Reference_setModifiedUpdateBaseObjectAndAttachment7 = reinterpret_cast<void(__thiscall*)(const Reference*)>(0x4026E4);
		Reference_setModifiedUpdateBaseObjectAndAttachment7(this);
	}

	void Reference::updateRotationMatrixForRaceAndSex(NI::Matrix33& matrix, bool unknown) const {
		const auto Reference_updateRotationMatrixForRaceAndSex = reinterpret_cast<void(__thiscall*)(const Reference*, NI::Matrix33*, bool)>(0x4028B0);
		return Reference_updateRotationMatrixForRaceAndSex(this, &matrix, unknown);
	}

	bool Reference::createSelectionWidget(NI::Vector3 boundsMin, NI::Vector3 boundsMax) {
		const auto Reference_createSelectionWidget = reinterpret_cast<bool(__thiscall*)(Reference*, NI::Vector3, NI::Vector3)>(0x540D50);

		if (!Reference_createSelectionWidget(this, boundsMin, boundsMax)) {
			return false;
		}

		// Create a new wrapper node for the selection bounding box widget.
		auto newNode = NI::Node::create();
		auto originalParent = selectionWidget->parentNode;
		newNode->attachChild(selectionWidget.get());
		if (originalParent) {
			originalParent->attachChild(selectionWidget);
		}
		selectionWidget = newNode;

		return true;
	}

	void Reference::setSelectionWidgetEnabled(int flag) {
		const auto Reference_setSelectionWidgetEnabled = reinterpret_cast<void(__thiscall*)(Reference*, int)>(0x540C60);
		Reference_setSelectionWidgetEnabled(this, flag);
	}

	bool Reference::hasActiveSelectionWidget() const {
		if (selectionWidget == nullptr || sceneNode == nullptr) {
			return false;
		}

		return selectionWidget->parentNode == sceneNode;
	}

	Cell* Reference::getCell() const {
		const auto Reference_getCell = reinterpret_cast<Cell*(__thiscall*)(const Reference*)>(0x401B0E);
		return Reference_getCell(this);
	}
}
