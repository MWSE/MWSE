#include "TES3InputController.h"

namespace TES3 {
	std::reference_wrapper<BYTE[8]> InputController::DirectInputMouseState::getButtons() {
		return std::ref(rgbButtons);
	}

	const auto TES3_InputController_readKeyState = reinterpret_cast<void(__thiscall*)(InputController*)>(0x4065E0);
	void InputController::readKeyState() {
		TES3_InputController_readKeyState(this);
	}

	const auto TES3_InputController_readButtonPressed = reinterpret_cast<int(__thiscall*)(InputController*, DWORD*)>(0x406950);
	int InputController::readButtonPressed(DWORD* data) {
		return TES3_InputController_readButtonPressed(this, data);
	}

	const auto TES3_InputController_keybindTest = reinterpret_cast<int(__thiscall*)(const InputController*, unsigned int, unsigned int)>(0x406F40);
	bool InputController::keybindTest(unsigned int keyBind, unsigned int transition) const {
		return TES3_InputController_keybindTest(this, keyBind, transition);
	}

	bool InputController::isKeyDown(unsigned char keyCode) const {
		return keyboardState[keyCode] & 0x80;
	}

	bool InputController::isKeyPressedThisFrame(unsigned char keyCode) const {
		return (keyboardState[keyCode] & 0x80) && !(previousKeyboardState[keyCode] & 0x80);
	}

	bool InputController::isKeyReleasedThisFrame(unsigned char keyCode) const {
		return !(keyboardState[keyCode] & 0x80) && (previousKeyboardState[keyCode] & 0x80);
	}

	bool InputController::isMouseButtonDown(unsigned char button) const {
		return mouseState.rgbButtons[button] & 0x80;
	}

	bool InputController::isMouseButtonPressedThisFrame(unsigned char button) const {
		return (mouseState.rgbButtons[button] & 0x80) && !(previousMouseState.rgbButtons[button] & 0x80);
	}

	bool InputController::isMouseButtonReleasedThisFrame(unsigned char button) const {
		return !(mouseState.rgbButtons[button] & 0x80) && (previousMouseState.rgbButtons[button] & 0x80);
	}

	bool InputController::isAltDown() const {
		return (keyboardState[DIK_LALT] & 0x80) || (keyboardState[DIK_RALT]);
	}

	bool InputController::isCapsLockActive() const {
		return (GetKeyState(VK_CAPITAL) & 0x0001);
	}

	bool InputController::isControlDown() const {
		return (keyboardState[DIK_LCONTROL] & 0x80) || (keyboardState[DIK_RCONTROL]);
	}

	bool InputController::isShiftDown() const {
		return (keyboardState[DIK_LSHIFT] & 0x80) || (keyboardState[DIK_RSHIFT]);
	}

	bool InputController::isSuperDown() const {
		return (keyboardState[DIK_LWIN] & 0x80) || (keyboardState[DIK_RWIN]);
	}

	bool InputController::keybindTest_lua(unsigned int key, sol::optional<unsigned int> transition) const {
		return keybindTest(key, transition.value_or(TES3::KeyTransition::Down));
	}

	std::reference_wrapper<unsigned char[256]> InputController::getKeyboardState() {
		return std::ref(keyboardState);
	}

	std::reference_wrapper<unsigned char[256]> InputController::getPreviousKeyboardState() {
		return std::ref(previousKeyboardState);
	}

	std::reference_wrapper<InputConfig[34]> InputController::getInputConfigs() {
		return std::ref(inputMaps);
	}
}
