#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class ObjectCopiedEvent : public ObjectFilteredEvent, public DisableableEvent<ObjectCopiedEvent> {
	public:
		ObjectCopiedEvent(TES3::Object* copy, TES3::Object* original);
		sol::table createEventTable();

		static TES3::Object* ms_LastCopied;
		static TES3::Object* ms_LastCopiedFrom;

	protected:
		TES3::Object* m_Copy;
		TES3::Object* m_Original;

	};
}
