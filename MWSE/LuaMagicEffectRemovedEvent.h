#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	class MagicEffectRemovedEvent : public GenericEvent, public DisableableEvent<MagicEffectRemovedEvent> {
	public:
		MagicEffectRemovedEvent(TES3::MagicSourceInstance* magicSourceInstance, TES3::MagicEffectInstance* magicEffectInstance, int effectIndex);
		sol::table createEventTable();
		sol::object getEventOptions();

	protected:
		TES3::MagicSourceInstance* m_MagicSourceInstance;
		TES3::MagicEffectInstance* m_MagicEffectInstance;
		int m_EffectIndex;
		int m_EffectId;
	};
}
