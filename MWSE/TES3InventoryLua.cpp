#include "TES3InventoryLua.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3Actor.h"
#include "TES3Script.h"
#include "TES3Spell.h"

#include "TES3ScriptLua.h"

#include "TES3Inventory.h"
#include "TES3ItemData.h"

namespace mwse::lua {
	void bindTES3Inventory() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::QuickKey
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::QuickKey>("tes3quickKey");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["type"] = sol::readonly_property(&TES3::QuickKey::type);
			usertypeDefinition["spell"] = sol::readonly_property(&TES3::QuickKey::spell);
			usertypeDefinition["item"] = sol::readonly_property(&TES3::QuickKey::item);
			usertypeDefinition["itemData"] = sol::readonly_property(&TES3::QuickKey::itemData);

			// Basic function binding.
			usertypeDefinition["clear"] = &TES3::QuickKey::clear;
			usertypeDefinition["getMagic"] = &TES3::QuickKey::getMagic;
			usertypeDefinition["setMagic"] = &TES3::QuickKey::setMagic;
			usertypeDefinition["getItem"] = &TES3::QuickKey::getItem;
			usertypeDefinition["setItem"] = &TES3::QuickKey::setItem;
		}

		// Binding for TES3::ItemData
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ItemData>("tes3itemData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["charge"] = &TES3::ItemData::charge;
			usertypeDefinition["count"] = &TES3::ItemData::count;
			usertypeDefinition["condition"] = &TES3::ItemData::condition;
			usertypeDefinition["script"] = &TES3::ItemData::script;
			usertypeDefinition["scriptVariables"] = &TES3::ItemData::scriptData;
			usertypeDefinition["timeLeft"] = &TES3::ItemData::timeLeft;

			// Complex properties that need special handling.
			usertypeDefinition["owner"] = sol::property(&TES3::ItemData::getOwner, &TES3::ItemData::setOwner_lua);
			usertypeDefinition["requirement"] = sol::property(&TES3::ItemData::getOwnerRequirement_lua, &TES3::ItemData::setOwnerRequirement_lua);
			usertypeDefinition["soul"] = sol::property(&TES3::ItemData::getSoul, &TES3::ItemData::setSoul_lua);

			// 
			usertypeDefinition["data"] = sol::property(&TES3::ItemData::getOrCreateLuaDataTable, &TES3::ItemData::setLuaDataTable);
			usertypeDefinition["tempData"] = sol::property(&TES3::ItemData::getOrCreateLuaTempDataTable, &TES3::ItemData::setLuaTempDataTable);

			// Add the ability to get the unique script context from this itemdata for ease of mwscript interaction.
			usertypeDefinition["context"] = sol::readonly_property(&TES3::ItemData::createContext);
		}

		// Binding for TES3::ItemStack
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ItemStack>("tes3itemStack");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["count"] = &TES3::ItemStack::count;
			usertypeDefinition["object"] = sol::readonly_property(&TES3::ItemStack::object);
			usertypeDefinition["variables"] = &TES3::ItemStack::variables;
		}

		// Binding for TES3::EquipmentStack
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::EquipmentStack>("tes3equipmentStack");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["itemData"] = &TES3::EquipmentStack::itemData;
			usertypeDefinition["object"] = sol::readonly_property(&TES3::EquipmentStack::object);

			// Legacy bindings.
			usertypeDefinition["variables"] = &TES3::EquipmentStack::itemData;
		}

		// Binding for TES3::Inventory
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Inventory>("tes3inventory");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["flags"] = sol::readonly_property(&TES3::Inventory::flags);
			usertypeDefinition["items"] = sol::readonly_property(&TES3::Inventory::itemStacks);

			// Basic function binding.
			usertypeDefinition["addItem"] = &TES3::Inventory::addItem_lua;
			usertypeDefinition["calculateWeight"] = &TES3::Inventory::calculateContainedWeight;
			usertypeDefinition["contains"] = &TES3::Inventory::contains_lua;
			usertypeDefinition["dropItem"] = &TES3::Inventory::dropItem;
			usertypeDefinition["findItemStack"] = &TES3::Inventory::findItemStack_lua;
			usertypeDefinition["getItemCount"] = &TES3::Inventory::getItemCount_lua;
			usertypeDefinition["removeItem"] = &TES3::Inventory::removeItem_lua;
			usertypeDefinition["resolveLeveledItems"] = &TES3::Inventory::resolveLeveledLists_lua;

			// Legacy bindings.
			usertypeDefinition["iterator"] = sol::readonly_property(&TES3::Inventory::itemStacks);
		}
	}
}
