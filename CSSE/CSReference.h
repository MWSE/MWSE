#pragma once

#include "CSPhysicalObject.h"

#include "NIObject.h"

#include "TES3AttachmentFlags.h"

namespace se::cs {
	struct Attachment {
		TES3::AttachmentType::AttachmentType type; // 0x0
		Attachment* next; // 0x4
	};
	static_assert(sizeof(Attachment) == 0x8, "TES3::Attachment failed size validation");

	template <typename T>
	struct AttachmentWithNode : Attachment {
		T* data; // 0x8
	};
	static_assert(sizeof(AttachmentWithNode<void>) == 0xC, "TES3::AttachmentWithNode failed size validation");

	typedef AttachmentWithNode<Reference*> LoadDoorBackReferenceAttachment;

	struct SecurityAttachmentNode {
		int lockLevel; // 0x0
		Object* key; // 0x4
		Spell* trap; // 0x8
		bool locked; // 0xC
	};
	static_assert(sizeof(SecurityAttachmentNode) == 0x10, "TES3::SecurityAttachmentNode failed size validation");
	typedef AttachmentWithNode<SecurityAttachmentNode> SecurityAttachment;

	struct LightAttachmentNode {
		NI::Pointer<NI::Light> light; // 0x0 // Note: This seems like it may be part of a larger structure.
		float flickerPhase; // 0x4
	};
	static_assert(sizeof(LightAttachmentNode) == 0x8, "TES3::LightAttachmentNode failed size validation");
	typedef AttachmentWithNode<LightAttachmentNode> LightAttachment;

	struct TravelDestination {
		Cell* cell; // 0x0
		char* cellName; // 0x4
		Reference* destination; // 0x8
	};
	static_assert(sizeof(TravelDestination) == 0xC, "TES3::TravelDestination failed size validation");
	typedef AttachmentWithNode<TravelDestination> TravelDestinationAttachment;

	struct Reference : Object {
		struct ReferenceData {
			PhysicalObject* baseObject; // 0x0
			NI::Point3 orientationNonAttached; // 0x4
			NI::Point3 unknown_0x10; // Position-related.
			NI::Point3 yetAnotherOrientation; // 0x1C
			NI::Point3 position; // 0x28
			NI::Point3 undoPosition; // 0x34
		};
		union {
			ReferenceData referenceData; // 0x28
			struct {
				PhysicalObject* baseObject; // 0x28
				NI::Point3 orientationNonAttached; //0x2C
				NI::Point3 unknown_0x10; // 0x38
				NI::Point3 yetAnotherOrientation; // 0x44
				NI::Point3 position; // 0x50
				NI::Point3 undoPosition; // 0x5C
			};
		};
		Attachment* firstAttachment; // 0x68
		int sourceID;
		int targetID; // Master index?
		NI::Pointer<NI::AVObject> selectionWidget; // 0x74. NiLines

		Attachment* getAttachment(TES3::AttachmentType::AttachmentType type) const;
		LightAttachmentNode* getLightAttachment() const;
		TravelDestination* getTravelDestination() const;
		Reference* getDoorMarkerBackReference() const;

		// Sets reference as modified, sets baseObject's flag 80, and if there is an attachment7 it sets that as modified too.
		void setAsEdited() const;

		void updateRotationMatrixForRaceAndSex(NI::Matrix33& matrix, bool unknown = false) const;

		bool createSelectionWidget(NI::Point3 boundsMin, NI::Point3 boundsMax);
		void setSelectionWidgetEnabled(int flag);
		bool hasActiveSelectionWidget() const;

		Cell* getCell() const;

		std::string getUniqueID() const;
	};
	static_assert(sizeof(Reference) == 0x78, "TES3::Reference failed size validation");
}
