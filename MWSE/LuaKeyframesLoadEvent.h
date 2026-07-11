#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

import NIObject;

namespace mwse::lua::event {
	class KeyframesLoadEvent : public GenericEvent, public DisableableEvent<KeyframesLoadEvent> {
	public:
		KeyframesLoadEvent(const char* path, const char* sequenceName);
		sol::table createEventTable();
		sol::object getEventOptions();

	protected:
		std::string m_Path;
		std::string m_SequenceName;
	};
}
