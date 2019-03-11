#include "LuaMusicSelectTrackEvent.h"

#include "LuaManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			MusicSelectTrackEvent::MusicSelectTrackEvent(int situation) :
				GenericEvent("musicSelectTrack"),
				m_Situation(situation)
			{

			}

			sol::table MusicSelectTrackEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();
				eventData["situation"] = m_Situation;
				return eventData;
			}
		}
	}
}
