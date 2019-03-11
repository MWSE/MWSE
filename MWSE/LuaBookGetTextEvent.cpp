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
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["book"] = makeLuaObject(m_Book);

				return eventData;
			}
		}
	}
}
