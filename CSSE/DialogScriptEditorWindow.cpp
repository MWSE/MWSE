#include "DialogScriptEditorWindow.h"

#include <Richedit.h>

#include "MemoryUtil.h"
#include "StringUtil.h"
#include "WinUIUtil.h"

#include "CSRecordHandler.h"
#include "CSGlobalVariable.h"

#include "Settings.h"
#include "LogUtil.h"

namespace se::cs::dialog::script_editor_window {
	GlobalVariable* __fastcall getCompilerGlobalVariable(RecordHandler* recordHandler, DWORD _EDX_, const char* id) {
		auto result = recordHandler->getGlobal(id);

		if (result == nullptr) {
			// We are trying to compile a script, and the global doesn't exist.
			// We already know by this point that no local variable exists with this name.
			if (string::equal(id, "MWSE_BUILD")) {
				result = new GlobalVariable("MWSE_BUILD");
				result->valueType = 'l';
			}/*
			else if (string::equal(id, "OPENMW_VERSION")) {
				// TODO: Change this when we know what OpenMW will name their variable.
				result = new GlobalVariable("OPENMW_VERSION");
				result->valueType = 'l';
			}*/

			if (result) {
				result->sourceFile = recordHandler->activeFile;
				result->setModified(true);
				recordHandler->globals->push_back(result);
				log::stream << "Created on-demand global variable '" << id << "'" << std::endl;
			}
		}

		return result;
	}

	void __stdcall PatchRichEditSetFormat(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		// Call original code.
		SendMessageA(hWnd, msg, wParam, lParam);

		// Set font face and size. Remove bold effect.
		CHARFORMATA format;
		format.cbSize = sizeof(CHARFORMATA);
		format.dwMask = CFM_EFFECTS | CFM_FACE | CFM_SIZE;
		format.dwEffects = 0;
		format.yHeight = 20 * settings.script_editor.font_size;
		strcpy_s(format.szFaceName, sizeof(format.szFaceName), settings.script_editor.font_face.c_str());

		SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&format);
	}

	void installPatches() {
		using memory::genCallUnprotected;
		using memory::genCallEnforced;

		genCallUnprotected(0x42D5D9, reinterpret_cast<DWORD>(PatchRichEditSetFormat), 6);
		genCallEnforced(0x560A10, 0x402F31, reinterpret_cast<DWORD>(getCompilerGlobalVariable));
	}
}
