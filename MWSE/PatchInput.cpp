#include "PatchInput.h"

#include "MemoryUtil.h"
#include "MWSEConfig.h"
#include "WindowsUtil.h"

#include "TES3InputController.h"
#include "TES3WorldController.h"

namespace mwse::patch::input {
	//
	// Patch: Expand keyboard key translations
	//

	inline static void WritePatchKeyCharacter(unsigned int key, char character) {
		se::memory::writeByteUnprotected(0x775148 + key, character); // US, Unshifted
		se::memory::writeByteUnprotected(0x775248 + key, character); // US, Shifted
		se::memory::writeValueEnforced<char>(0x775348 + key, 0, character); // DE, Unshifted
		se::memory::writeValueEnforced<char>(0x775448 + key, 0, character); // DE, Shifted
		se::memory::writeValueEnforced<char>(0x775548 + key, 0, character); // FR, Unshifted
		se::memory::writeValueEnforced<char>(0x775648 + key, 0, character); // FR, Shifted
	}

	static void PatchExpandKeyboardCharacterTranslations() {
		WritePatchKeyCharacter(DIK_NUMPAD0, '0');
		WritePatchKeyCharacter(DIK_NUMPAD1, '1');
		WritePatchKeyCharacter(DIK_NUMPAD2, '2');
		WritePatchKeyCharacter(DIK_NUMPAD3, '3');
		WritePatchKeyCharacter(DIK_NUMPAD4, '4');
		WritePatchKeyCharacter(DIK_NUMPAD5, '5');
		WritePatchKeyCharacter(DIK_NUMPAD6, '6');
		WritePatchKeyCharacter(DIK_NUMPAD7, '7');
		WritePatchKeyCharacter(DIK_NUMPAD8, '8');
		WritePatchKeyCharacter(DIK_NUMPAD9, '9');
		WritePatchKeyCharacter(DIK_NUMPADEQUALS, '=');
		WritePatchKeyCharacter(DIK_NUMPADMINUS, '-');
		WritePatchKeyCharacter(DIK_NUMPADPERIOD, '.');
		WritePatchKeyCharacter(DIK_NUMPADPLUS, '+');
		WritePatchKeyCharacter(DIK_NUMPADSLASH, '/');
		WritePatchKeyCharacter(DIK_NUMPADSTAR, '*');
	}

	//
	// Patch: Be better about showing/hiding the cursor.
	//

	static WNDPROC originalWindowProc = nullptr;
	using gCursorShown = se::memory::ExternalGlobal<bool, 0x776D0C>;
	static bool showCursorFlag = true;

	static void SetShowCursorState(bool shown, bool force = false) {
		if (!force && showCursorFlag == shown) {
			return;
		}

		if (shown) {
			while (ShowCursor(TRUE) < 0);
		}
		else {
			while (ShowCursor(FALSE) >= 0);
		}
		showCursorFlag = shown;
	}

	static void SetCursorShown(HWND hWnd, bool shown, bool forceShow = false) {
		gCursorShown::set(shown);
		SetShowCursorState(shown, forceShow && shown);

		const auto worldController = TES3::WorldController::get();
		if (!worldController) {
			return;
		}

		const auto inputController = worldController->inputController;
		if (!inputController) {
			return;
		}

		// Sync mouse state.
		if (inputController->mouse) {
			if (shown) {
				inputController->mouse->Unacquire();
			}
			else if (GetForegroundWindow() == hWnd) {
				inputController->mouse->Acquire();
			}
		}

		// Sync keyboard state.
		if (inputController->keyboard) {
			if (shown) {
				inputController->keyboard->Unacquire();
			}
			else if (GetForegroundWindow() == hWnd) {
				inputController->keyboard->Acquire();
			}
		}
	}

	static void PatchWindProc_CursorHitTest(se::windows::DialogProcContext& context) {
		context.callOriginalFunction();
		const auto hWnd = context.getWindowHandle();
		const auto result = context.getResult();
		auto shouldShow = result != HTCLIENT;

		SetCursorShown(hWnd, shouldShow || GetForegroundWindow() != hWnd);
	}

	static LRESULT __stdcall PatchWindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		se::windows::DialogProcContext context(hWnd, msg, wParam, lParam, (DWORD)originalWindowProc);

		switch (msg) {
		case WM_ACTIVATE:
			SetCursorShown(hWnd, context.getLOWParam() == WA_INACTIVE, true);
			break;
		case WM_ACTIVATEAPP:
			SetCursorShown(hWnd, !wParam, true);
			break;
		case WM_SETFOCUS:
			SetCursorShown(hWnd, false);
			break;
		case WM_KILLFOCUS:
			SetCursorShown(hWnd, true, true);
			break;
		case WM_NCHITTEST:
			PatchWindProc_CursorHitTest(context);
			break;
		}

		if (!context.hasResult()) {
			context.callOriginalFunction();
		}

		return context.getResult();
	}

	//
	// Patch: Make Morrowind believe that it is always the front window in the main gameplay loop block.
	//

	static HWND lastForegroundWindow = 0;
	HWND __stdcall PatchGetActiveWindowForMainLoop() {
		const auto worldController = TES3::WorldController::get();
		const auto mainWindowHandle = worldController->Win32_hWndParent;

		const auto foregroundWindow = GetForegroundWindow();
		if (foregroundWindow != mainWindowHandle && foregroundWindow != lastForegroundWindow) {
			auto inputController = worldController->inputController;
			if (inputController) {
				inputController->clearTransientInputState();
				if (inputController->mouse) {
					inputController->mouse->Unacquire();
				}
				if (inputController->keyboard) {
					inputController->keyboard->Unacquire();
				}
			}
		}

		lastForegroundWindow = foregroundWindow;

		if (Configuration::RunInBackground) {
			return mainWindowHandle;
		}
		return GetActiveWindow();
	}

	void __fastcall PatchGetMorrowindMainWindow_NoBackgroundInput(TES3::InputController* inputController) {
		if (GetForegroundWindow() != TES3::WorldController::get()->Win32_hWndParent) {
			return;
		}

		inputController->readKeyState();
	}

	void install() {
		// Patch: Expand keyboard key translations
		PatchExpandKeyboardCharacterTranslations();

		// Patch: Store last read key state.
		auto InputController_readButtonPressed = &TES3::InputController::readButtonPressed;
		se::memory::genCallEnforced(0x58E8C6, 0x406950, *reinterpret_cast<DWORD*>(&InputController_readButtonPressed));
		se::memory::genCallEnforced(0x5BCA1D, 0x406950, *reinterpret_cast<DWORD*>(&InputController_readButtonPressed));
	}

	void installPostLua() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;
		using se::memory::genPushEnforced;
		using se::memory::writeByteUnprotected;

		// Patch: Be better about showing/hiding the cursor.
		originalWindowProc = (WNDPROC)SetWindowLongPtr(TES3::WorldController::get()->Win32_hWndParent, GWLP_WNDPROC, (LONG_PTR)PatchWindProc);

		// Patch: Reset input state when focus changes outside Morrowind, even if it happened while paused in a debugger.
		genCallUnprotected(0x41AB7D, reinterpret_cast<DWORD>(PatchGetActiveWindowForMainLoop), 0x6);

		// Patch: The window is never out of focus.
		if (Configuration::RunInBackground) {
			writeByteUnprotected(0x416BC3 + 0x2 + 0x4, 1);
			genCallEnforced(0x425313, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x4772CE, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x47798C, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x477E1E, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x5BC9E1, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x5BCA33, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
		}

		// Patch: Use non-exclusive keyboard cooperative levels and allow shell keys.
		if (Configuration::NonExclusiveKeyboard) {
			genPushEnforced(0x40627D, BYTE(DISCL_BACKGROUND | DISCL_NONEXCLUSIVE));
			genPushEnforced(0x406291, BYTE(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
		}
	}
}
