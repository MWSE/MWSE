#include "LuaDamagedHandToHandEvent.h"
#include "LuaDamageHandToHandEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	DamagedHandToHandEvent::DamagedHandToHandEvent(TES3::MobileActor* mobileActor, float fatigueDamage) :
		ObjectFilteredEvent("damagedHandToHand", mobileActor->reference),
		m_MobileActor(mobileActor),
		m_FatigueDamage(fatigueDamage)
	{

	}

	sol::table DamagedHandToHandEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		if (m_MobileActor) {
			eventData["mobile"] = m_MobileActor;
			eventData["reference"] = m_MobileActor->reference;
		}

		if (DamageHandToHandEvent::m_Attacker) {
			eventData["attacker"] = DamageHandToHandEvent::m_Attacker;
			eventData["attackerReference"] = DamageHandToHandEvent::m_Attacker->reference;
		}

		if (DamageHandToHandEvent::m_Source) {
			eventData["source"] = DamageHandToHandEvent::m_Source;
		}

		eventData["fatigueDamage"] = m_FatigueDamage;

		return eventData;
	}
}
