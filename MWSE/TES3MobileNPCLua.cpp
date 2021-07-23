#include "TES3MobileNPCLua.h"

#include "TES3MobileActorLua.h"

#include "LuaManager.h"

#include "TES3Actor.h"
#include "TES3Alchemy.h"
#include "TES3Enchantment.h"
#include "TES3MobileNPC.h"
#include "TES3NPC.h"
#include "TES3Skill.h"
#include "TES3Spell.h"

namespace mwse {
	namespace lua {
		static bool equip(TES3::MobileNPC& self, sol::object arg) {
			if (arg.is<TES3::Item>()) {
				return self.equipItem(arg.as<TES3::Item*>());
			}

			sol::table params = arg;

			TES3::Item * item = getOptionalParamObject<TES3::Item>(params, "item");
			if (item == nullptr) {
				return false;
			}

			auto itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);
			auto addItem = getOptionalParam<bool>(params, "addItem", false);
			auto selectBestCondition = getOptionalParam<bool>(params, "selectBestCondition", false);
			auto selectWorstCondition = getOptionalParam<bool>(params, "selectWorstCondition", false);

			return self.equipItem(item, itemData, addItem, selectBestCondition, selectWorstCondition);
		}

		static bool unequip(TES3::MobileNPC& self, sol::table args) {
			TES3::Actor* actor = static_cast<TES3::Actor*>(self.reference->baseObject);
			TES3::EquipmentStack* s = nullptr;
			auto item = getOptionalParamObject<TES3::Item>(args, "item");
			int type = args.get_or("type", 0);
			int armourSlot = args.get_or("armorSlot", -1);
			int clothingSlot = args.get_or("clothingSlot", -1);

			if (item) {
				// Match by item
				s = actor->getEquippedItem(item);
			}
			else if (armourSlot != -1) {
				// Match by slot
				s = actor->getEquippedArmorBySlot(armourSlot);
			}
			else if (clothingSlot != -1) {
				// Match by slot
				s = actor->getEquippedClothingBySlot(clothingSlot);
			}
			if (s) {
				// Warning: Unequipping lights during menumode with updateGUI=true triggers a Morrowind crash.
				// UI update has been moved to a separate function.
				actor->unequipItem(s->object, 1, &self, 0, s->variables);
				actor->postUnequipUIRefresh(&self);
				return true;
			}

			if (type) {
				// Match all by objectType
				std::vector<TES3::EquipmentStack*> matches;
				for (auto it = actor->equipment.head; it; it = it->next) {
					if (it->data->object->objectType == type) {
						matches.push_back(it->data);
					}
				}
				// Warning: Unequipping lights during menumode with updateGUI=true triggers a Morrowind crash.
				// UI update has been moved to a separate function.
				for (auto it : matches) {
					s = it;
					actor->unequipItem(s->object, 1, &self, 0, s->variables);
				}
				actor->postUnequipUIRefresh(&self);
			}
			return bool(s);
		}

		void bindTES3MobileNPC() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::MobileNPC>("tes3mobileNPC");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::MobileActor, TES3::MobileObject>();

			// All binding is in the NPC function, so that mobile player doesn't have extra lookup times.
			setUserdataForTES3MobileNPC(usertypeDefinition);

			// Basic function binding.
			usertypeDefinition["equip"] = &equip;
			usertypeDefinition["unequip"] = &unequip;
		}
	}
}
