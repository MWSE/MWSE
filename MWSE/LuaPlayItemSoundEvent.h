#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class PlayItemSoundEvent : public ObjectFilteredEvent, public DisableableEvent<PlayItemSoundEvent> {
			public:
				PlayItemSoundEvent(TES3::BaseObject* item, int state, TES3::Reference* reference);
				sol::table createEventTable();

			protected:
				TES3::BaseObject* m_Item;
				int m_State;
				TES3::Reference* m_Reference;
			};
		}
	}
}
