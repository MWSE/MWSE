#include "DialogTextSearchWindow.h"

#include "CSBirthsign.h"
#include "CSClass.h"
#include "CSDataHandler.h"
#include "CSDialogue.h"
#include "CSDialogueInfo.h"
#include "CSFaction.h"
#include "CSGameSetting.h"
#include "CSRace.h"
#include "CSRecordHandler.h"
#include "CSScript.h"
#include "CSSpell.h"

#include "NIIteratedList.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "StringUtil.h"
#include "WinUIUtil.h"

#include "WindowMain.h"

#include "DialogDialogueWindow.h"
#include "DialogUseReportWindow.h"

#include "Settings.h"

#include "DialogProcContext.h"

#include "EditBasicExtended.h"

namespace se::cs::dialog::text_search_window {
	constexpr auto LOG_PERFORMANCE_RESULTS = false;
	constexpr auto REPLACE_SEARCH_LOGIC = true;

	struct DialogueResult {
		Dialogue* dialogue; // 0x0
		NI::IteratedList<DialogueInfo*> info; // 0x4

		void* operator new(size_t size) {
			return memory::_new(size);
		}

		void operator delete(void* address) {
			memory::_delete(address);
		}
	};
	static_assert(sizeof(DialogueResult) == 0x18, "DialogueResult failed size validation");

	//
	// Patch: Improve text search.
	//

	static std::regex TextSearchRegex;

	bool TextSearchGatherObjectResults(const std::string_view& needle, NI::IteratedList<BaseObject*>* results, const BaseObject::SearchSettings& settings, std::regex* regex) {
		const auto recordHandler = DataHandler::get()->recordHandler;

		// Search game settings.
		for (const auto& setting : recordHandler->gameSettingsHandler->gameSettings) {
			if (setting->search(needle, settings, regex)) {
				results->push_back(setting);
			}
		}

		// Search classes.
		for (const auto& _class : *recordHandler->classes) {
			if (_class->search(needle, settings, regex)) {
				results->push_back(_class);
			}
		}

		// Search races.
		for (const auto& race : *recordHandler->races) {
			if (race->search(needle, settings, regex)) {
				results->push_back(race);
			}
		}

		// Search factions.
		for (const auto& faction : *recordHandler->factions) {
			if (faction->search(needle, settings, regex)) {
				results->push_back(faction);
			}
		}

		// Search birthsigns.
		for (const auto& birthsign : *recordHandler->birthsigns) {
			if (birthsign->search(needle, settings, regex)) {
				results->push_back(birthsign);
			}
		}

		// Search spells.
		for (const auto& spell : *recordHandler->allSpells) {
			if (spell->search(needle, settings, regex)) {
				results->push_back(spell);
			}
		}

		// Search all other general objects.
		for (const auto& object : *recordHandler->allObjects) {
			if (object->searchWithInheritance(needle, settings, regex)) {
				results->push_back(object);
			}
		}

		return !results->empty();
	}

	bool __cdecl TextSearchGatherResults(const char* searchString, NI::IteratedList<Script*>* scriptResults, NI::IteratedList<DialogueResult*>* dialogueResults, NI::IteratedList<BaseObject*>* objectResults) {
		std::chrono::high_resolution_clock::time_point searchTimer;
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			searchTimer = std::chrono::high_resolution_clock::now();
		}

		if constexpr (REPLACE_SEARCH_LOGIC) {
			if (searchString == nullptr || *searchString == '\0') {
				return false;
			}

			const std::string_view needle = searchString;
			const auto caseSensitive = settings.text_search.search_settings.case_sensitive;
			std::regex* regex = nullptr;
			if (settings.text_search.search_settings.use_regex) {
				auto flags = std::regex_constants::extended | std::regex_constants::optimize | std::regex_constants::nosubs;
				if (!caseSensitive) {
					flags |= std::regex_constants::icase;
				}

				try {
					TextSearchRegex = std::regex(searchString, flags);
					regex = &TextSearchRegex;
				}
				catch (const std::regex_error& e) {
					log::stream << "Regex error when performing text search: " << e.what() << std::endl;
					// TODO: Paint the background of the search input red or something.
				}
			}

			// Search scripts.
			const auto recordHandler = DataHandler::get()->recordHandler;
			for (const auto& script : *recordHandler->scripts) {
				if (script->search(needle, settings.text_search.search_settings, regex)) {
					scriptResults->push_back(script);
				}
			}

			// Search dialogues.
			for (const auto& dialogue : *recordHandler->dialogues) {
				DialogueResult* result = nullptr;

				if (dialogue->search(needle, settings.text_search.search_settings, regex)) {
					if (result == nullptr) {
						result = new DialogueResult();
						result->dialogue = dialogue;
					}
				}

				for (const auto& info : dialogue->infos) {
					if (info->search(needle, settings.text_search.search_settings, regex)) {
						if (result == nullptr) {
							result = new DialogueResult();
							result->dialogue = dialogue;
						}
						result->info.push_back(info);
					}
				}

				if (result) {
					dialogueResults->push_back(result);
				}
			}

			// Search objects.
			TextSearchGatherObjectResults(needle, objectResults, settings.text_search.search_settings, regex);

		}
		else {
			const auto CS_search = reinterpret_cast<bool(__cdecl*)(const char*, NI::IteratedList<Script*>*, NI::IteratedList<DialogueResult*>*, NI::IteratedList<BaseObject*>*)>(0x4378D0);
			CS_search(searchString, scriptResults, dialogueResults, objectResults);
		}

		if constexpr (LOG_PERFORMANCE_RESULTS) {
			auto timeToSearch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - searchTimer);
			log::stream << "Searching for " << searchString << " took " << timeToSearch.count() << "ms" << std::endl;
		}

		return !scriptResults->empty() || !dialogueResults->empty() || !objectResults->empty();
	}

	//
	// Patch: Extend window messages.
	//

	void PatchDialogProc_OnNotify_DoubleClick_ObjectResult(DialogProcContext& context) {
		using namespace se::cs::winui;

		const auto lParam = context.getNotificationItemActivateData();
		const auto object = (BaseObject*)ListView_GetItemData(lParam->hdr.hwndFrom, lParam->iItem, lParam->iSubItem);
		if (object == nullptr) {
			return;
		}

		window::main::showObjectEditWindow(object);
	}

	void PatchDialogProc_OnNotify_DoubleClick_DialogueResult(DialogProcContext& context) {
		using namespace se::cs::winui;

		const auto lParam = context.getNotificationItemActivateData();
		const auto result = (DialogueResult*)ListView_GetItemData(lParam->hdr.hwndFrom, lParam->iItem, lParam->iSubItem);
		auto object = result->dialogue;

		dialog::dialogue_window::createOrFocus();
		dialog::dialogue_window::focusDialogue(object);
	}

	void PatchDialogProc_OnNotify_DoubleClick_DialogueInfoResult(DialogProcContext& context) {
		using namespace se::cs::winui;

		const auto lParam = context.getNotificationItemActivateData();
		const auto object = (DialogueInfo*)ListView_GetItemData(lParam->hdr.hwndFrom, lParam->iItem, lParam->iSubItem);

		dialog::dialogue_window::createOrFocus();
		dialog::dialogue_window::focusDialogue(object->getDialogue(), object);
	}

	void PatchDialogProc_OnNotify_DoubleClick(DialogProcContext& context) {
		auto itemActivate = context.getNotificationItemActivateData();
		switch (itemActivate->hdr.idFrom) {
		case CONTROL_ID_OBJECT_RESULTS_LIST:
			PatchDialogProc_OnNotify_DoubleClick_ObjectResult(context);
			break;
		case CONTROL_ID_TOPIC_RESULTS_LIST:
			PatchDialogProc_OnNotify_DoubleClick_DialogueResult(context);
			break;
		case CONTROL_ID_INFO_RESULTS_LIST:
			PatchDialogProc_OnNotify_DoubleClick_DialogueInfoResult(context);
			break;
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

		use_report_window::showUseReport(itemData);
	}

	void PatchDialogProc_BeforeNotify_KeyDown(DialogProcContext& context) {
		auto itemActivate = context.getNotificationItemActivateData();
		switch (itemActivate->hdr.idFrom) {
		case CONTROL_ID_OBJECT_RESULTS_LIST:
			PatchDialogProc_BeforeNotify_FromObjectsList_KeyDown_F1(context);
			break;
		}
	}

	void PatchDialogProc_OnNotify(DialogProcContext& context) {
		const auto hdr = context.getNotificationData();
		switch (hdr->code) {
		case NM_DBLCLK:
			PatchDialogProc_OnNotify_DoubleClick(context);
			break;
		case LVN_KEYDOWN:
			PatchDialogProc_BeforeNotify_KeyDown(context);
			break;
		}
	}

	void CALLBACK PatchDialogProc_AfterCreate(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();

		auto hDlgFilterEdit = GetDlgItem(hWnd, CONTROL_ID_SEARCH_EDIT);
		SetWindowSubclass(hDlgFilterEdit, ui_subclass::edit::BasicExtendedProc, NULL, NULL);
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x438610);

		switch (msg) {
		case WM_NOTIFY:
			PatchDialogProc_OnNotify(context);
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
			PatchDialogProc_AfterCreate(context);
			break;
		}

		return context.getResult();
	}

	void installPatches() {
		using namespace se::memory;

		// Patch: Extend search capabilities.
		genJumpEnforced(0x4035C6, 0x4378D0, reinterpret_cast<DWORD>(TextSearchGatherResults));

		// Patch: Extend Render Window message handling.
		genJumpEnforced(0x4046C9, 0x438610, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}