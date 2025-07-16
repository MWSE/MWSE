#include "TES3RaceLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3BodyPart.h"
#include "TES3Race.h"
#include "TES3SpellList.h"

namespace mwse::lua {
	void bindTES3Race() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::Race::SkillBonus.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Race::SkillBonus>("tes3raceSkillBonus");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["skill"] = &TES3::Race::SkillBonus::skill;
			usertypeDefinition["bonus"] = &TES3::Race::SkillBonus::bonus;
		}

		// Binding for TES3::Race::BaseAttribute.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Race::BaseAttribute>("tes3raceBaseAttribute");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["male"] = &TES3::Race::BaseAttribute::male;
			usertypeDefinition["female"] = &TES3::Race::BaseAttribute::female;
		}

		// Binding for TES3::Race::HeightWeight.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Race::HeightWeight>("tes3raceHeightWeight");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["male"] = &TES3::Race::HeightWeight::male;
			usertypeDefinition["female"] = &TES3::Race::HeightWeight::female;
		}

		// Binding for TES3::Race::BodyParts.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Race::BodyParts>("tes3raceBodyParts");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["ankle"] = &TES3::Race::BodyParts::ankle;
			usertypeDefinition["chest"] = &TES3::Race::BodyParts::chest;
			usertypeDefinition["clavicle"] = &TES3::Race::BodyParts::clavicle;
			usertypeDefinition["foot"] = &TES3::Race::BodyParts::foot;
			usertypeDefinition["forearm"] = &TES3::Race::BodyParts::forearm;
			usertypeDefinition["groin"] = &TES3::Race::BodyParts::groin;
			usertypeDefinition["hair"] = &TES3::Race::BodyParts::hair;
			usertypeDefinition["hands"] = &TES3::Race::BodyParts::hands;
			usertypeDefinition["head"] = &TES3::Race::BodyParts::head;
			usertypeDefinition["knee"] = &TES3::Race::BodyParts::knee;
			usertypeDefinition["neck"] = &TES3::Race::BodyParts::neck;
			usertypeDefinition["tail"] = &TES3::Race::BodyParts::tail;
			usertypeDefinition["upperArm"] = &TES3::Race::BodyParts::upperArm;
			usertypeDefinition["upperLeg"] = &TES3::Race::BodyParts::upperLeg;
			usertypeDefinition["vampireHead"] = &TES3::Race::BodyParts::vampireHead;
			usertypeDefinition["wrist"] = &TES3::Race::BodyParts::wrist;

			// Legacy bindings.
			usertypeDefinition["head1"] = &TES3::Race::BodyParts::head;
			usertypeDefinition["head2"] = &TES3::Race::BodyParts::vampireHead;
		}

		// Binding for TES3::Race.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Race>("tes3race");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
			setUserdataForTES3BaseObject(usertypeDefinition);

			// Base class overrides.
			usertypeDefinition[sol::meta_function::to_string] = &TES3::Race::getObjectID;
			usertypeDefinition["id"] = sol::readonly_property(&TES3::Race::getObjectID);

			// Basic property binding.
			usertypeDefinition["femaleBody"] = sol::readonly_property(&TES3::Race::femaleBody);
			usertypeDefinition["flags"] = &TES3::Race::flags;
			usertypeDefinition["height"] = sol::readonly_property(&TES3::Race::height);
			usertypeDefinition["maleBody"] = sol::readonly_property(&TES3::Race::maleBody);
			usertypeDefinition["weight"] = sol::readonly_property(&TES3::Race::weight);

			// Access to race flags.
			usertypeDefinition["isBeast"] = sol::property(&TES3::Race::getIsBeast, &TES3::Race::setIsBeast);
			usertypeDefinition["isPlayable"] = sol::property(&TES3::Race::getIsPlayable, &TES3::Race::setIsPlayable);

			// Indirect bindings to unions and arrays.
			usertypeDefinition["abilities"] = sol::readonly_property(&TES3::Race::abilities);
			usertypeDefinition["baseAttributes"] = sol::readonly_property(&TES3::Race::getBaseAttributes);
			usertypeDefinition["skillBonuses"] = sol::readonly_property(&TES3::Race::getSkillBonuses);

			// Functions exposed as properties.
			usertypeDefinition["name"] = sol::readonly_property(&TES3::Race::getName);

			// Description may need to be loaded from disk, handle it specially.
			usertypeDefinition["description"] = sol::readonly_property(&TES3::Race::getAndLoadDescription);
		}
	}
}
