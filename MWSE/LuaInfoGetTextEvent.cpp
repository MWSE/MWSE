#include "LuaInfoGetTextEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3DialogueInfo.h"

namespace mwse {
	namespace lua {
		namespace event {
			InfoGetTextEvent::InfoGetTextEvent(TES3::DialogueInfo * info) :
				ObjectFilteredEvent("infoGetText", info),
				m_DialogueInfo(info)
			{

			}

			sol::table InfoGetTextEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["info"] = makeLuaObject(m_DialogueInfo);

				return eventData;
			}
		}
	}
}
