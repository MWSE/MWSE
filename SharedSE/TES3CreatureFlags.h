#pragma once

namespace TES3 {
	namespace ActorFlagCreature {
		typedef unsigned int value_type;

		enum Flag : value_type {
			Biped = 0x1,
			Respawn = 0x2,
			WeaponAndShield = 0x4,
			IsBase = 0x8,
			Swims = 0x10,
			Flies = 0x20,
			Walks = 0x40,
			Essential = 0x80,

			// Blood is stored 10 bits in, and stored in 3 bits for 8 total options.
			BloodMask = 0x1C00,

			DefaultFlags = Walks | IsBase,
		};

		enum FlagBit {
			BipedBit = 0,
			RespawnBit = 1,
			WeaponAndShieldBit = 2,
			IsBaseBit = 3,
			SwimsBit = 4,
			FliesBit = 5,
			WalksBit = 6,
			EssentialBit = 7,

			BloodBitsFirst = 10,
			BloodBitsLast = 12,
		};
	}

	namespace CreatureType {
		enum CreatureType {
			Normal,
			Daedra,
			Undead,
			Humanoid
		};
	}

	namespace CreatureSkill {
		enum CreatureSkill {
			Combat,
			Magic,
			Stealth
		};
	}
}
