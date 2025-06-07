#include "DialogUseReportWindow.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "WinUIUtil.h"

#include "CSBaseObject.h"

#include "DialogProcContext.h"

#include "WindowMain.h"

namespace se::cs::dialog::use_report_window {
	void showUseReport(const BaseObject* object) {
		const auto hInstance = window::main::hInstance::get();
		const auto ghWnd = window::main::ghWnd::get();
		CreateDialogParamA(hInstance, (LPCSTR)0xDC, ghWnd, (DLGPROC)0x402D2E, (LPARAM)object);
	}

	void PatchDialogProc_BeforeNotify_FromObjectsList_DoubleLeftClick(DialogProcContext& context) {
		using winui::ListView_GetItemData;

		const auto clickData = (LPNMITEMACTIVATE)context.getLParam();
		const auto object = reinterpret_cast<BaseObject*>(ListView_GetItemData(clickData->hdr.hwndFrom, clickData->iItem, clickData->iSubItem));
		if (object) {
			window::main::showObjectEditWindow(object);
		}
	}

	void PatchDialogProc_BeforeNotify_FromObjectsList_KeyDown_F1(DialogProcContext& context) {
		using winui::ListView_GetItemData;

		const auto hWnd = context.getWindowHandle();
		const auto keyDownData = (LPNMLVKEYDOWN)context.getLParam();
		const auto hList = keyDownData->hdr.hwndFrom;

		const auto selected = ListView_GetNextItem(hList, -1, LVNI_SELECTED);
		if (selected == -1) {
			return;
		}

		const auto itemData = reinterpret_cast<BaseObject*>(ListView_GetItemData(hList, selected, 0));
		if (itemData == nullptr) {
			return;
		}

		showUseReport(itemData);
	}

	void PatchDialogProc_BeforeNotify_FromObjectsList_KeyDown(DialogProcContext& context) {
		const auto keyDownData = (LPNMLVKEYDOWN)context.getLParam();
		switch (keyDownData->wVKey) {
		case VK_F1:
			PatchDialogProc_BeforeNotify_FromObjectsList_KeyDown_F1(context);
			break;
		}
	}

	void PatchDialogProc_BeforeNotify_FromObjectsList(DialogProcContext& context) {
		const auto notification = context.getNotificationData();
		switch (notification->code) {
		case NM_DBLCLK:
			PatchDialogProc_BeforeNotify_FromObjectsList_DoubleLeftClick(context);
			break;
		case LVN_KEYDOWN:
			PatchDialogProc_BeforeNotify_FromObjectsList_KeyDown(context);
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

	void PatchDialogProc_AfterInitDialog(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto userData = context.getUserData<UserData>();

		// Change the window title to include the object ID.
		char buffer[256] = {};
		if (sprintf_s(buffer, "Use Report: %s", userData->reportingObject->getObjectID()) > 0) {
			SetWindowTextA(hWnd, buffer);
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
		case WM_INITDIALOG:
			PatchDialogProc_AfterInitDialog(context);
			break;
		}

		return context.getResult();
	}

	void installPatches() {
		using memory::genJumpEnforced;

		// Patch: Extend handling.
		genJumpEnforced(0x402D2E, 0x435B80, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
