#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class RemovedEquipmentBodyPartsEvent : public ObjectFilteredEvent, public DisableableEvent<RemovedEquipmentBodyPartsEvent> {
	public:
		RemovedEquipmentBodyPartsEvent(TES3::BodyPartManager* bodyPartManager);
		sol::table createEventTable();

	protected:
		TES3::BodyPartManager* m_BodyPartManager;
	};
}
