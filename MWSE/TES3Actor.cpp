#include "TES3Actor.h"

#include "TES3AIConfig.h"
#include "TES3Class.h"
#include "TES3MobileActor.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "LuaContainerClosedEvent.h"
#include "LuaEquippedEvent.h"
#include "LuaUnequippedEvent.h"

namespace TES3 {
	const auto TES3_Actor_equipItem = reinterpret_cast<Object* (__thiscall*)(Actor*, Object*, ItemData*, EquipmentStack**, MobileActor*)>(0x4958B0);
	const auto TES3_Actor_unequipItem = reinterpret_cast<EquipmentStack* (__thiscall*)(Actor*, Object*, bool, MobileActor*, bool, ItemData*)>(0x496710);
	const auto TES3_Actor_getEquippedArmorBySlot = reinterpret_cast<EquipmentStack* (__thiscall*)(Actor*, ArmorSlot::value_type)>(0x496E60);
	const auto TES3_Actor_getEquippedClothingBySlot = reinterpret_cast<EquipmentStack* (__thiscall*)(Actor*, ClothingSlot::value_type)>(0x496E00);
	const auto TES3_Actor_getEquippedItem = reinterpret_cast<EquipmentStack* (__thiscall*)(Actor*, Object*)>(0x496DD0);
	const auto TES3_Actor_getEquippedItemExact = reinterpret_cast<EquipmentStack* (__thiscall*)(Actor*, Object*, ItemData*)>(0x496D90);

	Actor * Actor::getBaseActor() {
		return vTable.actor->getBaseActor(this);
	}

	int Actor::getBaseBarterGold() {
		return vTable.actor->getBaseBarterGold(this);
	}

	void Actor::setBaseBarterGold(int value) {
		vTable.actor->setBaseBarterGold(this, value);
	}

	bool Actor::isGuard() {
		return vTable.actor->isGuard(this);
	}

	void Actor::clone(Reference* reference) {
		vTable.actor->clone(this, reference);
	}

	void Actor::onCloseInventory(Reference* reference, int unknown) {
		// Trigger or queue our event.
		if (mwse::lua::event::ContainerClosedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::ContainerClosedEvent(reference));
		}
	}

	AIPackageConfig * Actor::getAIPackageConfig() {
		return vTable.actor->getAIPackageConfig(this);
	}

	void Actor::setAIPackage(AIPackageConfig* packageConfig, Reference* reference) {
		vTable.actor->setAIPackage(this, packageConfig, reference);
	}

	int Actor::addItem(Item* item, int count, bool something) {
		return vTable.actor->addItem(this, item, count, something);
	}

	Object* Actor::equipItem(Object* item, ItemData* itemData, EquipmentStack** out_equipmentStack, MobileActor* mobileActor) {
		Object* result = TES3_Actor_equipItem(this, item, itemData, out_equipmentStack, mobileActor);

		// Trigger or queue our event, if this actor has a mobile actor.
		if (mobileActor && mwse::lua::event::EquippedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::EquippedEvent(this, mobileActor, item, itemData));
		}

		return result;
	}

	EquipmentStack* Actor::unequipItem(Object* item, bool deleteStack, MobileActor* mobileActor, bool updateGUI, ItemData* itemData) {
		EquipmentStack* result = TES3_Actor_unequipItem(this, item, deleteStack, mobileActor, updateGUI, itemData);

		// Trigger or queue our event if there's an attached mobile actor.
		if (mobileActor && mwse::lua::event::UnequippedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::UnequippedEvent(this, mobileActor, item, itemData));
		}

		return result;
	}

	const auto TES3_Actor_unequipAllItems = reinterpret_cast<void(__thiscall*)(Actor*, MobileActor*)>(0x496680);
	void Actor::unequipAllItems(MobileActor * mobileActor) {
		TES3_Actor_unequipAllItems(this, mobileActor);
	}

	void Actor::postUnequipUIRefresh(MobileActor* mobileActor) {
		// UI refresh code from the tail of TES3_Actor_unequipItem
		// Required to work around a crashing bug with unequipping lights
		const auto TES3_ui_inventoryUpdateIcons = reinterpret_cast<void (__cdecl*)()>(0x5CC910);
		const auto TES3_ui_inventoryUpdateWindowTitle = reinterpret_cast<void (__cdecl*)()>(0x5CE080);
		const auto TES3_ui_updateCharacterImage = reinterpret_cast<void (__cdecl*)(bool)>(0x5CD2A0);
		const auto TES3_ui_updateMagicMenu = reinterpret_cast<void (__cdecl*)()>(0x5E2E80);

		if (mobileActor->actorType == MobileActorType::Player) {
			auto player = static_cast<MobilePlayer*>(mobileActor);

			if (player->actorFlags & MobileActorFlag::BodypartsChanged) {
				player->reference->updateBipedParts();
				player->firstPersonReference->updateBipedParts();
				player->updateOpacity();
				player->actorFlags &= ~MobileActorFlag::BodypartsChanged;
			}

			TES3_ui_inventoryUpdateIcons();
			TES3_ui_inventoryUpdateWindowTitle();
			TES3_ui_updateCharacterImage(false);
			TES3_ui_updateMagicMenu();
		}
	}

	EquipmentStack* Actor::getEquippedItem(Object* item) {
		return TES3_Actor_getEquippedItem(this, item);
	}

	EquipmentStack* Actor::getEquippedItemExact(Object* item, ItemData* itemData) {
		return TES3_Actor_getEquippedItemExact(this, item, itemData);
	}

	EquipmentStack* Actor::getEquippedArmorBySlot(ArmorSlot::value_type slot) {
		return TES3_Actor_getEquippedArmorBySlot(this, slot);
	}

	EquipmentStack* Actor::getEquippedClothingBySlot(ClothingSlot::value_type slot) {
		return TES3_Actor_getEquippedClothingBySlot(this, slot);
	}

	bool Actor::isBaseActor() const {
		return (actorFlags & TES3::ActorFlag::IsBase);
	}

	bool Actor::isClone() const {
		return !(actorFlags & TES3::ActorFlag::IsBase);
	}

	bool Actor::tradesItemType(ObjectType::ObjectType type) {
		auto config = getAIConfig();

		switch (type) {
		case TES3::ObjectType::Alchemy:
			return (config->merchantFlags & TES3::ServiceFlag::BartersAlchemy);
		case TES3::ObjectType::Apparatus:
			return (config->merchantFlags & TES3::ServiceFlag::BartersApparatus);
		case TES3::ObjectType::Armor:
			return (config->merchantFlags & TES3::ServiceFlag::BartersArmor);
		case TES3::ObjectType::Book:
			return (config->merchantFlags & TES3::ServiceFlag::BartersBooks);
		case TES3::ObjectType::Clothing:
			return (config->merchantFlags & TES3::ServiceFlag::BartersClothing);
		case TES3::ObjectType::Ingredient:
			return (config->merchantFlags & TES3::ServiceFlag::BartersIngredients);
		case TES3::ObjectType::Light:
			return (config->merchantFlags & TES3::ServiceFlag::BartersLights);
		case TES3::ObjectType::Lockpick:
			return (config->merchantFlags & TES3::ServiceFlag::BartersLockpicks);
		case TES3::ObjectType::Misc:
			return (config->merchantFlags & TES3::ServiceFlag::BartersMiscItems);
		case TES3::ObjectType::Probe:
			return (config->merchantFlags & TES3::ServiceFlag::BartersProbes);
		case TES3::ObjectType::Repair:
			return (config->merchantFlags & TES3::ServiceFlag::BartersRepairTools);
		case TES3::ObjectType::Weapon:
		case TES3::ObjectType::Ammo:
			return (config->merchantFlags & TES3::ServiceFlag::BartersWeapons);
		}

		return false;
	}

	bool Actor::offersService(unsigned int service) {
		auto config = getAIConfig();
		return (config->merchantFlags & service);
	}

	int Actor::getBloodType() const {
		return (actorFlags >> 0xA) & 0x7;
	}

	void Actor::setBloodType(int value) {
		if (value < 0 || value > 7) {
			throw std::invalid_argument("Type must be between 0 and 7.");
		}

		// Clear blood flags.
		actorFlags &= ~0x1C00;
		actorFlags |= (value << 0xA);
	}

	void Actor::onCloseInventory_lua(TES3::Reference* reference, sol::optional<int> unknown) {
		vTable.actor->onCloseInventory(this, reference, unknown.value_or(0));
	}

	bool Actor::hasItemEquipped_lua(sol::object itemOrItemId, sol::optional<TES3::ItemData*> itemData) {
		TES3::Item* item = nullptr;
		
		if (itemOrItemId.is<TES3::Item*>()) {
			item = itemOrItemId.as<TES3::Item*>();
		}
		else if (itemOrItemId.is<const char*>()) {
			auto itemId = itemOrItemId.as<const char*>();
			auto ndd = TES3::DataHandler::get()->nonDynamicData;
			item = ndd->resolveObjectByType<TES3::Item>(itemId);
		}

		if (item == nullptr) {
			return false;
		}
		if (itemData.has_value()) {
			return getEquippedItemExact(item, itemData.value()) != nullptr;
		}
		else {
			return getEquippedItem(item) != nullptr;
		}
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Actor)
