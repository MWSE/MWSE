#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class PickpocketEvent : public GenericEvent, public DisableableEvent<PickpocketEvent> {
	public:
		PickpocketEvent(TES3::MobileActor* mobile, TES3::Item* item, TES3::ItemData* itemData, int count, int chance);
		sol::table createEventTable();

	protected:
		TES3::MobileActor* m_MobileActor;
		TES3::Item* m_Item;
		TES3::ItemData* m_ItemData;
		int m_Count;
		int m_Chance;
	};
}
