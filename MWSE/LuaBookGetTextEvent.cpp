#include "LuaBookGetTextEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Book.h"

namespace mwse::lua::event {
	BookGetTextEvent::BookGetTextEvent(TES3::Book* book) :
		ObjectFilteredEvent("bookGetText", book),
		m_Book(book)
	{

	}

	sol::table BookGetTextEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["book"] = m_Book;
		eventData["loadOriginalText"] = getOriginalText;

		return eventData;
	}

	const char* BookGetTextEvent::getOriginalText(sol::table self) {
		// Did we already find the text?
		sol::optional<const char*> cacheHit = self["_loadedOriginalText"];
		if (cacheHit) {
			return cacheHit.value();
		}

		setEventEnabled(false);
		auto text = self.get<TES3::Book*>("book")->getBookText();
		self["_loadedOriginalText"] = text;
		setEventEnabled(true);
		return text;
	}
}
