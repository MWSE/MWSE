#include "DialogUseReportWindow.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "WinUIUtil.h"

#include "CSBaseObject.h"

#include "DialogProcContext.h"

#include "WindowMain.h"

namespace se::cs::dialog::use_report_window {
	void PatchDialogProc_BeforeNotify_FromObjectsList_DoubleLeftClick(DialogProcContext& context) {
		using winui::ListView_GetItemData;

		const auto clickData = (LPNMITEMACTIVATE)context.getLParam();
		const auto object = reinterpret_cast<BaseObject*>(ListView_GetItemData(clickData->hdr.hwndFrom, clickData->iItem, clickData->iSubItem));
		if (object) {
			window::main::showObjectEditWindow(object);
		}
	}

	void PatchDialogProc_BeforeNotify_FromObjectsList(DialogProcContext& context) {
		const auto notification = context.getNotificationData();
		switch (notification->code) {
		case NM_DBLCLK:
			PatchDialogProc_BeforeNotify_FromObjectsList_DoubleLeftClick(context);
			break;
		}
	}

	void PatchDialogProc_BeforeNotify(DialogProcContext& context) {
		const auto notification = context.getNotificationData();
		switch (notification->idFrom) {
		case CONTROL_ID_USED_BY_OBJECTS_LIST_VIEW:
			PatchDialogProc_BeforeNotify_FromObjectsList(context);
			break;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x435B80);

		switch (msg) {
		case WM_NOTIFY:
			PatchDialogProc_BeforeNotify(context);
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

		}

		return context.getResult();
	}

	void installPatches() {
		using memory::genJumpEnforced;

		// Patch: Extend handling.
		genJumpEnforced(0x402D2E, 0x435B80, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
