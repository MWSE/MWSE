#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class UpdateBodyPartsForItemEvent : public ObjectFilteredEvent, public DisableableEvent<UpdateBodyPartsForItemEvent> {
	public:
		UpdateBodyPartsForItemEvent(TES3::Item* item, TES3::BodyPartManager* bodyPartManager, bool isFemale, bool isFirstPerson);
		sol::table createEventTable();

	protected:
		TES3::Item* m_Item;
		TES3::BodyPartManager* m_BodyPartManager;
		bool m_IsFemale;
		bool m_IsFirstPerson;
	};
}
