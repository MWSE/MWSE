#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class CellActivatedEvent : public ObjectFilteredEvent, public DisableableEvent<CellActivatedEvent> {
			public:
				CellActivatedEvent(TES3::Cell* cell);
				sol::table createEventTable();

			protected:
				TES3::Cell* m_Cell;
			};
		}
	}
}
