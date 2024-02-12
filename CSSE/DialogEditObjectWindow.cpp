#include "DialogEditObjectWindow.h"

#include "LogUtil.h"
#include "MemoryUtil.h"

#include "DialogEditAlchemyObjectWindow.h"
#include "DialogEditCreatureObjectWindow.h"
#include "DialogEditEnchantmentObjectWindow.h"
#include "DialogEditLeveledCreatureObjectWindow.h"
#include "DialogEditLeveledItemObjectWindow.h"
#include "DialogEditNPCObjectWindow.h"
#include "DialogEditSpellObjectWindow.h"

#include "DialogProcContext.h"

namespace se::cs::dialog::edit_object_window {

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
			SendDlgItemMessageA(context.getWindowHandle(), CONTROL_ID_SCRIPT_COMBO, WM_SETREDRAW, FALSE, NULL);
		}
	}

	void PatchDialogProc_AfterInitialize(DialogProcContext& context) {
		// Restore redraws.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			SendDlgItemMessageA(context.getWindowHandle(), CONTROL_ID_SCRIPT_COMBO, WM_SETREDRAW, TRUE, NULL);
		}

		if constexpr (LOG_PERFORMANCE_RESULTS) {
			auto timeToInitialize = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initializationTimer);
			log::stream << "Displaying default object data took " << timeToInitialize.count() << "ms" << std::endl;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x41AFD0);

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
		genJumpEnforced(0x402F9A, 0x41AFD0, reinterpret_cast<DWORD>(PatchDialogProc));

		// Extend other edit object windows.
		edit_alchemy_object_window::installPatches();
		edit_creature_object_window::installPatches();
		edit_enchantment_object_window::installPatches();
		edit_leveled_creature_object_window::installPatches();
		edit_leveled_item_object_window::installPatches();
		edit_npc_object_window::installPatches();
		edit_spell_object_window::installPatches();
	}
}
