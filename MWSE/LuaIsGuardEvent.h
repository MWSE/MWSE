#pragma once

#include "TES3NPC.h"

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class IsGuardEvent : public GenericEvent, public DisableableEvent<IsGuardEvent> {
			public:
				IsGuardEvent(TES3::NPCBase* npc, bool isGuard);
				sol::table createEventTable();

			protected:
				TES3::NPCBase* m_NpcBase;
				bool m_IsGuard;
			};
		}
	}
}
