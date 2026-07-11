#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

import NIObject;

namespace mwse::lua::event {
	class MeshLoadEvent : public GenericEvent, public DisableableEvent<MeshLoadEvent> {
	public:
		MeshLoadEvent(const char* path);
		sol::table createEventTable();
		sol::object getEventOptions();

	protected:
		std::string m_Path;
	};
}
