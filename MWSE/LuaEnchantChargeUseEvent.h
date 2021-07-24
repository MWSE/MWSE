#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse {
	namespace lua {
		namespace event {
			class EnchantChargeUseEvent : public GenericEvent, public DisableableEvent<EnchantChargeUseEvent> {
			public:
				EnchantChargeUseEvent(TES3::Enchantment* enchant, TES3::MobileActor* caster, float chargeRequired, bool isCast);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				TES3::Enchantment* m_Enchant;
				TES3::MobileActor* m_Caster;
				float m_ChargeRequired;
				bool m_IsCast;
			};
		}
	}
}
