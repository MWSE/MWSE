#include "LuaMobileObjectWaterImpactEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileObject.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	MobileObjectWaterImpactEvent::MobileObjectWaterImpactEvent(TES3::MobileObject* mobileObject, bool inWater) :
		ObjectFilteredEvent("collideWater", mobileObject->reference),
		m_MobileObject(mobileObject),
		m_InWater(inWater)
	{

	}

	sol::table MobileObjectWaterImpactEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		if (m_MobileObject) {
			eventData["mobile"] = m_MobileObject;
			eventData["reference"] = m_MobileObject->reference;
		}
		eventData["inWater"] = m_InWater;

		return eventData;
	}
}
