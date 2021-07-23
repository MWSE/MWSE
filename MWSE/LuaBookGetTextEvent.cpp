#include "LuaBookGetTextEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Book.h"

namespace mwse {
	namespace lua {
		namespace event {
			BookGetTextEvent::BookGetTextEvent(TES3::Book * book) :
				ObjectFilteredEvent("bookGetText", book),
				m_Book(book)
			{

			}

			sol::table BookGetTextEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["book"] = m_Book;

				return eventData;
			}

			bool BookGetTextEvent::m_EventEnabled = false;
		}
	}
}
