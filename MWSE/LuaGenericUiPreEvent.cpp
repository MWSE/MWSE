#include "LuaGenericUiPreEvent.h"

#include "LuaManager.h"

#include "TES3UIElement.h"

namespace mwse {
	namespace lua {
		namespace event {
			GenericUiPreEvent::GenericUiPreEvent(TES3::UI::Element* parent, TES3::UI::Element* element, unsigned int prop, unsigned int var1, unsigned int var2) :
				GenericEvent("uiPreEvent"),
				m_Parent(parent),
				m_Source(element),
				m_Property(prop),
				m_Variable1(var1),
				m_Variable2(var2)
			{

			}

			sol::table GenericUiPreEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["parent"] = m_Parent;
				eventData["source"] = m_Source;
				eventData["property"] = m_Property;
				eventData["var1"] = m_Variable1;
				eventData["var2"] = m_Variable2;

				// TODO: Remove. Legacy support for older mods.
				eventData["block"] = m_Source;

				return eventData;
			}
		}
	}
}
