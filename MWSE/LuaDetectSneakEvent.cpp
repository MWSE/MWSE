#include "LuaDetectSneakEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	DetectSneakEvent::DetectSneakEvent(TES3::MobileActor* detector, TES3::MobileActor* target, bool isDetected) :
		ObjectFilteredEvent("detectSneak", detector->reference),
		m_Detector(detector),
		m_Target(target),
		m_IsDetected(isDetected)
	{

	}

	sol::table DetectSneakEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["detector"] = m_Detector;
		eventData["target"] = m_Target;
		eventData["isDetected"] = m_IsDetected;

		return eventData;
	}
}
