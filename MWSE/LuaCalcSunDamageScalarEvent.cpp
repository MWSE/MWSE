#include "LuaCalcSunDamageScalarEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3WeatherController.h"

namespace mwse {
    namespace lua {
        namespace event {
            CalcSunDamageScalarEvent::CalcSunDamageScalarEvent(float damage) :
                GenericEvent("calcSunDamageScalar"),
                m_Damage(damage)
            {

            }

            sol::table CalcSunDamageScalarEvent::createEventTable() {
                auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
                sol::state& state = stateHandle.state;
                sol::table eventData = state.create_table();

                eventData["damage"] = m_Damage;

                return eventData;
            }

            bool CalcSunDamageScalarEvent::m_EventEnabled = false;
        }
    }
}