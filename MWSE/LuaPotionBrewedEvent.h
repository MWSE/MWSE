#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse {
	namespace lua {
		namespace event {
			class PotionBrewedEvent : public GenericEvent, public DisableableEvent<PotionBrewedEvent> {
			public:
				PotionBrewedEvent(TES3::Alchemy * object, TES3::Ingredient* ingredient1, TES3::Ingredient* ingredient2, TES3::Ingredient* ingredient3, TES3::Ingredient* ingredient4);
				sol::table createEventTable();

			protected:
				TES3::Alchemy * m_Object;
				TES3::Ingredient* m_Ingredient1;
				TES3::Ingredient* m_Ingredient2;
				TES3::Ingredient* m_Ingredient3;
				TES3::Ingredient* m_Ingredient4;
			};
		}
	}
}
