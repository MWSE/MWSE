#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class DamageEvent : public ObjectFilteredEvent, public DisableableEvent<DamageEvent> {
			public:
				DamageEvent(TES3::MobileActor* mobileActor, float damage);
				sol::table createEventTable();

				static TES3::MobileActor * m_Attacker;
				static TES3::MobileProjectile * m_Projectile;
				static TES3::MagicSourceInstance * m_MagicSourceInstance;
				static TES3::MagicEffectInstance * m_MagicEffectInstance;
				static TES3::ActiveMagicEffect * m_ActiveMagicEffect;
				static const char * m_Source;

			protected:
				TES3::MobileActor * m_MobileActor;
				float m_Damage;
			};
		}
	}
}
