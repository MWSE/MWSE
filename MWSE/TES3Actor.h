#pragma once

#include "TES3Defines.h"

#include "TES3Armor.h"
#include "TES3Clothing.h"
#include "TES3Inventory.h"
#include "TES3Object.h"

namespace TES3 {
	namespace ActorFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			IsBase = 0x8,
		};
	}

	struct ActorVirtualTable : PhysicalObjectVirtualTable {
		bool (__thiscall * loadAnimation)(Actor*); // 0x148
		Actor * (__thiscall * getBaseActor)(Actor*); // 0x14C
		int (__thiscall * addItem)(Actor*, Item*, int, bool); // 0x150
		int (__thiscall * getBaseBarterGold)(Actor*); // 0x154
		void (__thiscall * setBaseBarterGold)(Actor*, int); // 0x158
		void (__thiscall * clone)(Actor*, Reference*); // 0x15C
		void (__thiscall * onCloseInventory)(Actor*, Reference*, int); // 0x160
		bool (__thiscall * isGuard)(Actor*); // 0x164
		bool (__thiscall * canSwim)(Actor*); // 0x168
		bool (__thiscall * canWalk)(Actor*); // 0x16C
		bool (__thiscall * canFly)(Actor*); // 0x170
		bool (__thiscall * isOrganic)(Actor*); // 0x174
		AIPackageConfig * (__thiscall * getAIPackageConfig)(Actor*); // 0x178
		void (__thiscall * setAIPackage)(Actor*, AIPackageConfig*, Reference*); // 0x17C
	};
	static_assert(sizeof(ActorVirtualTable) == 0x180, "TES3::ActorVirtualTable failed size validation");

	struct Actor : PhysicalObject {
		ActorAnimationController * animationController; // 0x30
		unsigned int actorFlags; // 0x34
		int cloneCount; // 0x38
		Inventory inventory; // 0x3C
		IteratedList<EquipmentStack*> equipment; // 0x58

		//
		// Function wrappers for our virtual table.
		//

		Actor * getBaseActor();
		int getBaseBarterGold();
		void setBaseBarterGold(int);
		bool isGuard();
		void clone(Reference*);
		void onCloseInventory(Reference* reference, int unknown = 0);
		AIPackageConfig * getAIPackageConfig();
		void setAIPackage(AIPackageConfig* packageConfig, Reference* reference);
		int addItem(Item* item, int count, bool something = false);

		//
		// Other related this-call functions.
		//

		Object* equipItem(Object* item, ItemData* itemData, EquipmentStack** out_equipmentStack, MobileActor* mobileActor);
		EquipmentStack* unequipItem(Object* item, bool deleteStack, MobileActor* mobileActor, bool updateGUI, ItemData* itemData);
		void unequipAllItems(MobileActor * mobileActor);
		void postUnequipUIRefresh(MobileActor* mobileActor);
		EquipmentStack* getEquippedItem(Object* item);
		EquipmentStack* getEquippedItemExact(Object* item, ItemData* itemData);
		EquipmentStack* getEquippedArmorBySlot(ArmorSlot::value_type slot);
		EquipmentStack* getEquippedClothingBySlot(ClothingSlot::value_type slot);

		//
		// Custom functions.
		//

		bool isBaseActor() const;
		bool isClone() const;

		bool tradesItemType(ObjectType::ObjectType type);
		bool offersService(unsigned int service);

		int getBloodType() const;
		void setBloodType(int value);

		void onCloseInventory_lua(TES3::Reference* reference, sol::optional<int> unknown);

		bool hasItemEquipped_lua(sol::object itemOrItemId, sol::optional<TES3::ItemData*> itemData);
	};
	static_assert(sizeof(Actor) == 0x6C, "TES3::Actor failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Actor)
