#pragma once

#include "CSPhysicalObject.h"

#include "NIObject.h"

namespace se::cs {
	struct Attachment {
		enum class Type : unsigned int {
			Animation = 0,
			BodyPartManager = 1,
			Light = 2,
			Lock = 3,
			LeveledBaseReference = 4,
			TravelDestination = 5,
			Variables = 6,
			LoadDoorBackReference = 7,
			ActorData = 8,
			Action = 9,
			NewOrientation = 10,
		};

		Type type; // 0x0
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
			NI::Vector3 orientationNonAttached; // 0x4
			NI::Vector3 unknown_0x10; // Position-related.
			NI::Vector3 yetAnotherOrientation; // 0x1C
			NI::Vector3 position; // 0x28
			NI::Vector3 undoPosition; // 0x34
		};
		union {
			ReferenceData referenceData; // 0x28
			struct {
				PhysicalObject* baseObject; // 0x28
				NI::Vector3 orientationNonAttached; //0x2C
				NI::Vector3 unknown_0x10; // 0x38
				NI::Vector3 yetAnotherOrientation; // 0x44
				NI::Vector3 position; // 0x50
				NI::Vector3 undoPosition; // 0x5C
			};
		};
		Attachment* firstAttachment; // 0x68
		int unknown_0x6C;
		int unknown_0x70;
		NI::Pointer<NI::AVObject> selectionWidget; // 0x74. NiLines

		Attachment* getAttachment(Attachment::Type type) const;
		LightAttachmentNode* getLightAttachment() const;
		TravelDestination* getTravelDestination() const;
		Reference* getDoorMarkerBackReference() const;

		// Sets reference as modified, sets baseObject's flag 80, and if there is an attachment7 it sets that as modified too.
		void setAsEdited() const;

		void updateRotationMatrixForRaceAndSex(NI::Matrix33& matrix, bool unknown = false) const;

		bool createSelectionWidget(NI::Vector3 boundsMin, NI::Vector3 boundsMax);
		void setSelectionWidgetEnabled(int flag);
		bool hasActiveSelectionWidget() const;

		Cell* getCell() const;
	};
	static_assert(sizeof(Reference) == 0x78, "TES3::Reference failed size validation");
}
