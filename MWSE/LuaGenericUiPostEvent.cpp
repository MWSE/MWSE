#include "LuaGenericUiPostEvent.h"

#include "LuaManager.h"

#include "TES3UIElement.h"

namespace mwse::lua::event {
	GenericUiPostEvent::GenericUiPostEvent(TES3::UI::Element* parent, TES3::UI::Element* element, unsigned int prop, unsigned int var1, unsigned int var2) :
		GenericEvent("uiEvent"),
		m_Parent(parent),
		m_Source(element),
		m_Property(prop),
		m_Variable1(var1),
		m_Variable2(var2)
	{

	}

	sol::table GenericUiPostEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["parent"] = m_Parent;
		eventData["source"] = m_Source;
		eventData["property"] = m_Property;
		eventData["var1"] = m_Variable1;
		eventData["var2"] = m_Variable2;

		// TODO: Remove. Legacy support for older mods.
		eventData["block"] = m_Source;

		return eventData;
	}

	sol::object GenericUiPostEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_Source->id;

		return options;
	}
}
