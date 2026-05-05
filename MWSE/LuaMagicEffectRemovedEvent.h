#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class MagicEffectRemovedEvent : public ObjectFilteredEvent, public DisableableEvent<MagicEffectRemovedEvent> {
	public:
		MagicEffectRemovedEvent(TES3::MagicSourceInstance* magicSourceInstance, TES3::MagicEffectInstance* magicEffectInstance, int effectIndex);
		sol::table createEventTable();

	protected:
		TES3::MagicSourceInstance* m_MagicSourceInstance;
		TES3::MagicEffectInstance* m_MagicEffectInstance;
		int m_EffectIndex;
	};
}
