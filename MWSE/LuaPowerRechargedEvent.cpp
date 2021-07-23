#include "LuaPowerRechargedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

namespace mwse {
	namespace lua {
		namespace event {
			PowerRechargedEvent::PowerRechargedEvent(TES3::Spell* power, TES3::MobileActor* mobile) :
				ObjectFilteredEvent("powerRecharged", power),
				m_Power(power),
				m_Mobile(mobile)
			{

			}

			sol::table PowerRechargedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["power"] = m_Power;
				eventData["mobile"] = m_Mobile;
				eventData["reference"] = m_Mobile->reference;

				return eventData;
			}

			bool PowerRechargedEvent::m_EventEnabled = false;
		}
	}
}
