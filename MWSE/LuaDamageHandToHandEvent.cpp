#include "LuaDamageHandToHandEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	DamageHandToHandEvent::DamageHandToHandEvent(TES3::MobileActor* mobileActor, float fatigueDamage) :
		ObjectFilteredEvent("damageHandToHand", mobileActor->reference),
		m_MobileActor(mobileActor),
		m_FatigueDamage(fatigueDamage)
	{

	}

	sol::table DamageHandToHandEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		if (m_MobileActor) {
			eventData["mobile"] = m_MobileActor;
			eventData["reference"] = m_MobileActor->reference;
		}

		if (m_Attacker) {
			eventData["attacker"] = m_Attacker;
			eventData["attackerReference"] = m_Attacker->reference;
		}

		if (m_Source) {
			eventData["source"] = m_Source;
		}

		eventData["fatigueDamage"] = m_FatigueDamage;

		return eventData;
	}

	TES3::MobileActor* DamageHandToHandEvent::m_Attacker = nullptr;
	const char* DamageHandToHandEvent::m_Source = nullptr;
}
