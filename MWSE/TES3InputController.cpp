#include "TES3InputController.h"

#include "LuaManager.h"

#include "BitUtil.h"

#include "MWSEConfig.h"
#include "TES3WorldController.h"

#include "LuaKeybindTestedEvent.h"

namespace TES3 {
	std::reference_wrapper<BYTE[8]> InputController::DirectInputMouseState::getButtons() {
		return std::ref(rgbButtons);
	}

	DIDEVICEOBJECTDATA InputController::lastReadKeyboardData = {};

	static bool isSupportedDevice(const DIDEVICEINSTANCEA* device) {
		// Hard-supported types.
		switch (device->dwDevType) {
		case DI8DEVTYPE_JOYSTICK:
		case DI8DEVTYPE_GAMEPAD:
		case DI8DEVTYPE_1STPERSON:
		case DI8DEVTYPE_DRIVING:
		case DI8DEVTYPE_FLIGHT:
		case DI8DEVTYPE_SUPPLEMENTAL:
			return true;
		}

		// Also recognize HID Generic Desktop joysticks/gamepads.
		if (device->wUsagePage == HID_USAGE_PAGE_GENERIC && (device->wUsage == HID_USAGE_GENERIC_JOYSTICK || device->wUsage == HID_USAGE_GENERIC_GAMEPAD)) {
			return true;
		}

		return false;
	}

	static BOOL CALLBACK EnumAxisCallback(const DIDEVICEOBJECTINSTANCEA* object, InputController::DeviceAxisSupport* context) {
		const GUID* axes[] = { &GUID_XAxis, &GUID_YAxis, &GUID_ZAxis, &GUID_RxAxis, &GUID_RyAxis, &GUID_RzAxis };
		for (auto axis = 0u; axis < std::size(axes); ++axis) {
			if (IsEqualGUID(object->guidType, *axes[axis])) {
				BIT_SET_ON(context->axesSupported, axis);
			}
		}

		if (object->dwType & DIDFT_BUTTON) {
			const auto button = DIDFT_GETINSTANCE(object->dwType);
			if (button < 32) {
				BIT_SET_ON(context->deviceTypeFlag, button);
			}
		}

		return DIENUM_CONTINUE;
	}

	BOOL CALLBACK InputController::gamepadEnumCallback(const DIDEVICEINSTANCEA* device, InputController* context) {
		if (context->deviceCount >= std::size(context->gamepadsPointers)) {
			return DIENUM_STOP;
		}

		if (!isSupportedDevice(device)) {
			return DIENUM_CONTINUE;
		}

		// Enabling joysticks again must not register the same device twice.
		for (auto i = 0u; i < context->deviceCount; ++i) {
			if (IsEqualGUID(context->gamepads[i].guidInstance, device->guidInstance)) {
				return DIENUM_CONTINUE;
			}
		}

		LPDIRECTINPUTDEVICE8 gamepad = nullptr;
		if (FAILED(context->directInputInterface->CreateDevice(device->guidInstance, &gamepad, nullptr))) {
			return DIENUM_CONTINUE;
		}

		DIDEVCAPS capabilities = {};
		capabilities.dwSize = sizeof(capabilities);
		if (FAILED(gamepad->SetCooperativeLevel(GetActiveWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)) || FAILED(gamepad->SetDataFormat(&c_dfDIJoystick)) || FAILED(gamepad->GetCapabilities(&capabilities))) {
			gamepad->Release();
			return DIENUM_CONTINUE;
		}

		const auto index = context->deviceCount;
		auto& axisSupport = context->gamepadAxisSupport[index];
		axisSupport.axesSupported = 0;
		axisSupport.deviceTypeFlag = 0;
		if (FAILED(gamepad->EnumObjects((LPDIENUMDEVICEOBJECTSCALLBACK)EnumAxisCallback, &axisSupport, DIDFT_ALL))) {
			gamepad->Release();
			return DIENUM_CONTINUE;
		}

		// Not entirely supported by our expanded device selection.
		DIPROPRANGE range = {};
		range.diph.dwSize = sizeof(range);
		range.diph.dwHeaderSize = sizeof(range.diph);
		range.diph.dwHow = DIPH_DEVICE;
		range.lMin = -100;
		range.lMax = 100;
		gamepad->SetProperty(DIPROP_RANGE, &range.diph);
		DIPROPDWORD deadZone = {};
		deadZone.diph.dwSize = sizeof(deadZone);
		deadZone.diph.dwHeaderSize = sizeof(deadZone.diph);
		deadZone.diph.dwHow = DIPH_DEVICE;
		deadZone.dwData = 1000;
		gamepad->SetProperty(DIPROP_DEADZONE, &deadZone.diph);

		context->gamepads[index] = *device;
		context->gamepadCapabilities[index] = capabilities;
		context->gamepadsPointers[index] = gamepad;
		++context->deviceCount;

		return DIENUM_CONTINUE;
	}

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
		if (mwse::Configuration::RunInBackground && GetForegroundWindow() != TES3::WorldController::get()->Win32_hWndParent) {
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

	std::reference_wrapper<BYTE[32]> InputController::DirectInputGamepadState::getButtons() {
		return std::ref(rgbButtons);
	}

	std::reference_wrapper<LONG[2]> InputController::DirectInputGamepadState::getSliders() {
		return std::ref(rglSlider);
	}

	std::reference_wrapper<DWORD[4]> InputController::DirectInputGamepadState::getPovs() {
		return std::ref(rgdwPOV);
	}

	size_t InputController::getGamepadCount() const {
		return std::clamp(deviceCount, 0u, 8u);
	}

	std::reference_wrapper<InputController::GamepadState[8]> InputController::getGamepadStates() {
		return std::ref(gamepadStates);
	}

	bool InputController::isGamepadButtonDown(unsigned int gamepad, unsigned int button) const {
		return (gamepadStates[gamepad].current.rgbButtons[button] & 0x80);
	}

	bool InputController::wasGamepadButtonDownLastFrame(unsigned int gamepad, unsigned int button) const {
		return (gamepadStates[gamepad].previous.rgbButtons[button] & 0x80);
	}

	bool InputController::isGamepadButtonPressedThisFrame(unsigned int gamepad, unsigned int button) const {
		return isGamepadButtonDown(gamepad, button) && !wasGamepadButtonDownLastFrame(gamepad, button);
	}

	bool InputController::isGamepadButtonReleasedThisFrame(unsigned int gamepad, unsigned int button) const {
		return !isGamepadButtonDown(gamepad, button) && wasGamepadButtonDownLastFrame(gamepad, button);
	}

	void InputController::clearTransientInputState() {
		memset(gamepadStates, 0, sizeof(gamepadStates));
		for (auto& gamepad : gamepadStates) {
			std::fill_n(gamepad.current.rgdwPOV, 4, 0xFFFFFFFF);
			std::fill_n(gamepad.previous.rgdwPOV, 4, 0xFFFFFFFF);
		}
		memset(keyboardState, 0, sizeof(keyboardState));
		memset(previousKeyboardState, 0, sizeof(previousKeyboardState));
		memset(&mouseState, 0, sizeof(mouseState));
		memset(&previousMouseState, 0, sizeof(previousMouseState));
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
