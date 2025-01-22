#include "LuaMouseWheelEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	MouseWheelEvent::MouseWheelEvent(int delta, bool controlDown, bool shiftDown, bool altDown, bool superDown) :
		GenericEvent("mouseWheel"),
		m_Delta(delta),
		m_ControlDown(controlDown),
		m_ShiftDown(shiftDown),
		m_AltDown(altDown),
		m_SuperDown(superDown)
	{

	}

	sol::table MouseWheelEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["delta"] = m_Delta;
		eventData["isControlDown"] = m_ControlDown;
		eventData["isShiftDown"] = m_ShiftDown;
		eventData["isAltDown"] = m_AltDown;
		eventData["isSuperDown"] = m_SuperDown;

		return eventData;
	}
}
