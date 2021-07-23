#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	class PotionBrewFailedEvent : public GenericEvent, public DisableableEvent<PotionBrewFailedEvent> {
	public:
		PotionBrewFailedEvent(TES3::Ingredient* ingredient1, TES3::Ingredient* ingredient2, TES3::Ingredient* ingredient3, TES3::Ingredient* ingredient4);
		sol::table createEventTable();

	protected:
		TES3::Ingredient* m_Ingredient1;
		TES3::Ingredient* m_Ingredient2;
		TES3::Ingredient* m_Ingredient3;
		TES3::Ingredient* m_Ingredient4;
	};
}
