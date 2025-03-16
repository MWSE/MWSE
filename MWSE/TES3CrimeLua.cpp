#include "TES3CrimeLua.h"

#include "LuaManager.h"

#include "TES3CrimeEvent.h"

#include "TES3Actor.h"
#include "TES3AIData.h"
#include "TES3Faction.h"
#include "TES3MobilePlayer.h"

namespace mwse::lua {
	void bindTES3Crime() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Binding for TES3::CrimeEvent
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::CrimeEvent>("tes3crimeEvent");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["bountyKey"] = sol::property(&TES3::CrimeEvent::getBountyKey, &TES3::CrimeEvent::setBountyKey);
			usertypeDefinition["criminal"] = &TES3::CrimeEvent::criminal;
			usertypeDefinition["position"] = &TES3::CrimeEvent::position;
			usertypeDefinition["stolenFrom"] = &TES3::CrimeEvent::stolenFrom;
			usertypeDefinition["stolenValue"] = &TES3::CrimeEvent::stolenValue;
			usertypeDefinition["timestamp"] = &TES3::CrimeEvent::timestamp;
			usertypeDefinition["type"] = &TES3::CrimeEvent::type;
			usertypeDefinition["victim"] = &TES3::CrimeEvent::victim;
			usertypeDefinition["victimFaction"] = &TES3::CrimeEvent::victimFaction;
			usertypeDefinition["witnesses"] = &TES3::CrimeEvent::witnesses;
		}

		// Binding for TES3::BountyData
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::BountyData>("tes3bountyData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["keys"] = sol::readonly_property(&TES3::BountyData::getKeys_lua);

			// Basic function binding.
			usertypeDefinition["getValue"] = &TES3::BountyData::getValue_lua;
			usertypeDefinition["setValue"] = &TES3::BountyData::setValue_lua;
			usertypeDefinition["modValue"] = &TES3::BountyData::modValue_lua;
		}
	}
}