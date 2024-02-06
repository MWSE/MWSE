#include "LuaAnimationTriggerEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3AnimationData.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	AnimationTriggerEvent::AnimationTriggerEvent(TES3::Reference* reference, const std::string& triggerName, const std::string& triggerParam) :
		GenericEvent("animationTrigger"),
		m_Reference(reference),
		m_TriggerName(triggerName),
		m_TriggerParam(triggerParam)
	{
	}

	sol::table AnimationTriggerEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["reference"] = m_Reference;
		eventData["name"] = m_TriggerName;
		if (!m_TriggerParam.empty()) {
			eventData["param"] = m_TriggerParam;
		}

		return eventData;
	}

	bool AnimationTriggerEvent::m_EventEnabled = false;
}