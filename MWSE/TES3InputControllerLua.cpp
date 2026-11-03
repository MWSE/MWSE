#include "TES3InputControllerLua.h"

#include "LuaManager.h"

#include "TES3InputController.h"

namespace mwse::lua {
	void bindTES3InputController() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::InputConfig
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::InputConfig>("tes3inputConfig");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["code"] = &TES3::InputConfig::keyCode;
			usertypeDefinition["device"] = &TES3::InputConfig::device;
		}

		// Binding for TES3::InputController::MouseState (DIMOUSESTATE2)
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::InputController::DirectInputMouseState>("tes3directInputMouseState");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["x"] = &TES3::InputController::DirectInputMouseState::lX;
			usertypeDefinition["y"] = &TES3::InputController::DirectInputMouseState::lY;
			usertypeDefinition["z"] = &TES3::InputController::DirectInputMouseState::lZ;

			// Indirect bindings to unions and arrays.
			usertypeDefinition["buttons"] = sol::readonly_property(&TES3::InputController::DirectInputMouseState::getButtons);
		}

		// Raw DirectInput gamepad state and its current/previous pair.
		{
			auto usertypeDefinition = state.new_usertype<TES3::InputController::DirectInputGamepadState>("tes3directInputGamepadState");
			usertypeDefinition["new"] = sol::no_constructor;
			usertypeDefinition["x"] = &TES3::InputController::DirectInputGamepadState::lX;
			usertypeDefinition["y"] = &TES3::InputController::DirectInputGamepadState::lY;
			usertypeDefinition["z"] = &TES3::InputController::DirectInputGamepadState::lZ;
			usertypeDefinition["rx"] = &TES3::InputController::DirectInputGamepadState::lRx;
			usertypeDefinition["ry"] = &TES3::InputController::DirectInputGamepadState::lRy;
			usertypeDefinition["rz"] = &TES3::InputController::DirectInputGamepadState::lRz;
			usertypeDefinition["buttons"] = sol::readonly_property(&TES3::InputController::DirectInputGamepadState::getButtons);
			usertypeDefinition["sliders"] = sol::readonly_property(&TES3::InputController::DirectInputGamepadState::getSliders);
			usertypeDefinition["povs"] = sol::readonly_property(&TES3::InputController::DirectInputGamepadState::getPovs);
		}
		{
			auto usertypeDefinition = state.new_usertype<TES3::InputController::GamepadState>("tes3gamepadState");
			usertypeDefinition["new"] = sol::no_constructor;
			usertypeDefinition["current"] = sol::readonly_property(&TES3::InputController::GamepadState::current);
			usertypeDefinition["previous"] = sol::readonly_property(&TES3::InputController::GamepadState::previous);
		}

		// Binding for TES3::InputController
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::InputController>("tes3inputController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["creationFlags"] = sol::readonly_property(&TES3::InputController::creationFlags);
			usertypeDefinition["inputMaps"] = sol::readonly_property(&TES3::InputController::getInputConfigs);
			usertypeDefinition["keyboardState"] = sol::readonly_property(&TES3::InputController::getKeyboardState);
			usertypeDefinition["mouseState"] = sol::readonly_property(&TES3::InputController::mouseState);
			usertypeDefinition["previousKeyboardState"] = sol::readonly_property(&TES3::InputController::getPreviousKeyboardState);
			usertypeDefinition["previousMouseState"] = sol::readonly_property(&TES3::InputController::previousMouseState);

			usertypeDefinition["gamepadCount"] = sol::readonly_property(&TES3::InputController::getGamepadCount);
			usertypeDefinition["gamepadStates"] = sol::readonly_property(&TES3::InputController::getGamepadStates);
			usertypeDefinition["isGamepadButtonDown"] = &TES3::InputController::isGamepadButtonDown;
			usertypeDefinition["isGamepadButtonPressedThisFrame"] = &TES3::InputController::isGamepadButtonPressedThisFrame;
			usertypeDefinition["isGamepadButtonReleasedThisFrame"] = &TES3::InputController::isGamepadButtonReleasedThisFrame;

			// Basic function binding.
			usertypeDefinition["isAltDown"] = &TES3::InputController::isAltDown;
			usertypeDefinition["isCapsLockActive"] = &TES3::InputController::isCapsLockActive;
			usertypeDefinition["isControlDown"] = &TES3::InputController::isControlDown;
			usertypeDefinition["isKeyDown"] = &TES3::InputController::isKeyDown;
			usertypeDefinition["isKeyPressedThisFrame"] = &TES3::InputController::isKeyPressedThisFrame;
			usertypeDefinition["isKeyReleasedThisFrame"] = &TES3::InputController::isKeyReleasedThisFrame;
			usertypeDefinition["isMouseButtonDown"] = &TES3::InputController::isMouseButtonDown;
			usertypeDefinition["isMouseButtonPressedThisFrame"] = &TES3::InputController::isMouseButtonPressedThisFrame;
			usertypeDefinition["isMouseButtonReleasedThisFrame"] = &TES3::InputController::isMouseButtonReleasedThisFrame;
			usertypeDefinition["isShiftDown"] = &TES3::InputController::isShiftDown;
			usertypeDefinition["isSuperDown"] = &TES3::InputController::isSuperDown;
			usertypeDefinition["keybindTest"] = &TES3::InputController::keybindTest_lua;
		}
	}
}
