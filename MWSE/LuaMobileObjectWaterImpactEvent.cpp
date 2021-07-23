#include "LuaMobileObjectWaterImpactEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileObject.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			MobileObjectWaterImpactEvent::MobileObjectWaterImpactEvent(TES3::MobileObject* mobileObject, bool inWater) :
				ObjectFilteredEvent("collideWater", mobileObject->reference),
				m_MobileObject(mobileObject),
				m_InWater(inWater)
			{

			}

			sol::table MobileObjectWaterImpactEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["mobile"] = m_MobileObject;
				eventData["inWater"] = m_InWater;

				return eventData;
			}

			bool MobileObjectWaterImpactEvent::m_EventEnabled = false;
		}
	}
}
