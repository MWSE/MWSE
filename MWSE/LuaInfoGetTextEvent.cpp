#include "LuaInfoGetTextEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3DialogueInfo.h"

namespace mwse::lua::event {
	InfoGetTextEvent::InfoGetTextEvent(TES3::DialogueInfo* info) :
		ObjectFilteredEvent("infoGetText", info),
		m_DialogueInfo(info)
	{

	}

	sol::table InfoGetTextEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["info"] = m_DialogueInfo;
		eventData["loadOriginalText"] = getOriginalText;

		return eventData;
	}

	const char* InfoGetTextEvent::getOriginalText(sol::table self) {
		// Did we already find the text?
		sol::optional<const char*> cacheHit = self["_loadedOriginalText"];
		if (cacheHit) {
			return cacheHit.value();
		}

		setEventEnabled(false);
		auto text = self.get<TES3::DialogueInfo*>("info")->getText();
		self["_loadedOriginalText"] = text;
		setEventEnabled(true);
		return text;
	}
}
