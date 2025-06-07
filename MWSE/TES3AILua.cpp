#include "TES3AILua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3AIConfig.h"
#include "TES3AIData.h"
#include "TES3AIPackage.h"
#include "TES3Cell.h"
#include "TES3MobileActor.h"

namespace mwse::lua {
	void bindTES3AI() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Binding for TES3::AIConfig
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AIConfig>("tes3aiConfig");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["alarm"] = &TES3::AIConfig::alarm;
			usertypeDefinition["fight"] = &TES3::AIConfig::fight;
			usertypeDefinition["flee"] = &TES3::AIConfig::flee;
			usertypeDefinition["hello"] = &TES3::AIConfig::hello;
			usertypeDefinition["merchantFlags"] = &TES3::AIConfig::merchantFlags;
			usertypeDefinition["travelDestinations"] = &TES3::AIConfig::travelDestinations;

			// Nicer access to merchant flags.
			usertypeDefinition["bartersAlchemy"] = sol::property(&TES3::AIConfig::getBartersAlchemy, &TES3::AIConfig::setBartersAlchemy);
			usertypeDefinition["bartersApparatus"] = sol::property(&TES3::AIConfig::getBartersApparatus, &TES3::AIConfig::setBartersApparatus);
			usertypeDefinition["bartersArmor"] = sol::property(&TES3::AIConfig::getBartersArmor, &TES3::AIConfig::setBartersArmor);
			usertypeDefinition["bartersBooks"] = sol::property(&TES3::AIConfig::getBartersBooks, &TES3::AIConfig::setBartersBooks);
			usertypeDefinition["bartersClothing"] = sol::property(&TES3::AIConfig::getBartersClothing, &TES3::AIConfig::setBartersClothing);
			usertypeDefinition["bartersEnchantedItems"] = sol::property(&TES3::AIConfig::getBartersEnchantedItems, &TES3::AIConfig::setBartersEnchantedItems);
			usertypeDefinition["bartersIngredients"] = sol::property(&TES3::AIConfig::getBartersIngredients, &TES3::AIConfig::setBartersIngredients);
			usertypeDefinition["bartersLights"] = sol::property(&TES3::AIConfig::getBartersLights, &TES3::AIConfig::setBartersLights);
			usertypeDefinition["bartersLockpicks"] = sol::property(&TES3::AIConfig::getBartersLockpicks, &TES3::AIConfig::setBartersLockpicks);
			usertypeDefinition["bartersMiscItems"] = sol::property(&TES3::AIConfig::getBartersMiscItems, &TES3::AIConfig::setBartersMiscItems);
			usertypeDefinition["bartersProbes"] = sol::property(&TES3::AIConfig::getBartersProbes, &TES3::AIConfig::setBartersProbes);
			usertypeDefinition["bartersRepairTools"] = sol::property(&TES3::AIConfig::getBartersRepairTools, &TES3::AIConfig::setBartersRepairTools);
			usertypeDefinition["bartersWeapons"] = sol::property(&TES3::AIConfig::getBartersWeapons, &TES3::AIConfig::setBartersWeapons);
			usertypeDefinition["offersBartering"] = sol::readonly_property(&TES3::AIConfig::getOffersBartering);
			usertypeDefinition["offersEnchanting"] = sol::property(&TES3::AIConfig::getOffersEnchanting, &TES3::AIConfig::setOffersEnchanting);
			usertypeDefinition["offersRepairs"] = sol::property(&TES3::AIConfig::getOffersRepairs, &TES3::AIConfig::setOffersRepairs);
			usertypeDefinition["offersSpellmaking"] = sol::property(&TES3::AIConfig::getOffersSpellmaking, &TES3::AIConfig::setOffersSpellmaking);
			usertypeDefinition["offersSpells"] = sol::property(&TES3::AIConfig::getOffersSpells, &TES3::AIConfig::setOffersSpells);
			usertypeDefinition["offersTraining"] = sol::property(&TES3::AIConfig::getOffersTraining, &TES3::AIConfig::setOffersTraining);
		}

		// Binding for TES3::AIPlanner
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AIPlanner>("tes3aiPlanner");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["currentPackageIndex"] = sol::readonly_property(&TES3::AIPlanner::currentPackageIndex);
			usertypeDefinition["mobile"] = sol::readonly_property(&TES3::AIPlanner::mobileActor);
			usertypeDefinition["nextOpenPackageIndex"] = sol::readonly_property(&TES3::AIPlanner::nextOpenPackageIndex);

			// Basic function binding.
			usertypeDefinition["getActivePackage"] = &TES3::AIPlanner::getActivePackage;

			// Indirect bindings to unions and arrays.
			usertypeDefinition["packages"] = sol::readonly_property(&TES3::AIPlanner::getPackages);

			// Legacy renamed fields.
			usertypeDefinition["countPackages"] = sol::readonly_property(&TES3::AIPlanner::currentPackageIndex);
			usertypeDefinition["indexActivePackage"] = sol::readonly_property(&TES3::AIPlanner::nextOpenPackageIndex);
		}

		// Binding for TES3::AIPackage
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AIPackage>("tes3aiPackage");
			usertypeDefinition["new"] = sol::no_constructor;

			setUserdataForTES3AIPackage(usertypeDefinition);
		}

		// Binding for TES3::AIPackageTravel
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AIPackageTravel>("tes3aiPackageTravel");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::AIPackage>();
			setUserdataForTES3AIPackage(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["destination"] = &TES3::AIPackageTravel::destination;
		}

		// Binding for TES3::AIPackageWander
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AIPackageWander>("tes3aiPackageWander");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::AIPackage>();
			setUserdataForTES3AIPackage(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["activationTarget"] = &TES3::AIPackageWander::activationTarget;
			usertypeDefinition["idles"] = sol::readonly_property(&TES3::AIPackageWander::getIdles);
		}

		// Binding for TES3::AIPackageWander::IdleNode
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AIPackageWander::IdleNode>("tes3aiPackageWanderIdleNode");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["chance"] = &TES3::AIPackageWander::IdleNode::chance;
			usertypeDefinition["index"] = &TES3::AIPackageWander::IdleNode::index;
		}

		// Binding for TES3::AIPackageEscort
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AIPackageEscort>("tes3aiPackageEscort");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::AIPackage>();
			setUserdataForTES3AIPackage(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["destination"] = &TES3::AIPackageEscort::destination;
			usertypeDefinition["targetActor"] = sol::property(&TES3::AIPackageEscort::getTargetActor, &TES3::AIPackageEscort::setTargetActorAsFriend);
		}

		// Binding for TES3::AIPackageFollow
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AIPackageFollow>("tes3aiPackageFollow");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::AIPackage>();
			setUserdataForTES3AIPackage(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["destination"] = &TES3::AIPackageFollow::destination;
			usertypeDefinition["followDistance"] = &TES3::AIPackageFollow::followDistance;
			usertypeDefinition["targetActor"] = sol::property(&TES3::AIPackageEscort::getTargetActor, &TES3::AIPackageFollow::setTargetActorAsFriendIfActive);
		}

		// Binding for TES3::AIPackageActivate
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AIPackageActivate>("tes3aiPackageActivate");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::AIPackage>();
			setUserdataForTES3AIPackage(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["activateTarget"] = &TES3::AIPackageActivate::activateTarget;
		}
	}
}

