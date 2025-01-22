#include "LuaGenericUiActivatedEvent.h"

#include "LuaManager.h"

#include "TES3UIManager.h"

#include "TES3UIElement.h"

namespace mwse::lua::event {
	GenericUiActivatedEvent::GenericUiActivatedEvent(TES3::UI::Element* element) :
		GenericEvent("uiActivated"),
		m_Element(element)
	{
		// Check if the menu was just created or only made visible
		static const auto prop = TES3::UI::registerProperty("MWSE:uiCreated");

		if (element->getProperty(TES3::UI::PropertyType::Integer, prop).integerValue == 0) {
			element->setProperty(prop, 1);
			m_Created = true;
		}
		else {
			m_Created = false;
		}
	}

	sol::table GenericUiActivatedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["element"] = m_Element;
		eventData["newlyCreated"] = m_Created;

		return eventData;
	}

	sol::object GenericUiActivatedEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_Element->name.cString;

		return options;
	}

	GenericUiCreatedEvent::GenericUiCreatedEvent(TES3::UI::Element* element) :
		GenericUiActivatedEvent(element)
	{
		m_EventName = "uiCreated";
	}

}
