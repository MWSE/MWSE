#include "LuaIsMobilePlayerUnderwaterEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobilePlayer.h"

namespace mwse {
    namespace lua {
        namespace event {
            IsMobilePlayerUnderwaterEvent::IsMobilePlayerUnderwaterEvent(TES3::MobilePlayer* mobilePlayer, bool isUnderwater) :
                GenericEvent("isMobilePlayerUnderwater"),
                m_MobilePlayer(mobilePlayer),
                m_IsUnderwater(isUnderwater)
            {

            }

            sol::table IsMobilePlayerUnderwaterEvent::createEventTable() {
                auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
                sol::state& state = stateHandle.state;
                sol::table eventData = state.create_table();

                eventData["isUnderwater"] = m_IsUnderwater;

                return eventData;
            }

            bool IsMobilePlayerUnderwaterEvent::m_EventEnabled = false;
        }
    }
}