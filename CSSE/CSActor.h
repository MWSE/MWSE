#pragma once

#include "CSAnimatedObject.h"

#include "NIIteratedList.h"
#include "NITArray.h"

namespace se::cs {
	namespace ActorFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			IsBase = 0x8,
		};
	}

	struct ItemData {
		int count; // 0x0
		BaseObject* owner; // 0x4
		union {
			long requiredRank;
			GlobalVariable* requiredVariable;
		}; // 0x8
		union {
			int condition;
			float timeLeft;
		}; // 0xC
		union {
			float charge;
			Actor* soul;
		}; // 0x10
		Script* script; // 0x14
		void* scriptData; // 0x18
	};

	struct ItemStack {
		int count; // 0x0
		Object* object; // 0x4
		NI::TArray<ItemData*>* variables; // 0x8
	};

	struct Inventory {
		unsigned int flags; // 0x0
		NI::IteratedList<ItemStack*> items; // 0x4
		Light* internalLight; // 0x18
	};
	static_assert(sizeof(Inventory) == 0x1C, "Inventory failed size validation");

	struct Actor_VirtualTable : Object_VirtualTable {
		void* unknown_0x138;
		void* unknown_0x13C;
		void* unknown_0x140;
		void* unknown_0x144;
		void* unknown_0x148;
		void* unknown_0x14C;
		void* unknown_0x150;
		bool(__thiscall* hasMovementSwim)(const Object*); // 0x154
		bool(__thiscall* hasMovementWalk)(const Object*); // 0x154
		bool(__thiscall* hasMovementFly)(const Object*); // 0x154
		void* unknown_0x160;
		void* unknown_0x164;
		void* unknown_0x168;
	};
	static_assert(sizeof(Actor_VirtualTable) == 0x16C, "Actor virtual table failed size validation");

	struct Actor : AnimatedObject {
		unsigned int actorFlags; // 0x4C
		int unknown_0x50;
		Inventory inventory; // 0x54
		void* equipment; // 0x70
		int unknown_0x74;
		int unknown_0x78;

		inline bool hasMovementSwim() const {
			return vtbl.actor->hasMovementSwim(this);
		}

		inline bool hasMovementWalk() const {
			return vtbl.actor->hasMovementWalk(this);
		}

		inline bool hasMovementFly() const {
			return vtbl.actor->hasMovementFly(this);
		}
	};
	static_assert(sizeof(Actor) == 0x7C, "CS::Actor failed size validation");
}