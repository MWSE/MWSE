#include "LuaKeyEvent.h"

#include "LuaManager.h"

#include "TES3UIElement.h"
#include "TES3UIMenuController.h"
#include "TES3WorldController.h"
#include "TES3InputController.h"

namespace mwse::lua::event {
	KeyEvent::KeyEvent(int keyCode, bool pressed, bool controlDown, bool shiftDown, bool altDown, bool superDown) :
		GenericEvent("key"),
		m_KeyCode(keyCode),
		m_Pressed(pressed),
		m_ControlDown(controlDown),
		m_ShiftDown(shiftDown),
		m_AltDown(altDown),
		m_SuperDown(superDown)
	{

	}

	sol::table KeyEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["keyCode"] = m_KeyCode;
		eventData["pressed"] = m_Pressed;
		eventData["isControlDown"] = m_ControlDown;
		eventData["isShiftDown"] = m_ShiftDown;
		eventData["isAltDown"] = m_AltDown;
		eventData["isSuperDown"] = m_SuperDown;

		const auto worldController = TES3::WorldController::get();
		if (worldController) {
			eventData["element"] = worldController->menuController->menuInputController->getTextInputElement();
		}

		return eventData;
	}

	sol::object KeyEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_KeyCode;

		return options;
	}
}
