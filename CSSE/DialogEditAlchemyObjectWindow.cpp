#include "DialogEditAlchemyObjectWindow.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "WinUIUtil.h"

#include "DialogProcContext.h"

namespace se::cs::dialog::edit_alchemy_object_window {

	//
	// Configuration
	//

	constexpr auto ENABLE_ALL_OPTIMIZATIONS = true;
	constexpr auto LOG_PERFORMANCE_RESULTS = false;

	//
	// Extended window messages.
	//

	std::chrono::high_resolution_clock::time_point initializationTimer;

	void PatchDialogProc_BeforeInitialize(DialogProcContext& context) {
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			initializationTimer = std::chrono::high_resolution_clock::now();
		}

		// Optimize redraws.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			const auto hWnd = context.getWindowHandle();
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT1_EFFECT_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT1_EFFECT_VAR_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT2_EFFECT_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT2_EFFECT_VAR_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT3_EFFECT_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT3_EFFECT_VAR_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT4_EFFECT_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT4_EFFECT_VAR_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT5_EFFECT_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT5_EFFECT_VAR_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT6_EFFECT_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT6_EFFECT_VAR_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT7_EFFECT_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT7_EFFECT_VAR_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT8_EFFECT_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT8_EFFECT_VAR_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_SCRIPT_COMBO, WM_SETREDRAW, FALSE, NULL);
		}
	}

	void PatchDialogProc_AfterInitialize(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();

		// Remove the icon frame so it doesn't look so weird with transparent icons.
		const auto hIconFrame = GetDlgItem(hWnd, CONTROL_ID_ICON_FRAME);
		if (hIconFrame) {
			ShowWindow(hIconFrame, FALSE);
		}

		// Make the texture icon clickable.
		const auto hIcon = GetDlgItem(hWnd, CONTROL_ID_ICON_TEXTURE);
		if (hIcon) {
			winui::RemoveStyles(hIcon, WS_DISABLED);
		}

		// Restore redraws.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT1_EFFECT_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT1_EFFECT_VAR_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT2_EFFECT_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT2_EFFECT_VAR_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT3_EFFECT_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT3_EFFECT_VAR_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT4_EFFECT_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT4_EFFECT_VAR_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT5_EFFECT_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT5_EFFECT_VAR_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT6_EFFECT_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT6_EFFECT_VAR_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT7_EFFECT_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT7_EFFECT_VAR_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT8_EFFECT_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT8_EFFECT_VAR_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_SCRIPT_COMBO, WM_SETREDRAW, TRUE, NULL);
		}

		if constexpr (LOG_PERFORMANCE_RESULTS) {
			const auto timeToInitialize = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initializationTimer);
			log::stream << "Displaying alchemy object data took " << timeToInitialize.count() << "ms" << std::endl;
		}
	}

	void PatchDialogProc_AfterCommand_OnIconTexture_ButtonClicked(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();

		const auto hInventoryImageButton = GetDlgItem(hWnd, CONTROL_ID_INVENTORY_TEXTURE_BUTTON);
		if (!hInventoryImageButton) {
			return;
		}

		if (winui::IsDisabled(hInventoryImageButton)) {
			return;
		}

		SendMessageA(hWnd, WM_COMMAND, MAKEWPARAM(CONTROL_ID_INVENTORY_TEXTURE_BUTTON, BN_CLICKED), (LPARAM)hInventoryImageButton);
	}

	void PatchDialogProc_AfterCommand_OnIconTexture(DialogProcContext& context) {
		const auto code = context.getCommandNotificationCode();

		switch (code) {
		case BN_CLICKED:
			PatchDialogProc_AfterCommand_OnIconTexture_ButtonClicked(context);
			break;
		}
	}

	void PatchDialogProc_AfterCommand(DialogProcContext& context) {
		const auto id = context.getCommandControlIdentifier();

		switch (id) {
		case CONTROL_ID_ICON_TEXTURE:
			PatchDialogProc_AfterCommand_OnIconTexture(context);
			break;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x434940);

		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_BeforeInitialize(context);
			break;
		}

		// Call original function, or return early if we already have a result.
		if (context.hasResult()) {
			return context.getResult();
		}
		else {
			context.callOriginalFunction();
		}

		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_AfterInitialize(context);
			break;
		case WM_COMMAND:
			PatchDialogProc_AfterCommand(context);
			break;
		}

		return context.getResult();
	}

	//
	//
	//

	void installPatches() {
		using memory::genJumpEnforced;

		// Patch: Extend handling.
		genJumpEnforced(0x4035BC, 0x434940, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
