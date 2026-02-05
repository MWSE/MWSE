#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Reference.h"

namespace mwse::lua::event {
	class AnimationTriggerEvent : public GenericEvent, public DisableableEvent<AnimationTriggerEvent> {
	public:
		AnimationTriggerEvent(TES3::Reference* reference, const std::string& triggerName, const std::string& triggerParam);
		sol::table createEventTable();
		sol::object AnimationTriggerEvent::getEventOptions();

	protected:
		TES3::Reference* m_Reference;
		std::string m_TriggerName;
		std::string m_TriggerParam;
	};
}
