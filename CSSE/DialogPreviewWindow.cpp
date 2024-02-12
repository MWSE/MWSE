#include "DialogCellWindow.h"

#include "MemoryUtil.h"
#include "LogUtil.h"

#include "DialogProcContext.h"

namespace se::cs::dialog::preview_window {
	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x455AA0);

		// Handle pre-patches.
		switch (msg) {

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

		}

		return context.getResult();
	}

	void installPatches() {
		using memory::genJumpEnforced;
		using memory::writeBytesUnprotected;

		// Patch: Don't reset viewing angle when a new object is chosen.
		const BYTE patchSkipSetTransform[] = { 0xEB, 0x25 };
		writeBytesUnprotected(0x45752D, patchSkipSetTransform, 2);

		// Patch: Extend window messages.
		genJumpEnforced(0x403189, 0x455AA0, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
