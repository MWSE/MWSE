#pragma once

namespace TES3 {
	namespace ObjectFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			FromMaster = 0x1,
			Modified = 0x2,
			LinksResolved = 0x8,
			NoCollision = 0x10,
			Delete = 0x20,
			Persistent = 0x400,
			Disabled = 0x800,
			SelectedByConsole = 0x1000,
			Blocked = 0x2000,
			EmptyInventory = 0x2000,
			ScaleModifiedToOne = 0x8000
		};

		enum FlagBit {
			FromMasterBit = 0,
			ModifiedBit = 1,
			LinksResolvedBit = 3,
			NoCollisionBit = 4,
			DeleteBit = 5,
			PersistentBit = 10,
			DisabledBit = 11,
			SelectedByConsoleBit = 12,
			BlockedBit = 13,
			EmptyInventoryBit = 13,
			ScaleModifiedToOneBit = 15,
		};
	}
}
