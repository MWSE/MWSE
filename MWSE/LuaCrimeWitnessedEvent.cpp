#include "LuaCrimeWitnessedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3CrimeEvent.h"
#include "TES3Faction.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CrimeWitnessedEvent::CrimeWitnessedEvent(TES3::MobileActor* witness, TES3::CrimeEvent* crime) :
		GenericEvent("crimeWitnessed"),
		m_Witness(witness),
		m_Crime(crime)
	{

	}

	sol::object CrimeWitnessedEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();
		options["filter"] = m_Crime->bountyKey.c_str;
		return options;
	}

	sol::table CrimeWitnessedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["type"] = m_Crime->bountyKey.c_str;
		eventData["position"] = m_Crime->position;
		eventData["realTimestamp"] = m_Crime->timestamp;
		if (m_Crime->stolenFrom) {
			eventData["victim"] = m_Crime->stolenFrom;
		}
		else {
			eventData["victim"] = m_Crime->victimFaction;
		}
		eventData["victimMobile"] = m_Crime->victim;
		eventData["victimFaction"] = m_Crime->victimFaction;
		eventData["value"] = m_Crime->stolenValue;
		eventData["witness"] = m_Witness->reference;
		eventData["witnessMobile"] = m_Witness;

		eventData["crimeEvent"] = m_Crime;

		return eventData;
	}
}
