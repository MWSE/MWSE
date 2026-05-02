#include "DialogActorAIWindow.h"

#include "CSCell.h"
#include "CSDataHandler.h"
#include "CSRecordHandler.h"

#include "DialogProcContext.h"

#include "LogUtil.h"

namespace se::cs::dialog::actor_ai_window {

	constexpr auto ENABLE_ALL_OPTIMIZATIONS = true;
	constexpr auto LOG_PERFORMANCE_RESULTS = false;

	//
	// Main patching function.
	//

	auto initializationTimer = std::chrono::high_resolution_clock::now();

	void CALLBACK PatchDialogProc_BeforeInit(DialogProcContext& context) {
		// Begin measure of initialization time.
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			initializationTimer = std::chrono::high_resolution_clock::now();
		}

		// Disable redraw.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			const auto hWnd = context.getWindowHandle();
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_ADD_PACKAGE_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_TRAVEL_SERVICE_1_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_TRAVEL_SERVICE_2_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_TRAVEL_SERVICE_3_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_TRAVEL_SERVICE_4_COMBO), WM_SETREDRAW, FALSE, NULL);
		}
	}

	void CALLBACK PatchDialogProc_AfterInit(DialogProcContext& context) {
		// Reenable redraw.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			const auto hWnd = context.getWindowHandle();
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_ADD_PACKAGE_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_TRAVEL_SERVICE_1_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_TRAVEL_SERVICE_2_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_TRAVEL_SERVICE_3_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_TRAVEL_SERVICE_4_COMBO), WM_SETREDRAW, TRUE, NULL);
		}

		// Finish measure of initialization time.
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			auto timeToInitialize = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initializationTimer);
			log::stream << "Total actor AI data window initialization time: " << timeToInitialize.count() << "ms" << std::endl;
		}
	}

	void CALLBACK PatchDialogProc_BeforeCommand_FromTravelReturnButton(DialogProcContext& context) {
		const auto userData = context.getUserData<UserData>();

		// Prevent any command from doing anything if we have no cell.
		if (userData->returnCell == nullptr) {
			context.setResult(TRUE);
			return;
		}
	}

	void CALLBACK PatchDialogProc_BeforeCommand(DialogProcContext& context) {
		const auto wParam = context.getWParam();
		const auto command = HIWORD(wParam);
		const auto id = LOWORD(wParam);

		switch (id) {
		case CONTROL_ID_TRAVEL_SERVICE_RETURN_BUTTON:
			PatchDialogProc_BeforeCommand_FromTravelReturnButton(context);
			break;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x4A7900);

		// Handle pre-patches.
		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_BeforeInit(context);
			break;
		case WM_COMMAND:
			PatchDialogProc_BeforeCommand(context);
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
			PatchDialogProc_AfterInit(context);
			break;
		}

		return context.getResult();
	}

	void installPatches() {
		using memory::genJumpEnforced;
		using memory::genNOPUnprotected;

		// Patch: Extend Object Window message handling.
		genJumpEnforced(0x403378, 0x4A7900, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}