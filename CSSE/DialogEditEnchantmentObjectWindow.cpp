#include "DialogEditEnchantmentObjectWindow.h"

#include "LogUtil.h"
#include "MemoryUtil.h"

#include "CSEnchantment.h"

#include "DialogProcContext.h"

namespace se::cs::dialog::edit_enchantment_object_window {

	//
	// Configuration
	//

	constexpr auto ENABLE_ALL_OPTIMIZATIONS = true;
	constexpr auto LOG_PERFORMANCE_RESULTS = false;

	//
	// Optimize insertion for combo boxes.
	//

	void setRedrawState(DialogProcContext& context, bool redraw) {
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			const auto hWnd = context.getWindowHandle();
			const auto wParam = redraw ? TRUE : FALSE;
			SendDlgItemMessageA(hWnd, CONTROL_ID_CAST_TYPE_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT1_EFFECT_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT1_EFFECT_VAR_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT1_RANGE_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT2_EFFECT_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT2_EFFECT_VAR_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT2_RANGE_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT3_EFFECT_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT3_EFFECT_VAR_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT3_RANGE_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT4_EFFECT_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT4_EFFECT_VAR_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT4_RANGE_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT5_EFFECT_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT5_EFFECT_VAR_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT5_RANGE_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT6_EFFECT_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT6_EFFECT_VAR_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT6_RANGE_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT7_EFFECT_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT7_EFFECT_VAR_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT7_RANGE_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT8_EFFECT_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT8_EFFECT_VAR_COMBO, WM_SETREDRAW, wParam, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_EFFECT8_RANGE_COMBO, WM_SETREDRAW, wParam, NULL);
		}
	}

	//
	// Extended window messages.
	//

	std::chrono::high_resolution_clock::time_point initializationTimer;

	void PatchDialogProc_BeforeInitialize(DialogProcContext& context) {
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			initializationTimer = std::chrono::high_resolution_clock::now();
		}

		// Optimize redraws.
		setRedrawState(context, false);
	}

	void PatchDialogProc_AfterInitialize(DialogProcContext& context) {
		// Restore redraws.
		setRedrawState(context, true);

		if constexpr (LOG_PERFORMANCE_RESULTS) {
			auto timeToInitialize = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initializationTimer);
			log::stream << "Displaying enchantment object data took " << timeToInitialize.count() << "ms" << std::endl;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x433A40);

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
		}

		return context.getResult();
	}

	//
	//
	//

	void installPatches() {
		using memory::genJumpEnforced;

		// Patch: Extend handling.
		genJumpEnforced(0x404912, 0x433A40, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
