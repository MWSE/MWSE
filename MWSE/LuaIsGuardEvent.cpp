#include "LuaIsGuardEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3NPC.h"

namespace mwse {
    namespace lua {
        namespace event {
            IsGuardEvent::IsGuardEvent(TES3::NPCBase* npc, bool isGuard) :
                GenericEvent("isGuard"),
                m_NpcBase(npc),
                m_IsGuard(isGuard)
            {

            }

            sol::table IsGuardEvent::createEventTable() {
                auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
                sol::state& state = stateHandle.state;
                sol::table eventData = state.create_table();

                eventData["npc"] = m_NpcBase;
                eventData["isGuard"] = m_IsGuard;

                return eventData;
            }

            bool IsGuardEvent::m_EventEnabled = false;
        }
    }
}