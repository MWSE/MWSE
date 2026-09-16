#pragma once

namespace TES3 {
	namespace AttachmentType {
		enum AttachmentType {
			Animation = 0x0,
			BodyPartManager = 0x1,
			Light = 0x2,
			Lock = 0x3,
			LeveledBaseReference = 0x4,
			TravelDestination = 0x5,
			Variables = 0x6,
			ActorData = 0x8,
			Action = 0x9,
			NewOrientation = 0xA
		};
	}

	namespace ActionFlags {
		typedef unsigned int value_type;

		enum Flag : value_type {
			UseEnabled = 0x1,
			OnActivate = 0x2,
			OnDeath = 0x10,
			OnKnockout = 0x20,
			OnMurder = 0x40,
			DoorOpening = 0x100,
			DoorClosing = 0x200,
			DoorJammedOpening = 0x400,
			DoorJammedClosing = 0x800
		};
	}
}
