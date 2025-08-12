#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class ActivationTargetChangedEvent : public ObjectFilteredEvent, public DisableableEvent<ActivationTargetChangedEvent> {
	public:
		ActivationTargetChangedEvent(TES3::Reference* current);
		sol::table createEventTable();

		static TES3::Reference* ms_PreviousReference;

	protected:
		TES3::Reference* m_CurrentReference;
	};
}
