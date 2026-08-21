#pragma once

#include "TES3AttachmentFlags.h"
#include "TES3Defines.h"

#include "NILight.h"

#include "TES3Item.h"
#include "NIPoint3.h"

namespace TES3 {
	struct Attachment {
		AttachmentType::AttachmentType type;
		Attachment * next;

		Attachment() = delete;
		~Attachment() = delete;
	};
	static_assert(sizeof(Attachment) == 0x8, "TES3::Attachment failed size validation");

	template <typename T>
	struct AttachmentWithNode : Attachment {
		T * data;

		AttachmentWithNode() = delete;
		~AttachmentWithNode() = delete;
	};
	static_assert(sizeof(AttachmentWithNode<void>) == 0xC, "TES3::AttachmentWithNode failed size validation");

	//
	// Animation data.
	//

	typedef AttachmentWithNode<AnimationData> AnimationAttachment;

	//
	// Body Part Manager.
	//

	typedef AttachmentWithNode<BodyPartManager> BodyPartManagerAttachment;

	//
	// Lights
	//

	struct LightAttachmentNode {
		NI::Pointer<NI::Light> light; // 0x0
		float flickerPhase; // 0x4

		LightAttachmentNode() = delete;
		~LightAttachmentNode() = delete;

		NI::Light* getLight() const;
		void setLight(NI::Light* light);
	};
	static_assert(sizeof(LightAttachmentNode) == 0x8, "TES3::LightAttachmentNode failed size validation");

	typedef AttachmentWithNode<LightAttachmentNode> LightAttachment;

	//
	// Locks
	//

	struct LockAttachmentNode {
		int lockLevel; // 0x00
		Misc * key; // 0x04
		Spell * trap; // 0x08
		bool locked; // 0x0C

		LockAttachmentNode() = delete;
		~LockAttachmentNode() = delete;

		//
		// Custom functions.
		//

		Misc* getKey() const;
		void setKey(Misc* key);

	};
	static_assert(sizeof(LockAttachmentNode) == 0x10, "TES3::LockAttachmentNode failed size validation");

	typedef AttachmentWithNode<LockAttachmentNode> LockAttachment;

	//
	// Leveled Base Reference
	//

	typedef AttachmentWithNode<Reference> LeveledBaseReferenceAttachment;

	//
	// TravelDestination
	//

	struct TravelDestination {
		Cell * cell; // 0x0
		char * cellName; // 0x4
		Reference * destination; // 0x8

		TravelDestination() = delete;
		~TravelDestination() = delete;
	};
	static_assert(sizeof(TravelDestination) == 0xC, "TES3::TravelDestination failed size validation");

	typedef AttachmentWithNode<TravelDestination> TravelDestinationAttachment;

	//
	// Variables
	//

	typedef AttachmentWithNode<ItemData> ItemDataAttachment;

	//
	// Ownership
	//

	struct OwnershipAttachmentNode {
		int unknown_0x00;
		BaseObject * owner; // 0x04
		union {
			long rank;
			void * variable;
		} rankVar; // 0x08

		OwnershipAttachmentNode() = delete;
		~OwnershipAttachmentNode() = delete;
	};
	static_assert(sizeof(OwnershipAttachmentNode) == 0x0C, "TES3::OwnershipAttachmentNode failed size validation");

	typedef AttachmentWithNode<OwnershipAttachmentNode> OwnershipAttachment;

	//
	// Actor Data / Mobile Object
	//

	typedef AttachmentWithNode<MobileActor> MobileActorAttachment;

	//
	// Action
	//

	struct ActionAttachment : Attachment {
		ActionFlags::Flag flags;
		Reference * reference;

		ActionAttachment() = delete;
		~ActionAttachment() = delete;
	};
	static_assert(sizeof(ActionAttachment) == 0x10, "TES3::ActionAttachment failed size validation");
	
	//
	// New Orientation
	//

	struct NewOrientationAttachment : Attachment {
		NI::Point3 position;
		NI::Point3 orientation;

		NewOrientationAttachment() = delete;
		~NewOrientationAttachment() = delete;
	};
	static_assert(sizeof(NewOrientationAttachment) == 0x20, "TES3::NewOrientationAttachment failed size validation");
}
