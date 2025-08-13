#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3AnimationGroup.h"

namespace mwse::lua::event {
	class KeyframesParsedEvent : public GenericEvent, public DisableableEvent<KeyframesParsedEvent> {
	public:
		KeyframesParsedEvent(const char* path, const std::vector<TES3::ParsedTextKeyEntry>& entries);
		sol::table createEventTable();
		sol::object getEventOptions();

		static void refillEntryVector(sol::table luaEntries, std::vector<TES3::ParsedTextKeyEntry>& cEntries);

	protected:
		const char* m_Path;
		sol::table m_Entries;
	};
}
