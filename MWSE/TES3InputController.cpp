#include "TES3InputController.h"

#include "LuaManager.h"

#include "MWSEConfig.h"
#include "TES3WorldController.h"

#include "LuaKeybindTestedEvent.h"

namespace TES3 {
	std::reference_wrapper<BYTE[8]> InputController::DirectInputMouseState::getButtons() {
		return std::ref(rgbButtons);
	}

	DIDEVICEOBJECTDATA InputController::lastReadKeyboardData = {};

	const auto TES3_InputController_readKeyState = reinterpret_cast<void(__thiscall*)(InputController*)>(0x4065E0);
	void InputController::readKeyState() {
		TES3_InputController_readKeyState(this);
	}

	const auto TES3_InputController_readButtonPressed = reinterpret_cast<int(__thiscall*)(InputController*, DWORD*)>(0x406950);
	int InputController::readButtonPressed(DWORD* out_dik) {
		if (!keyboard) {
			return 0;
		}

		DWORD readCount = 1;
		if (keyboard->GetDeviceData(sizeof(lastReadKeyboardData), &lastReadKeyboardData, &readCount, NULL) != DI_OK) {
			return 0;
		}

		if (readCount == 0) {
			return 0;
		}

		// Pretend that nothing was found if the game isn't in focus.
		if (mwse::Configuration::RunInBackground && GetActiveWindow() != TES3::WorldController::get()->Win32_hWndParent) {
			*out_dik = 0;
			return 0;
		}

		*out_dik = lastReadKeyboardData.dwOfs;

		const auto pressed = (lastReadKeyboardData.dwData & 0x80) != 0;
		return pressed ? 1 : 2;
	}

	const auto TES3_InputController_keybindTest = reinterpret_cast<BOOL(__thiscall*)(const InputController*, unsigned int, unsigned int)>(0x406F40);
	BOOL InputController::keybindTest(unsigned int keyBind, unsigned int transition) const {
		auto result = TES3_InputController_keybindTest(this, keyBind, transition);

		if (mwse::lua::event::KeybindTestedEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::KeybindTestedEvent(keyBind, transition, bool(result)));
			if (eventData.valid()) {
				if (eventData.get_or("block", false)) {
					return FALSE;
				}

				sol::optional<bool> luaResult = eventData["result"];
				result = luaResult.value_or(bool(result));
			}
		}

		return result;
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
		return isKeyDown(DIK_LALT) || isKeyDown(DIK_RALT);
	}

	bool InputController::isCapsLockActive() const {
		return (GetKeyState(VK_CAPITAL) & 0x0001);
	}

	bool InputController::isControlDown() const {
		return isKeyDown(DIK_LCONTROL) || isKeyDown(DIK_RCONTROL);
	}

	bool InputController::isShiftDown() const {
		return isKeyDown(DIK_LSHIFT) || isKeyDown(DIK_RSHIFT);
	}

	bool InputController::isSuperDown() const {
		return isKeyDown(DIK_LWIN) || isKeyDown(DIK_RWIN);
	}

	bool InputController::keybindTest_lua(unsigned int key, sol::optional<unsigned int> transition) const {
		return keybindTest(key, transition.value_or(TES3::KeyTransition::IsDown)) != FALSE;
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
