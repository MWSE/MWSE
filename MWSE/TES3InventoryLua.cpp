#include "TES3InventoryLua.h"

#include "sol.hpp"
#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3CollectionsLua.h"
#include "TES3ScriptLua.h"

#include "TES3Inventory.h"
#include "TES3ItemData.h"
#include "TES3GlobalVariable.h"
#include "TES3Creature.h"
#include "TES3Script.h"

namespace TES3 {
	sol::object ItemStack::getObjectLua() {
		return mwse::lua::makeLuaObject(object);
	}

	sol::object EquipmentStack::getObjectLua() {
		return mwse::lua::makeLuaObject(object);
	}
}

namespace mwse {
	namespace lua {
		sol::object getItemDataOwner(TES3::ItemData& itemData) {
			return makeLuaObject(itemData.owner);
		}

		void setItemDataOwner(TES3::ItemData& itemData, sol::object value) {
			if (value.is<TES3::BaseObject*>()) {
				TES3::BaseObject * newOwner = value.as<TES3::BaseObject*>();

				// We only support NPC and Faction owners.
				if (newOwner->objectType != TES3::ObjectType::NPC && newOwner->objectType != TES3::ObjectType::Faction) {
					throw std::exception("Ownership must be an NPC or a faction.");
				}

				// If the owner type is changing, reset the requirement.
				if (itemData.owner->objectType != newOwner->objectType) {
					itemData.requiredVariable = nullptr;
				}

				itemData.owner = newOwner;
			}
		}

		sol::object getItemDataOwnerRequirement(TES3::ItemData& itemData) {
			if (itemData.owner == nullptr) {
				return sol::nil;
			}
			else if (itemData.owner->objectType == TES3::ObjectType::Faction) {
				return sol::make_object(LuaManager::getInstance().getState(), itemData.requiredRank);
			}
			else if (itemData.owner->objectType == TES3::ObjectType::NPC) {
				return makeLuaObject(itemData.requiredVariable);
			}

			return sol::nil;
		}

		void setItemDataOwnerRequirement(TES3::ItemData& itemData, sol::object value) {
			if (itemData.owner == nullptr) {
				throw std::exception("An owner must be set before the requirement can be set.");
			}
			else if (itemData.owner->objectType == TES3::ObjectType::Faction) {
				if (value.is<double>()) {
					itemData.requiredRank = (int)value.as<double>();
				}
				else {
					throw std::exception("Faction ownership used. Requirement must be a rank (number).");
				}
			}
			else if (itemData.owner->objectType == TES3::ObjectType::NPC) {
				if (value.is<TES3::GlobalVariable*>()) {
					itemData.requiredVariable = value.as<TES3::GlobalVariable*>();
				}
				else {
					throw std::exception("NPC ownership used. Requirement must be a global variable.");
				}
			}
		}

		sol::object getItemDataSoul(TES3::ItemData& itemData) {
			// Make our best attempt at making sure this actually has a soul. This is not reliable!
			if (itemData.charge == -1.0f) {
				return sol::nil;
			}

			return makeLuaObject(itemData.soul);
		}

		void setItemDataSoul(TES3::ItemData& itemData, sol::object soul) {
			// Make our best attempt at making sure this actually has a soul. This is not reliable!
			if (itemData.charge == -1.0f) {
				return;
			}
			else if (soul.is<TES3::Creature*>()) {
				itemData.soul = soul.as<TES3::Creature*>();
			}
			else if (soul.is<TES3::CreatureInstance*>()) {
				itemData.soul = soul.as<TES3::CreatureInstance*>()->baseCreature;
			}
		}

		void bindTES3Inventory() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Binding for TES3::ItemData
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::ItemData>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("charge", &TES3::ItemData::charge);
				usertypeDefinition.set("count", &TES3::ItemData::count);
				usertypeDefinition.set("condition", &TES3::ItemData::condition);
				usertypeDefinition.set("scriptVariables", &TES3::ItemData::scriptData);
				usertypeDefinition.set("timeLeft", &TES3::ItemData::timeLeft);

				// Access to other objects that need to be packaged.
				usertypeDefinition.set("script", sol::readonly_property([](TES3::ItemData& self) { return makeLuaObject(self.script); }));

				// Complex properties that need special handling.
				usertypeDefinition.set("owner", sol::property(&getItemDataOwner, &setItemDataOwner));
				usertypeDefinition.set("requirement", sol::property(&getItemDataOwnerRequirement, &setItemDataOwnerRequirement));
				usertypeDefinition.set("soul", sol::property(&getItemDataSoul, &setItemDataSoul));

				// 
				usertypeDefinition.set("data", sol::property(&TES3::ItemData::getOrCreateLuaDataTable, &TES3::ItemData::setLuaDataTable));

				// Add the ability to get the unique script context from this itemdata for ease of mwscript interaction.
				usertypeDefinition.set("context", sol::readonly_property([](TES3::ItemData& self) { return std::shared_ptr<ScriptContext>(new ScriptContext(self.script, self.scriptData)); }));

				// Finish up our usertype.
				state.set_usertype("tes3itemData", usertypeDefinition);
			}

			// Binding for TES3::ItemStack
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::ItemStack>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("count", &TES3::ItemStack::count);
				usertypeDefinition.set("variables", &TES3::ItemStack::variables);

				// Access to other objects that need to be packaged.
				usertypeDefinition.set("object", sol::readonly_property([](TES3::ItemStack& self) { return makeLuaObject(self.object); }));

				// Finish up our usertype.
				state.set_usertype("tes3itemStack", usertypeDefinition);
			}

			// Binding for TES3::EquipmentStack
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::EquipmentStack>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Basic property binding.
				usertypeDefinition.set("variables", &TES3::EquipmentStack::variables);

				// Access to other objects that need to be packaged.
				usertypeDefinition.set("object", sol::readonly_property([](TES3::EquipmentStack& self) { return makeLuaObject(self.object); }));

				// Finish up our usertype.
				state.set_usertype("tes3equipmentStack", usertypeDefinition);
			}

			// Binding for TES3::Inventory
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.create_simple_usertype<TES3::Inventory>();
				usertypeDefinition.set("new", sol::no_constructor);

				// Metamethod binding.
				usertypeDefinition.set(sol::meta_function::pairs, [](TES3::Inventory& self) {
					Iterator_state<TES3::ItemStack> it_state(&self.iterator);
					return std::make_tuple(&bindIterator_pairsNext<TES3::ItemStack>, sol::user<Iterator_state<TES3::ItemStack>>(std::move(it_state)), sol::lua_nil);
				});
				usertypeDefinition.set(sol::meta_function::length, [](TES3::Inventory& self) {
					return self.iterator.size;
				});

				// Basic property binding.
				usertypeDefinition.set("flags", sol::readonly_property(&TES3::Inventory::flags));
				usertypeDefinition.set("iterator", sol::readonly_property(&TES3::Inventory::iterator));

				// Basic function binding.
				usertypeDefinition.set("addItem", [](TES3::Inventory& self, sol::table params) {
					TES3::MobileActor * mact = getOptionalParamMobileActor(params, "mobile");
					TES3::Item * item = getOptionalParamObject<TES3::Item>(params, "item");
					int count = getOptionalParam<int>(params, "count", 1);
					TES3::ItemData * itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);
					self.addItem(mact, item, count, false, itemData ? &itemData : nullptr);
				});
				usertypeDefinition.set("contains", &TES3::Inventory::containsItem);
				usertypeDefinition.set("dropItem", &TES3::Inventory::dropItem);
				usertypeDefinition.set("calculateWeight", &TES3::Inventory::calculateContainedWeight);
				usertypeDefinition.set("removeItem", [](TES3::Inventory& self, sol::table params) {
					TES3::MobileActor * mact = getOptionalParamMobileActor(params, "mobile");
					TES3::Item * item = getOptionalParamObject<TES3::Item>(params, "item");
					int count = getOptionalParam<int>(params, "count", 1);
					TES3::ItemData * itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);
					bool deleteItemData = getOptionalParam<bool>(params, "deleteItemData", false);
					self.removeItemWithData(mact, item, itemData, count, deleteItemData);
				});
				usertypeDefinition.set("resolveLeveledItems", [](TES3::Inventory& self, sol::optional<TES3::MobileActor*> actor) { self.resolveLeveledLists(actor.value_or(nullptr)); });

				// Finish up our usertype.
				state.set_usertype("tes3inventory", usertypeDefinition);
			}
		}
	}
}
