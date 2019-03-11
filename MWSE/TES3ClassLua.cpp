#include "TES3ClassLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Class.h"

namespace TES3 {
	sol::table Class::getMajorSkills() {
		sol::table result = mwse::lua::LuaManager::getInstance().createTable();
		for (int i = 0; i < 5; i++) {
			result[i + 1] = &skills[i * 2];
		}
		return result;
	}

	sol::table Class::getMinorSkills() {
		sol::table result = mwse::lua::LuaManager::getInstance().createTable();
		for (int i = 0; i < 5; i++) {
			result[i + 1] = &skills[i * 2 + 1];
		}
		return result;
	}
}

namespace mwse {
	namespace lua {
		void bindTES3Class() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.create_simple_usertype<TES3::Class>();
			usertypeDefinition.set("new", sol::no_constructor);

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition.set(sol::base_classes, sol::bases<TES3::BaseObject>());
			setUserdataForBaseObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition.set("services", &TES3::Class::services);
			usertypeDefinition.set("specialization", &TES3::Class::specialization);

			// Indirect bindings to unions and arrays.
			usertypeDefinition.set("attributes", sol::readonly_property([](TES3::Class& self) { return std::ref(self.attributes); }));
			usertypeDefinition.set("skills", sol::readonly_property([](TES3::Class& self) { return std::ref(self.skills); }));

			// Provide friendly access to service flags.
			usertypeDefinition.set("bartersAlchemy", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersAlchemy); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersAlchemy, set); }
			));
			usertypeDefinition.set("bartersApparatus", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersApparatus); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersApparatus, set); }
			));
			usertypeDefinition.set("bartersArmor", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersArmor); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersArmor, set); }
			));
			usertypeDefinition.set("bartersBooks", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersBooks); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersBooks, set); }
			));
			usertypeDefinition.set("bartersClothing", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersClothing); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersClothing, set); }
			));
			usertypeDefinition.set("bartersEnchantedItems", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersEnchantedItems); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersEnchantedItems, set); }
			));
			usertypeDefinition.set("bartersIngredients", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersIngredients); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersIngredients, set); }
			));
			usertypeDefinition.set("bartersLights", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersLights); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersLights, set); }
			));
			usertypeDefinition.set("bartersLockpicks", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersLockpicks); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersLockpicks, set); }
			));
			usertypeDefinition.set("bartersMiscItems", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersMiscItems); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersMiscItems, set); }
			));
			usertypeDefinition.set("bartersProbes", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersProbes); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersProbes, set); }
			));
			usertypeDefinition.set("bartersRepairTools", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersRepairTools); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersRepairTools, set); }
			));
			usertypeDefinition.set("bartersWeapons", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::BartersWeapons); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::BartersWeapons, set); }
			));
			usertypeDefinition.set("offersEnchanting", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::OffersEnchanting); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::OffersEnchanting, set); }
			));
			usertypeDefinition.set("offersRepairs", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::OffersRepairs); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::OffersRepairs, set); }
			));
			usertypeDefinition.set("offersSpellmaking", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::OffersSpellmaking); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::OffersSpellmaking, set); }
			));
			usertypeDefinition.set("offersSpells", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::OffersSpells); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::OffersSpells, set); }
			));
			usertypeDefinition.set("offersTraining", sol::property(
				[](TES3::Class& self) { return self.getServiceFlag(TES3::ServiceFlag::OffersTraining); },
				[](TES3::Class& self, bool set) { self.setServiceFlag(TES3::ServiceFlag::OffersTraining, set); }
			));

			// Properties that have type remapping.
			usertypeDefinition.set("playable", sol::property(
				[](TES3::Class& self) { return self.playable != 0; },
				[](TES3::Class& self, bool value) { self.playable = value; }
			));

			// Functions exposed as properties.
			usertypeDefinition.set("majorSkills", sol::readonly_property(&TES3::Class::getMajorSkills));
			usertypeDefinition.set("minorSkills", sol::readonly_property(&TES3::Class::getMinorSkills));
			usertypeDefinition.set("name", sol::property(
				[](TES3::Class& self) { return self.name; },
				[](TES3::Class& self, const char* value) { if (strlen(value) < 32) strcpy(self.name, value); }
			));

			// Description may need to be loaded from disk, handle it specially.
			usertypeDefinition.set("description", sol::property(
				[](TES3::Class& self) -> sol::object
			{
				// If the description is already loaded, just return it.
				if (self.description) {
					sol::state& state = LuaManager::getInstance().getState();
					return sol::make_object(state, self.description);
				}

				// Otherwise we need to load it from disk, then free it.
				else {
					char * description = self.loadDescription();
					if (description) {
						// We loaded successfully, package, free, then return.
						sol::state& state = LuaManager::getInstance().getState();
						sol::object value = sol::make_object(state, description);
						self.freeDescription();
						return value;
					}
				}

				return sol::nil;
			},
				[](TES3::Class& self, const char* value)
			{
				self.setDescription(value);
				self.setObjectModified(true);
			}
			));

			// Finish up our usertype.
			state.set_usertype("tes3class", usertypeDefinition);
		}
	}
}
