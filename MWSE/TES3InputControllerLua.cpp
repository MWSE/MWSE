#include "TES3InputControllerLua.h"

#include "LuaManager.h"

#include "TES3InputController.h"

namespace mwse {
	namespace lua {
		void bindTES3InputController() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for TES3::InputConfig
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::InputConfig>("tes3inputConfig");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["code"] = &TES3::InputConfig::keyCode;
				usertypeDefinition["device"] = &TES3::InputConfig::device;
			}

			// Binding for TES3::InputController::MouseState (DIMOUSESTATE2)
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::InputController::DirectInputMouseState>("tes3directInputMouseState");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["x"] = &TES3::InputController::DirectInputMouseState::lX;
				usertypeDefinition["y"] = &TES3::InputController::DirectInputMouseState::lY;
				usertypeDefinition["z"] = &TES3::InputController::DirectInputMouseState::lZ;

				// Indirect bindings to unions and arrays.
				usertypeDefinition["buttons"] = sol::readonly_property(&TES3::InputController::DirectInputMouseState::getButtons);
			}

			// Binding for TES3::InputController
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::InputController>("tes3inputController");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["creationFlags"] = sol::readonly_property(&TES3::InputController::creationFlags);
				usertypeDefinition["inputMaps"] = sol::readonly_property(&TES3::InputController::getInputConfigs);
				usertypeDefinition["keyboardState"] = sol::readonly_property(&TES3::InputController::getKeyboardState);
				usertypeDefinition["mouseState"] = sol::readonly_property(&TES3::InputController::mouseState);
				usertypeDefinition["previousKeyboardState"] = sol::readonly_property(&TES3::InputController::getPreviousKeyboardState);
				usertypeDefinition["previousMouseState"] = sol::readonly_property(&TES3::InputController::previousMouseState);

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
}
