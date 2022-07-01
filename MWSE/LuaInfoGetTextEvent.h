#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class InfoGetTextEvent : public ObjectFilteredEvent, public DisableableEvent<InfoGetTextEvent> {
	public:
		InfoGetTextEvent(TES3::DialogueInfo* info);
		sol::table createEventTable();

		static const char* getOriginalText(sol::table self);

	protected:
		TES3::DialogueInfo* m_DialogueInfo;
	};
}
