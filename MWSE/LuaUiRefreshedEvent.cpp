#include "LuaUiRefreshedEvent.h"

#include "LuaManager.h"

#include "TES3UIElement.h"

namespace mwse {
	namespace lua {
		namespace event {
			UiRefreshedEvent::UiRefreshedEvent(TES3::UI::Element * element) :
				GenericEvent("uiRefreshed"),
				m_Element(element)
			{

			}

			sol::table UiRefreshedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["element"] = m_Element;

				return eventData;
			}

			sol::object UiRefreshedEvent::getEventOptions() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table options = LuaManager::getInstance().createTable();

				options["filter"] = m_Element->name.cString;

				return options;
			}
		}
	}
}
