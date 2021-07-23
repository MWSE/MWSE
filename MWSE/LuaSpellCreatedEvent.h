#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse {
	namespace lua {
		namespace event {
			class SpellCreatedEvent : public GenericEvent, public DisableableEvent<SpellCreatedEvent> {
			public:
				SpellCreatedEvent(TES3::Spell* spell, const char* source);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				TES3::Spell* m_Spell;
				const char* m_Source;
			};
		}
	}
}
