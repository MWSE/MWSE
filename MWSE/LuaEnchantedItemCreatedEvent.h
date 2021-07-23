#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	class EnchantedItemCreatedEvent : public GenericEvent, public DisableableEvent<EnchantedItemCreatedEvent> {
	public:
		EnchantedItemCreatedEvent(TES3::Item* createdItem, TES3::Item* baseItem, TES3::Misc* soulGem, TES3::Actor* soul);
		sol::table createEventTable();

	protected:
		TES3::Item* m_ItemCreated;
		TES3::Item* m_EnchantedFrom;
		TES3::Misc* m_SoulGemUsed;
		TES3::Actor* m_SoulUsed;
	};
}
