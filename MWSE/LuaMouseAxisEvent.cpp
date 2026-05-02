#include "LuaMouseAxisEvent.h"

#include "LuaManager.h"

namespace mwse::lua::event {
	MouseAxisEvent::MouseAxisEvent(int deltaX, int deltaY, bool controlDown, bool shiftDown, bool altDown, bool superDown) :
		GenericEvent("mouseAxis"),
		m_DeltaX(deltaX),
		m_DeltaY(deltaY),
		m_ControlDown(controlDown),
		m_ShiftDown(shiftDown),
		m_AltDown(altDown),
		m_SuperDown(superDown)
	{

	}

	sol::table MouseAxisEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["deltaX"] = m_DeltaX;
		eventData["deltaY"] = m_DeltaY;
		eventData["isControlDown"] = m_ControlDown;
		eventData["isShiftDown"] = m_ShiftDown;
		eventData["isAltDown"] = m_AltDown;
		eventData["isSuperDown"] = m_SuperDown;

		return eventData;
	}
}
