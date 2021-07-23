#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class LeveledItemPickedEvent : public ObjectFilteredEvent, public DisableableEvent<LeveledItemPickedEvent> {
			public:
				LeveledItemPickedEvent(TES3::LeveledItem * list, TES3::Object * vanillaResult);
				sol::table createEventTable();

				static TES3::Reference* m_Reference;

			protected:
				TES3::LeveledItem* m_List;
				TES3::Object* m_Result;
			};
		}
	}
}
