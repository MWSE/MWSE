#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class DamageHandToHandEvent : public ObjectFilteredEvent, public DisableableEvent<DamageHandToHandEvent> {
	public:
		DamageHandToHandEvent(TES3::MobileActor* mobileActor, float fatigueDamage);
		sol::table createEventTable();

		static TES3::MobileActor* m_Attacker;
		static const char* m_Source;

	protected:
		TES3::MobileActor* m_MobileActor;
		float m_FatigueDamage;
	};
}
