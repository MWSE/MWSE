#include "DialogSearchAndReplaceWindow.h"

#include "MemoryUtil.h"
#include "LogUtil.h"

#include "DialogProcContext.h"

namespace se::cs::dialog::search_and_replace_window {
	constexpr auto ENABLE_ALL_OPTIMIZATIONS = true;
	constexpr auto LOG_PERFORMANCE_RESULTS = false;

	auto initializationTimer = std::chrono::high_resolution_clock::now();

	void setRedrawOnExpensiveWindows(HWND hWnd, bool redraw) {
		if constexpr (!ENABLE_ALL_OPTIMIZATIONS) {
			return;
		}

		const auto wParam = redraw ? TRUE : FALSE;
		SendMessageA(GetDlgItem(hWnd, CONTROL_ID_SEARCH_FOR_COMBO), WM_SETREDRAW, wParam, NULL);
		SendMessageA(GetDlgItem(hWnd, CONTROL_ID_REPLACE_WITH_COMBO), WM_SETREDRAW, wParam, NULL);
	}

	void PatchDialogProc_BeforeInitialize(DialogProcContext& context) {
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			initializationTimer = std::chrono::high_resolution_clock::now();
		}

		setRedrawOnExpensiveWindows(context.getWindowHandle(), false);
	}

	void PatchDialogProc_AfterInitialize(DialogProcContext& context) {
		setRedrawOnExpensiveWindows(context.getWindowHandle(), true);

		if constexpr (LOG_PERFORMANCE_RESULTS) {
			auto timeToInitialize = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initializationTimer);
			log::stream << "Total search & replace window initialization time: " << timeToInitialize.count() << "ms" << std::endl;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x438CE0);

		// Handle pre-patches.
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

		// Handle post-patches.
		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_AfterInitialize(context);
			break;
		}

		return context.getResult();
	}

	void installPatches() {
		using memory::genJumpEnforced;

		// Patch: Extend Object Window message handling.
		genJumpEnforced(0x4024D7, 0x438CE0, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
