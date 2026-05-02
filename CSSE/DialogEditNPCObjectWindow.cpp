#include "DialogEditNPCObjectWindow.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "MetadataUtil.h"

#include "CSBodyPart.h"

#include "DialogProcContext.h"

namespace se::cs::dialog::edit_npc_object_window {

	//
	// Configuration
	//

	constexpr auto ENABLE_ALL_OPTIMIZATIONS = true;
	constexpr auto LOG_PERFORMANCE_RESULTS = false;

	//
	// Patch: Hide deprecated heads/hairs
	//

	ObjectType::ObjectType __fastcall PatchFilterHeadHair(ObjectType::ObjectType& objectType) {
		if (objectType != ObjectType::Bodypart) {
			return objectType;
		}

		const auto object = reinterpret_cast<BodyPart*>(reinterpret_cast<BYTE*>(&objectType) - offsetof(BaseObject, objectType));

		// Prevent deprecated objects from being added.
		if (metadata::isDeprecated(object)) {
			return ObjectType::Invalid;
		}

		// Return the right value to continue.
		return ObjectType::Bodypart;
	}


	//
	// Extended window messages.
	//

	std::chrono::high_resolution_clock::time_point initializationTimer;

	void setRedrawOnExpensiveWindows(HWND hWnd, bool redraw) {
		if constexpr (!ENABLE_ALL_OPTIMIZATIONS) {
			return;
		}

		const auto wParam = redraw ? TRUE : FALSE;
		SendDlgItemMessageA(hWnd, CONTROL_ID_BLOOD_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_CLASS_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_DETAILS_LIST, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_FACTION_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_FACTION_RANK_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_HAIR_LIST, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_HEAD_LIST, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_RACE_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_SCRIPT_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_SCRIPT_COMBO, WM_SETREDRAW, wParam, NULL);
		SendDlgItemMessageA(hWnd, CONTROL_ID_SKILLS_LIST, WM_SETREDRAW, wParam, NULL);
	}

	void PatchDialogProc_BeforeInitialize(DialogProcContext& context) {
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			initializationTimer = std::chrono::high_resolution_clock::now();
		}

		// Optimize redraws.
		setRedrawOnExpensiveWindows(context.getWindowHandle(), false);
	}

	void PatchDialogProc_AfterInitialize(DialogProcContext& context) {
		// Restore redraws.
		setRedrawOnExpensiveWindows(context.getWindowHandle(), true);

		if constexpr (LOG_PERFORMANCE_RESULTS) {
			const auto timeToInitialize = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initializationTimer);
			log::stream << "Displaying NPC object data took " << timeToInitialize.count() << "ms" << std::endl;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x423060);

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
		using memory::genCallEnforced;

		// Patch: Hide deprecated heads/hairs
		genCallEnforced(0x530D99, 0x401843, reinterpret_cast<DWORD>(PatchFilterHeadHair));

		// Patch: Extend handling.
		genJumpEnforced(0x40313E, 0x423060, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
