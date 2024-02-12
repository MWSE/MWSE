#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3ActorAnimationController.h"

namespace mwse::lua::event {
	class KeyframesLoadedEvent : public GenericEvent, public DisableableEvent<KeyframesLoadedEvent> {
	public:
		KeyframesLoadedEvent(const char* path, const char* sequenceName, TES3::KeyframeDefinition* keyframeDefinition);
		sol::table createEventTable();
		sol::object getEventOptions();

	protected:
		std::string m_Path;
		std::string m_SequenceName;
		TES3::KeyframeDefinition* m_KeyframeDefinition;
	};
}
