#include "DialogPathGridWindow.h"

#include "DialogProcContext.h"

#include "MemoryUtil.h"
#include "WinUIUtil.h"

namespace se::cs::dialog::path_grid_window {
	// Set the save button as focused by default instead of the generate default button.
	void PatchDialogProc_AfterInitialize(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto hDefaultFocus = context.getDefaultFocus();
		if (GetDlgCtrlID(hDefaultFocus) != CONTROL_ID_SAVE_BUTTON) {
			SetFocus(GetDlgItem(hWnd, CONTROL_ID_SAVE_BUTTON));

			// FALSE must be returned to change keyboard focus.
			context.setResult(FALSE);
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x553660);

		switch (msg) {

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
		genJumpEnforced(0x401AAA, 0x553660, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}