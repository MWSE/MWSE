#pragma once

namespace TES3 {
	namespace KeyBind {
		enum KeyBind {
			Forward = 0x0,
			Back = 0x1,
			Left = 0x2,
			Right = 0x3,
			Use = 0x4,
			Activate = 0x5,
			ReadyWeapon = 0x6,
			ReadyMagic = 0x7,
			Crouch = 0x8,
			Run = 0x9,
			AlwaysRun = 0xA,
			AutoRun = 0xB,
			Jump = 0xC,
			NextWeapon = 0xD,
			PrevWeapon = 0xE,
			NextSpell = 0xF,
			PrevSpell = 0x10,
			TogglePOV = 0x11,
			MenuMode = 0x12,
			Journal = 0x13,
			Rest = 0x14,
			QuickMenu = 0x15,
			Quick1 = 0x16,
			Quick2 = 0x17,
			Quick3 = 0x18,
			Quick4 = 0x19,
			Quick5 = 0x1A,
			Quick6 = 0x1B,
			Quick7 = 0x1C,
			Quick8 = 0x1D,
			Quick9 = 0x1E,
			Quick10 = 0x1F,
			QuickSave = 0x20,
			QuickLoad = 0x21,
			Esc = 0x22,
			Console = 0x23,
			Screenshot = 0x24,
			ReadyMagicMCP = 0x25, // Used by MCP

			FindBindableKey = Forward,
			LastBindableKey = QuickLoad,

			FirstKey = Forward,
			LastKey = ReadyMagicMCP
		};
	}

	namespace KeyTransition {
		enum KeyTransition {
			Test = 0x0,
			Up = 0x1,
			Down = 0x2,
			Toggled = 0x3,
		};
	}

	namespace InputDevice {
		enum InputDevice {
			Keyboard,
			Mouse,
			Joystick,
			Gamepad,
			FirstDevice = Keyboard,
			LastDevice = Gamepad
		};
	}

	struct InputConfig {
		int keyCode;
		int device;
		int unknown_0x08;
		int unknown_0x0C;
	};
	static_assert(sizeof(InputConfig) == 0x10, "TES3::InputConfig failed size validation");

	struct InputController {
		struct DirectInputMouseState : DIMOUSESTATE2 {
			//
			// Custom functions.
			//

			std::reference_wrapper<BYTE[8]> getButtons();

		};
		struct GamepadState {
			DIJOYSTATE current;
			DIJOYSTATE previous;
		};
		struct DeviceAxisSupport {
			unsigned int axesSupported;
			unsigned int deviceTypeFlag;
		};
		struct Unknown_0x1B28 {
			int unknown_0x0;
			int unknown_0x4;
			int unknown_0x8;
		};
		unsigned int creationFlags; // 0x0
		LPDIRECTINPUT8 directInputInterface; // 0x4
		LPDIRECTINPUTDEVICE8 * gamepadsPointers[8]; // 0x8
		LPDIRECTINPUTDEVICE8 keyboard; // 0x28
		LPDIRECTINPUTDEVICE8 mouse; // 0x2C
		GamepadState gamepadStates[8]; // 0x30
		DIDEVICEINSTANCEA gamepads[8]; // 0x530
		DIDEVCAPS gamepadCapabilities[8]; // 0x1750
		DeviceAxisSupport gamepadAxisSupport[8];
		int deviceCount; // 0x18F0
		unsigned char keyboardState[256]; // 0x18F4
		unsigned char previousKeyboardState[256]; // 0x19F4
		DirectInputMouseState mouseState; // 0x1AF4
		DirectInputMouseState previousMouseState; // 0x1B08
		int unknown_0x1B1C;
		unsigned int doubleClickTime; // 0x1B20
		int unknown_0x1B24;
		Unknown_0x1B28 unknown_0x1B28;
		int unknown_0x1B34;
		char unknown_0x1B38; // Flag of some kind.
		char unknown_0x1B39; // Flag of some kind.
		char unknown_0x1B3A;
		char unknown_0x1B3B;
		InputConfig inputMaps[34]; // 0x1B3C

		//
		// Other related this-call functions.
		//

		void readKeyState();
		int readButtonPressed(DWORD* data);
		bool keybindTest(unsigned int, unsigned int) const;

		//
		// Custom functions.
		//

		bool isKeyDown(unsigned char keyCode) const;
		bool isKeyPressedThisFrame(unsigned char keyCode) const;
		bool isKeyReleasedThisFrame(unsigned char keyCode) const;

		bool isMouseButtonDown(unsigned char button) const;
		bool isMouseButtonPressedThisFrame(unsigned char button) const;
		bool isMouseButtonReleasedThisFrame(unsigned char button) const;

		bool isAltDown() const;
		bool isCapsLockActive() const;
		bool isControlDown() const;
		bool isShiftDown() const;
		bool isSuperDown() const;

		bool keybindTest_lua(unsigned int key, sol::optional<unsigned int> transition) const;

		std::reference_wrapper<unsigned char[256]> getKeyboardState();
		std::reference_wrapper<unsigned char[256]> getPreviousKeyboardState();
		std::reference_wrapper<InputConfig[34]> getInputConfigs();

	};
	static_assert(sizeof(InputController) == 0x1D5C, "TES3::InputController failed size validation");
	static_assert(sizeof(InputController::DirectInputMouseState) == sizeof(DIMOUSESTATE2), "TES3::InputController::DirectInputMouseState failed size validation");
	static_assert(sizeof(InputController::GamepadState) == 0xA0, "TES3::InputController::GamepadState failed size validation");
	static_assert(sizeof(InputController::DeviceAxisSupport) == 0x8, "TES3::InputController::DeviceAxisSupport failed size validation");
	static_assert(sizeof(InputController::Unknown_0x1B28) == 0xC, "TES3::InputController::Unknown_0x1B28 failed size validation");
}
