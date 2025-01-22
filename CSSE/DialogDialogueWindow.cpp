#include "DialogDialogueWindow.h"

#include "CSCell.h"
#include "CSDataHandler.h"
#include "CSDialogue.h"
#include "CSDialogueInfo.h"
#include "CSObject.h"
#include "CSRecordHandler.h"
#include "CSScript.h"
#include "CSActor.h"
#include "CSReference.h"

#include "LogUtil.h"
#include "StringUtil.h"
#include "WinUIUtil.h"

#include "Settings.h"

#include "EditBasicExtended.h"

#include "WindowMain.h"
#include "DialogRenderWindow.h"

#include "DialogProcContext.h"

namespace se::cs::dialog::dialogue_window {
	constexpr auto ENABLE_ALL_OPTIMIZATIONS = true;
	constexpr auto LOG_PERFORMANCE_RESULTS = false;

	void redisplayAllData(HWND hWnd) {
		// Invoke update logic by simulating the combo box selection changing.
		// There may be a better way.
		auto hFilterCombo = GetDlgItem(hWnd, CONTROL_ID_FILTER_FOR_COMBO);
		SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(CONTROL_ID_FILTER_FOR_COMBO, CBN_SELCHANGE), (LPARAM)hFilterCombo);
	}

	void clearFilters(HWND hWnd) {
		const auto userData = reinterpret_cast<UserData*>(GetWindowLongA(hWnd, GWL_USERDATA));
		if (userData == nullptr) {
			return;
		}

		userData->cellFilterMode = UserData::CellFilterMode::UseCellReference;
		userData->modeShowModifiedOnly = false;

		Button_SetCheck(GetDlgItem(hWnd, CONTROL_ID_SHOW_MODIFIED_ONLY_BUTTON), BST_UNCHECKED);
		ComboBox_SetCurSel(GetDlgItem(hWnd, CONTROL_ID_FILTER_FOR_COMBO), 0);
		ComboBox_SetCurSel(GetDlgItem(hWnd, CONTROL_ID_FILTER_CELL_SETTING_COMBO), 0);

		redisplayAllData(hWnd);
	}

	HWND createOrFocus(Actor* filter) {
		auto hWnd = ghWnd::get();
		if (hWnd) {
			SetFocus(hWnd);
			return hWnd;
		}

		return CreateDialogParamA(window::main::hInstance::get(), (LPSTR)DIALOG_ID, window::main::ghWnd::get(), (DLGPROC)0x401334, (LPARAM)filter);
	}

	HWND getActiveDialogueWindow() {
		// Get the currently active window.
		const auto hWndFocused = GetFocus();
		if (hWndFocused) {
			char buffer[64] = {};
			if (GetClassName(hWndFocused, buffer, sizeof(buffer)) && strncmp(buffer, "Dialog", 64) == 0) {
				return hWndFocused;
			}
		}

		// Fall back to the last created window.
		return ghWnd::get();
	}

	void selectTab(DialogueType type) {
		const auto hWnd = ghWnd::get();
		if (hWnd == NULL) {
			return;
		}

		const auto hTabControl = GetDlgItem(hWnd, CONTROL_ID_TOPIC_TABS);
		if (hTabControl == NULL) {
			return;
		}

		const auto index = (unsigned int)type;

		// Note: This is currently being relied on to always call the UI update code for above filter changes.
		// If optimizing this function to just use TabCtrl_SetCurSel, be sure to modify the focusDialogue function.
		winui::TabCtrl_SetCurSelEx(hTabControl, index);
	}

	void selectTab(Dialogue* dialogue) {
		selectTab(dialogue->type);
	}

	bool focusDialogue(Dialogue* dialogue, DialogueInfo* info) {
		using namespace se::cs::winui;

		const auto hWnd = ghWnd::get();
		if (hWnd == NULL) {
			return false;
		}

		const auto userData = reinterpret_cast<UserData*>(GetWindowLongA(hWnd, GWL_USERDATA));

		// If the dialog isn't modified and we're filtered to it, make sure it still shows.
		if (!dialogue->getModified() && userData->modeShowModifiedOnly) {
			userData->modeShowModifiedOnly = false;
		}

		// If the dialogue/info isn't available for the current filter actor, remove that too.
		if (userData->currentFilterObject) {
			if (info) {
				if (!info->filter(userData->currentFilterObject, nullptr, 1, dialogue)) {
					userData->currentFilterObject = nullptr;
					ComboBox_SetCurSel(GetDlgItem(hWnd, CONTROL_ID_FILTER_FOR_COMBO), 0);
				}
			}
			else {
				bool hasValidInfo = false;
				for (const auto& iterInfo : dialogue->infos) {
					if (iterInfo->filter(userData->currentFilterObject, nullptr, 1, dialogue)) {
						hasValidInfo = true;
						break;
					}
				}
				if (!hasValidInfo) {
					userData->currentFilterObject = nullptr;
					ComboBox_SetCurSel(GetDlgItem(hWnd, CONTROL_ID_FILTER_FOR_COMBO), 0);
				}
			}
		}

		// Select the right tab.
		selectTab(dialogue);

		const auto hDlgTopicList = GetDlgItem(hWnd, CONTROL_ID_TOPIC_LIST);
		if (!ListView_SelectByParam(hDlgTopicList, (LPARAM)dialogue)) {
			return false;
		}

		if (info) {
			const auto hDlgInfoList = GetDlgItem(hWnd, CONTROL_ID_INFO_LIST);
			if (!ListView_SelectByParam(hDlgInfoList, (LPARAM)info)) {
				return false;
			}
		}

		return true;
	}

	void OnCurrentTextEditChanged(HWND hWnd) {
		using namespace se::cs::winui;
		auto hDlgCurrentTextEdit = GetDlgItem(hWnd, CONTROL_ID_CURRENT_TEXT_EDIT);
		auto textCharCount = Edit_GetTextLength(hDlgCurrentTextEdit);

		SetDlgItemInt(hWnd, CONTROL_ID_CURRENT_TEXT_CHAR_COUNT, textCharCount, FALSE);
	}

	void resumeRenderingAndRepaint(HWND parent, DWORD childId) {
		// Update the current text count.
		OnCurrentTextEditChanged(parent);

		auto child = GetDlgItem(parent, childId);
		SendMessageA(child, WM_SETREDRAW, TRUE, NULL);
		RedrawWindow(child, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	void restoreInfoColumnWidths(HWND hWnd) {
		const auto infoList = GetDlgItem(hWnd, CONTROL_ID_INFO_LIST);

		ListView_SetColumnWidth(infoList, 0, settings.dialogue_window.column_text.width);
		ListView_SetColumnWidth(infoList, 1, settings.dialogue_window.column_info_id.width);
		ListView_SetColumnWidth(infoList, 2, settings.dialogue_window.column_disp_index.width);
		ListView_SetColumnWidth(infoList, 3, settings.dialogue_window.column_id.width);
		ListView_SetColumnWidth(infoList, 4, settings.dialogue_window.column_faction.width);
		ListView_SetColumnWidth(infoList, 5, settings.dialogue_window.column_cell.width);
		ListView_SetColumnWidth(infoList, 6, settings.dialogue_window.column_condition1.width);
		ListView_SetColumnWidth(infoList, 7, settings.dialogue_window.column_condition2.width);
		ListView_SetColumnWidth(infoList, 8, settings.dialogue_window.column_condition3.width);
		ListView_SetColumnWidth(infoList, 9, settings.dialogue_window.column_condition4.width);
		ListView_SetColumnWidth(infoList, 10, settings.dialogue_window.column_condition5.width);
		ListView_SetColumnWidth(infoList, 11, settings.dialogue_window.column_condition6.width);
	}


	void saveInfoColumnWidths(HWND hWnd) {
		const auto infoList = GetDlgItem(hWnd, CONTROL_ID_INFO_LIST);

		settings.dialogue_window.column_text.width = ListView_GetColumnWidth(infoList, 0);
		settings.dialogue_window.column_info_id.width = ListView_GetColumnWidth(infoList, 1);
		settings.dialogue_window.column_disp_index.width = ListView_GetColumnWidth(infoList, 2);
		settings.dialogue_window.column_id.width = ListView_GetColumnWidth(infoList, 3);
		settings.dialogue_window.column_faction.width = ListView_GetColumnWidth(infoList, 4);
		settings.dialogue_window.column_cell.width = ListView_GetColumnWidth(infoList, 5);
		settings.dialogue_window.column_condition1.width = ListView_GetColumnWidth(infoList, 6);
		settings.dialogue_window.column_condition2.width = ListView_GetColumnWidth(infoList, 7);
		settings.dialogue_window.column_condition3.width = ListView_GetColumnWidth(infoList, 8);
		settings.dialogue_window.column_condition4.width = ListView_GetColumnWidth(infoList, 9);
		settings.dialogue_window.column_condition5.width = ListView_GetColumnWidth(infoList, 10);
		settings.dialogue_window.column_condition6.width = ListView_GetColumnWidth(infoList, 11);
	}

	//
	// Patch: Optimize cell string find/insertion.
	//

	std::unordered_set<std::string> insertedCells;

	LRESULT CALLBACK PatchOptimizeCellInsertion(HWND hWnd, UINT msg, WPARAM wParam, BaseObject* object) {
		std::string lowercased = object->getObjectID();
		string::to_lower(lowercased);

		if (insertedCells.find(lowercased) == insertedCells.end()) {
			auto r = SendMessageA(hWnd, CB_ADDSTRING, NULL, (LPARAM)object->getObjectID());
			SendMessageA(hWnd, CB_SETITEMDATA, r, (LPARAM)object);
			insertedCells.insert(std::move(lowercased));
			return -1;
		}
		return 1;
	}

	//
	// Patch: Ensure variable combo boxes always have a dropdown of sufficient width.
	//

	LONG PatchEnsureVariableComboBoxListWidth_LongestField = 0;
	HDC PatchEnsureVariableComboBoxListWidth_hDC = NULL;

	HDC CALLBACK PatchEnsureVariableComboBoxListWidth_GetCachedHDC(HWND hWnd) {
		PatchEnsureVariableComboBoxListWidth_hDC = GetDC(hWnd);
		return PatchEnsureVariableComboBoxListWidth_hDC;
	}

	LRESULT CALLBACK PatchEnsureVariableComboBoxListWidth_SendDlgItemMessageAWrapper(HWND hDlg, int nIDDlgItem, UINT msg, WPARAM wParam, LPARAM lParam) {
		SIZE textExtentSize = {};

		switch (msg) {
		case CB_RESETCONTENT:
			PatchEnsureVariableComboBoxListWidth_LongestField = 0;
			break;
		case CB_ADDSTRING:
			if (GetTextExtentPoint32A(PatchEnsureVariableComboBoxListWidth_hDC, (const char*)lParam, strlen((const char*)lParam), &textExtentSize)) {
				if (textExtentSize.cx > PatchEnsureVariableComboBoxListWidth_LongestField) {
					PatchEnsureVariableComboBoxListWidth_LongestField = textExtentSize.cx;
				}
			}
			break;
		case CB_SETDROPPEDWIDTH:
			wParam = PatchEnsureVariableComboBoxListWidth_LongestField;
			break;
		}

		return SendDlgItemMessageA(hDlg, nIDDlgItem, msg, wParam, lParam);
	}

	static DWORD PatchEnsureVariableComboBoxListWidth_SendDlgItemMessageAWrapper_ExternPointer = (DWORD)PatchEnsureVariableComboBoxListWidth_SendDlgItemMessageAWrapper;

	//
	// Patch: Optimize displaying of INFO info.
	//

	static std::unordered_map<std::string, int> allRelevantLocals;
	static std::unordered_map<std::string, Cell*> allCells;

	void populateCollections() {
		// Clear existing data.
		allRelevantLocals.clear();
		allCells.clear();

		std::unordered_set<std::string> caseInsensitiveIDs;

		auto recordHandler = DataHandler::get()->recordHandler;

		// Gather all local variable names.
		for (auto script : *recordHandler->scripts) {
			for (auto i = 0; i < script->header.numShorts; ++i) {
				auto rawId = script->getShortVarName(i);
				if (rawId && rawId[0] != '\0') {
					std::string lowerId = rawId;
					string::to_lower(lowerId);

					if (caseInsensitiveIDs.find(lowerId) == caseInsensitiveIDs.end()) {
						allRelevantLocals.insert({ rawId, 115 });
						caseInsensitiveIDs.insert(lowerId);
					}
				}
			}
			for (auto i = 0; i < script->header.numLongs; ++i) {
				auto rawId = script->getLongVarName(i);
				if (rawId && rawId[0] != '\0') {
					std::string lowerId = rawId;
					string::to_lower(lowerId);

					if (caseInsensitiveIDs.find(lowerId) == caseInsensitiveIDs.end()) {
						allRelevantLocals.insert({ rawId, 108 });
						caseInsensitiveIDs.insert(lowerId);
					}
				}
			}
			for (auto i = 0; i < script->header.numFloats; ++i) {
				auto rawId = script->getFloatVarName(i);
				if (rawId && rawId[0] != '\0') {
					std::string lowerId = rawId;
					string::to_lower(lowerId);

					if (caseInsensitiveIDs.find(lowerId) == caseInsensitiveIDs.end()) {
						allRelevantLocals.insert({ rawId, 102 });
						caseInsensitiveIDs.insert(lowerId);
					}
				}
			}
		}

		// Gather all cell IDs.
		for (auto i = 0u; i < recordHandler->getCellCount(); ++i) {
			auto cell = recordHandler->getCellByIndex(i);
			if (cell) {
				allCells.insert({ cell->getObjectID(), cell });
			}
		}
	}

	void __fastcall PatchOptimizePopulatingLocalVariableNames(HWND hWnd, int nIDDlgItem) {
		for (const auto& [name, itemData] : allRelevantLocals) {
			auto index = PatchEnsureVariableComboBoxListWidth_SendDlgItemMessageAWrapper(hWnd, nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)name.c_str());
			SendDlgItemMessageA(hWnd, nIDDlgItem, CB_SETITEMDATA, index, itemData);
		}
	}

	__declspec(naked) void PatchOptimizePopulatingLocalVariableNames_Setup() {
		__asm {
			mov ecx, ebp					// Size: 0x2
			mov edx, ebx					// Size: 0x2
			nop								// Size: 0x5
			nop
			nop
			nop
			nop
		}
	}
	constexpr auto PatchOptimizePopulatingLocalVariableNames_Setup_CallOffset = 0x2 + 0x2;
	constexpr auto PatchOptimizePopulatingLocalVariableNames_Setup_Size = 0x5 + PatchOptimizePopulatingLocalVariableNames_Setup_CallOffset;
	
	void __fastcall PatchOptimizePopulatingCellVariableNames(HWND hWnd, int nIDDlgItem) {
		for (const auto& [id, cell] : allCells) {
			auto index = SendDlgItemMessageA(hWnd, nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)cell->getObjectID());
			SendDlgItemMessageA(hWnd, nIDDlgItem, CB_SETITEMDATA, index, (LPARAM)cell);
		}
	}

	constexpr auto PatchOptimizePopulatingCellVariableNames_ReturnAddr = 0x4E8222;
	__declspec(naked) void PatchOptimizePopulatingCellVariableNames_Setup() {
		__asm {
			mov ecx, ebp											// Size: 0x2
			mov edx, ebx											// Size: 0x2
			nop														// Size: 0x5
			nop
			nop
			nop
			nop
			jmp PatchOptimizePopulatingCellVariableNames_ReturnAddr	// Size: 0x6
		}
	}
	constexpr auto PatchOptimizePopulatingCellVariableNames_Setup_Size = 0xFu;

	void __fastcall PatchOptimizeDialogueInfoDisplaying(DialogueInfo* info, DWORD _edx_, HWND hWnd) {
		std::chrono::high_resolution_clock::time_point initializationTimer;
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			initializationTimer = std::chrono::high_resolution_clock::now();
		}

		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION1_TYPE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION1_TYPE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION1_VARIABLE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION1_CONDITION_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION1_VALUE_EDIT, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION2_TYPE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION2_VARIABLE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION2_CONDITION_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION2_VALUE_EDIT, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION3_TYPE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION3_VARIABLE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION3_CONDITION_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION3_VALUE_EDIT, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION4_TYPE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION4_VARIABLE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION4_CONDITION_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION4_VALUE_EDIT, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION5_TYPE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION5_VARIABLE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION5_CONDITION_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION5_VALUE_EDIT, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION6_TYPE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION6_VARIABLE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION6_CONDITION_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CONDITION_FUNCTION6_VALUE_EDIT, WM_SETREDRAW, FALSE, NULL);
		}

		const auto CS_DialogueInfo_displayToEditor = reinterpret_cast<void(__thiscall*)(DialogueInfo*, HWND)>(0x4F1070);
		CS_DialogueInfo_displayToEditor(info, hWnd);

		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION1_TYPE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION1_VARIABLE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION1_CONDITION_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION1_VALUE_EDIT);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION2_TYPE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION2_VARIABLE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION2_CONDITION_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION2_VALUE_EDIT);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION3_TYPE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION3_VARIABLE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION3_CONDITION_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION3_VALUE_EDIT);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION4_TYPE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION4_VARIABLE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION4_CONDITION_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION4_VALUE_EDIT);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION5_TYPE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION5_VARIABLE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION5_CONDITION_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION5_VALUE_EDIT);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION6_TYPE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION6_VARIABLE_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION6_CONDITION_COMBO);
			resumeRenderingAndRepaint(hWnd, CONTROL_ID_CONDITION_FUNCTION6_VALUE_EDIT);
		}

		if constexpr (LOG_PERFORMANCE_RESULTS) {
			auto windowData = (UserData*)GetWindowLongA(hWnd, GWL_USERDATA);
			auto timeToInitialize = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initializationTimer);
			log::stream << "Displaying INFO " << windowData->currentDialogue->id << "/" << info->loadLinkNodes->name << " took " << timeToInitialize.count() << "ms" << std::endl;
		}
	}

	Dialogue* __cdecl PatchOptimizeTopicListRefresh(HWND hWnd, BaseObject* a, BaseObject* filterObject, Dialogue* filterDialogue) {
		SendDlgItemMessageA(hWnd, CONTROL_ID_TOPIC_LIST, WM_SETREDRAW, FALSE, NULL);

		const auto CS_TopicListRefresh = reinterpret_cast<Dialogue*(__cdecl*)(HWND, BaseObject*, BaseObject*, Dialogue*)>(0x4E70E0);
		auto dialogue = CS_TopicListRefresh(hWnd, a, filterObject, filterDialogue);

		SendDlgItemMessageA(hWnd, CONTROL_ID_TOPIC_LIST, WM_SETREDRAW, TRUE, NULL);

		return dialogue;
	}

	//
	// Patch: Allow filtering of topic list.
	//

	bool PatchFilterTopicList(HWND hWnd, const Dialogue* topic) {
		auto userData = reinterpret_cast<UserData*>(GetWindowLongA(hWnd, GWL_USERDATA));
		if (userData->modeShowModifiedOnly && !topic->getModified()) {
			return false;
		}
		return true;
	}

	void __cdecl PatchTopicListAddItem(HWND hDlg, const Dialogue* topic) {
		if (PatchFilterTopicList(hDlg, topic)) {
			const auto CS_AddTopicToList = reinterpret_cast<void(__cdecl*)(HWND, const Dialogue*)>(0x4E7050);
			CS_AddTopicToList(hDlg, topic);
		}
	}

	//
	// Patch: Change behavior of local variable filtering.
	// 
	// By default the game shows all locals. This change makes it so that if filtering by an actor, it will only show
	// locals that are on that NPC.
	//

	void __cdecl PatchFillConditionCombos(HWND hWnd, int controlIdOffset, int conditionType) {
		const auto CS_FillConditionCombos = reinterpret_cast<void(__cdecl*)(HWND, int, int)>(0x4E7C00);

		const auto userData = reinterpret_cast<UserData*>(GetWindowLongA(hWnd, GWL_USERDATA));
		const auto filterScript = (userData && userData->currentFilterObject) ? userData->currentFilterObject->getScript() : nullptr;

		// We only care if we are filtering for local variables, and have a script.
		// Not Local conditions should not be filtered.
		if (filterScript == nullptr || conditionType != DialogueInfo::Condition::TypeLocal) {
			CS_FillConditionCombos(hWnd, controlIdOffset, conditionType);
			return;
		}

		// Here we just want to clear the combo and add all the filtered actor's local variables.
		const auto hConditionCombo = GetDlgItem(hWnd, controlIdOffset + CONTROL_ID_CONDITION_FUNCTION1_VARIABLE_COMBO);
		ComboBox_ResetContent(hConditionCombo);
		for (auto i = 0; i < filterScript->header.numShorts; ++i) {
			const auto index = ComboBox_AddString(hConditionCombo, filterScript->getShortVarName(i));
			ComboBox_SetItemData(hConditionCombo, index, 's');
		}
		for (auto i = 0; i < filterScript->header.numLongs; ++i) {
			const auto index = ComboBox_AddString(hConditionCombo, filterScript->getLongVarName(i));
			ComboBox_SetItemData(hConditionCombo, index, 'l');
		}
		for (auto i = 0; i < filterScript->header.numFloats; ++i) {
			const auto index = ComboBox_AddString(hConditionCombo, filterScript->getFloatVarName(i));
			ComboBox_SetItemData(hConditionCombo, index, 'f');
		}
	}

	//
	// Patch: Change behavior of cell filtering.
	//

	__declspec(naked) void PatchFilterCellBehavior_Setup() {
		__asm {
			mov ecx, esi;	// Size: 0x2
			mov edx, ebp;	// Size: 0x2
			nop;			// Size: 0x5, will be a call.
			nop;
			nop;
			nop;
			nop;
			test al, al;	// Size: 0x2
		}
	}
	constexpr auto PatchFilterCellBehavior_SetupSize = 0xBu;

	bool FilterWithReferenceCell(DialogueInfo* info, Actor* filterActor) {
		const auto reference = DataHandler::get()->recordHandler->getReference(filterActor);
		if (reference == nullptr) {
			return false;
		}

		const auto referenceCell = reference->getCell();
		if (referenceCell == nullptr) {
			return false;
		}

		const std::string_view referenceCellId = referenceCell->getObjectID();
		const std::string_view filterCellId = info->filterCell->getObjectID();
		if (_strnicmp(referenceCellId.data(), filterCellId.data(), filterCellId.size())) {
			return false;
		}

		return true;
	}

	bool FilterWithRenderWindowCell(DialogueInfo* info, Actor* filterActor) {
		const auto currentCell = render_window::gCurrentCell::get();
		if (currentCell == nullptr) {
			return false;
		}

		const std::string_view currentCellId = currentCell->getObjectID();
		const std::string_view filterCellId = info->filterCell->getObjectID();
		if (_strnicmp(currentCellId.data(), filterCellId.data(), filterCellId.size())) {
			return false;
		}

		return true;
	}

	bool __fastcall PatchFilterCellBehavior(DialogueInfo* info, Actor* filterActor) {
		const auto hWnd = getActiveDialogueWindow();
		const auto userData = reinterpret_cast<UserData*>(GetWindowLongA(hWnd, GWL_USERDATA));
		const auto filterMode = userData ? userData->cellFilterMode : UserData::CellFilterMode::UseCellReference;

		switch (filterMode) {
		case UserData::CellFilterMode::UseCellReference:
			return FilterWithReferenceCell(info, filterActor);
		case UserData::CellFilterMode::UseRenderWindowCell:
			return FilterWithRenderWindowCell(info, filterActor);
		case UserData::CellFilterMode::IgnoreCellFilter:
			return true;
		}

		return true;
	}

	//
	// Patch: Extend structure of the dialogue window user data.
	//

	LONG __stdcall SetExtendedUserData(HWND hWnd, int nIndex, UserData* userData) {
		userData->cellFilterMode = UserData::CellFilterMode::UseCellReference;
		userData->modeShowModifiedOnly = false;
		return SetWindowLongA(hWnd, nIndex, (LONG)userData);
	}

	//
	// Patch: Extend Render Window message handling.
	//

	auto initializationTimer = std::chrono::high_resolution_clock::now();

	void PatchDialogProc_BeforeInitialize(DialogProcContext& context) {
		using winui::GetStyle;
		using winui::SetStyle;
		using winui::ResizeAndCenterWindow;

		// Begin measure of initialization time.
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			initializationTimer = std::chrono::high_resolution_clock::now();
		}

		// Disable redraw.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			const auto hWnd = context.getWindowHandle();
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_FILTER_FOR_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_TOPIC_LIST), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_INFO_LIST), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_ID_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_RACE_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_CLASS_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_FACTION_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_RANK_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_CELL_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_PC_FACTION_COMBO), WM_SETREDRAW, FALSE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_PC_RANK_COMBO), WM_SETREDRAW, FALSE, NULL);

			// Gather what data we need to populate lists.
			populateCollections();
		}
	}

	void PatchDialogProc_BeforeDestroy(DialogProcContext& context) {
		// Save column info.
		saveInfoColumnWidths(context.getWindowHandle());
	}

	constexpr auto MIN_WIDTH = 1113u;
	constexpr auto MIN_HEIGHT = 700u;

	void PatchDialogProc_GetMinMaxInfo(DialogProcContext& context) {
		const auto info = context.getMinMaxInfo();
		info->ptMinTrackSize.x = MIN_WIDTH;
		info->ptMinTrackSize.y = MIN_HEIGHT;

		context.setResult(0);
	}

	void PatchDialogProc_AfterInitialize(DialogProcContext& context) {
		using se::cs::winui::AddStyles;
		using se::cs::winui::GetRectHeight;
		using se::cs::winui::GetRectWidth;
		using se::cs::winui::RemoveStyles;
		using se::cs::winui::ResizeAndCenterWindow;
		using se::cs::winui::SetWindowIdByValue;

		// Restore column widths.
		const auto hWnd = context.getWindowHandle();
		restoreInfoColumnWidths(hWnd);

		// Reenable redraw.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_FILTER_FOR_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_TOPIC_LIST), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_INFO_LIST), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_ID_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_RACE_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_CLASS_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_FACTION_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_RANK_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_CELL_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_PC_FACTION_COMBO), WM_SETREDRAW, TRUE, NULL);
			SendMessageA(GetDlgItem(hWnd, CONTROL_ID_CONDITION_PC_RANK_COMBO), WM_SETREDRAW, TRUE, NULL);

			insertedCells.clear();
		}

		// Gather dialogs that we want to edit.
		auto hDlgCurrentEdit = GetDlgItem(hWnd, CONTROL_ID_CURRENT_TEXT_EDIT);
		auto hDlgResultEdit = GetDlgItem(hWnd, CONTROL_ID_CURRENT_RESULT_EDIT);
		auto hDlgTopicList = GetDlgItem(hWnd, CONTROL_ID_TOPIC_LIST);

		// Enable advanced controls on elements.
		SetWindowSubclass(hDlgCurrentEdit, ui_subclass::edit::BasicExtendedProc, NULL, NULL);
		SetWindowSubclass(hDlgResultEdit, ui_subclass::edit::BasicExtendedProc, NULL, NULL);

		// Give IDs to controls that don't normally have one.
		SetWindowIdByValue(hWnd, "Filter for", CONTROL_ID_FILTER_FOR_STATIC);
		SetWindowIdByValue(hWnd, "Speaker Condition", CONTROL_ID_SPEAKER_CONDITION_BUTTON);
		SetWindowIdByValue(hWnd, "ID", CONTROL_ID_CONDITION_ID_STATIC);
		SetWindowIdByValue(hWnd, "Race", CONTROL_ID_CONDITION_RACE_STATIC);
		SetWindowIdByValue(hWnd, "Cell", CONTROL_ID_CONDITION_CELL_STATIC);
		SetWindowIdByValue(hWnd, "Faction", CONTROL_ID_CONDITION_FACTION_STATIC);
		SetWindowIdByValue(hWnd, "Rank", CONTROL_ID_CONDITION_RANK_STATIC);
		SetWindowIdByValue(hWnd, "Class", CONTROL_ID_CONDITION_CLASS_STATIC);
		SetWindowIdByValue(hWnd, "PC Faction", CONTROL_ID_CONDITION_PC_FACTION_STATIC);
		SetWindowIdByValue(hWnd, "PC Rank", CONTROL_ID_CONDITION_PC_RANK_STATIC);
		SetWindowIdByValue(hWnd, "Function/Variable", CONTROL_ID_CONDITION_FUNCTION_VARIABLE_STATIC);
		SetWindowIdByValue(hWnd, "Result", CONTROL_ID_CURRENT_RESULT_STATIC);
		SetWindowIdByValue(hWnd, "Shared By", CONTROL_ID_SHARED_BY_STATIC);

		// Unhide quest stuff to mark them as disabled.
		auto hDlgJournalQuestNameCheckButton = GetDlgItem(hWnd, CONTROL_ID_CONDITION_JOURNAL_QUEST_NAME_CHECKBOX);
		ShowWindow(hDlgJournalQuestNameCheckButton, TRUE);
		EnableWindow(hDlgJournalQuestNameCheckButton, FALSE);
		auto hDlgJournalQuestFinishedCheckButton = GetDlgItem(hWnd, CONTROL_ID_CONDITION_JOURNAL_FINISHED_CHECKBOX);
		ShowWindow(hDlgJournalQuestFinishedCheckButton, TRUE);
		EnableWindow(hDlgJournalQuestFinishedCheckButton, FALSE);
		auto hDlgJournalQuestRestartCheckButton = GetDlgItem(hWnd, CONTROL_ID_CONDITION_JOURNAL_RESTART_CHECKBOX);
		ShowWindow(hDlgJournalQuestRestartCheckButton, TRUE);
		EnableWindow(hDlgJournalQuestRestartCheckButton, FALSE);

		// Add custom controls.
		auto hInstance = (HINSTANCE)GetWindowLongA(hWnd, GWLP_HINSTANCE);
		auto font = SendMessageA(hWnd, WM_GETFONT, FALSE, FALSE);

		//constexpr auto hDlgFilterCellStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_HASSTRINGS;
		constexpr auto hDlgFilterCellStyles = CBS_DROPDOWNLIST | CBS_SORT | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP;
		//constexpr auto hDlgFilterCellExtendedStlyes = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY;
		constexpr auto hDlgFilterCellExtendedStlyes = NULL;
		const auto hDlgFilterCell = CreateWindowExA(hDlgFilterCellExtendedStlyes, WC_COMBOBOXA, NULL, hDlgFilterCellStyles, 0, 0, 300, 100, hWnd, (HMENU)CONTROL_ID_FILTER_CELL_SETTING_COMBO, hInstance, NULL);
		SendMessageA(hDlgFilterCell, EM_SETEXTENDEDSTYLE, hDlgFilterCellExtendedStlyes, hDlgFilterCellExtendedStlyes);
		SendMessageA(hDlgFilterCell, WM_SETFONT, font, MAKELPARAM(TRUE, FALSE));
		ComboBox_AddString(hDlgFilterCell, "Filter with first reference's cell");
		ComboBox_AddString(hDlgFilterCell, "Filter with render window cell");
		ComboBox_AddString(hDlgFilterCell, "Ignore cell filter");
		ComboBox_SetCurSel(hDlgFilterCell, 0);
		ComboBox_SetMinVisible(hDlgFilterCell, 3);

		auto hDlgShowModifiedOnly = CreateWindowExA(NULL, WC_BUTTON, "Show modified only", BS_AUTOCHECKBOX | BS_PUSHLIKE | WS_CHILD | WS_VISIBLE | WS_GROUP, 0, 0, 0, 0, hWnd, (HMENU)CONTROL_ID_SHOW_MODIFIED_ONLY_BUTTON, hInstance, NULL);
		SendMessageA(hDlgShowModifiedOnly, WM_SETFONT, font, MAKELPARAM(TRUE, FALSE));

		auto hDlgCurrentTextCharCount = CreateWindowExA(NULL, WC_STATIC, "0", SS_RIGHT | WS_CHILD | WS_VISIBLE | WS_GROUP, 0, 0, 0, 0, hWnd, (HMENU)CONTROL_ID_CURRENT_TEXT_CHAR_COUNT, hInstance, NULL);
		SendMessageA(hDlgCurrentTextCharCount, WM_SETFONT, font, MAKELPARAM(TRUE, FALSE));
		OnCurrentTextEditChanged(hWnd);

		auto hDlgCurrentTextMaxCharCount = CreateWindowExA(NULL, WC_STATIC, "/512", SS_RIGHT | WS_CHILD | WS_VISIBLE | WS_GROUP, 0, 0, 0, 0, hWnd, (HMENU)CONTROL_ID_CURRENT_TEXT_MAX_CHAR_COUNT, hInstance, NULL);
		SendMessageA(hDlgCurrentTextMaxCharCount, WM_SETFONT, font, MAKELPARAM(TRUE, FALSE));

		// Make it so the window can be maximized and generally resized.
		RemoveStyles(hWnd, DS_MODALFRAME);
		AddStyles(hWnd, WS_SIZEBOX | WS_MAXIMIZEBOX);

		// Restore size, with enforced minimum.
		const auto& settingsSize = settings.dialogue_window.size;
		const auto width = std::max(settingsSize.width, MIN_WIDTH);
		const auto height = std::max(settingsSize.height, MIN_HEIGHT);
		ResizeAndCenterWindow(hWnd, width, height);

		// Finish measure of initialization time.
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			auto timeToInitialize = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initializationTimer);
			log::stream << "Total dialogue window initialization time: " << timeToInitialize.count() << "ms" << std::endl;
		}
	}

	void PatchDialogProc_AfterNotify_CustomDraw(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto lplvcd = context.getNotificationCustomDraw();

		const auto idFrom = lplvcd->nmcd.hdr.idFrom;
		if (settings.dialogue_window.highlight_modified_items && idFrom == CONTROL_ID_TOPIC_LIST || idFrom == CONTROL_ID_INFO_LIST) {
			if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT) {
				SetWindowLongA(hWnd, DWLP_MSGRESULT, CDRF_NOTIFYITEMDRAW);
			}
			else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) {
				auto object = (BaseObject*)lplvcd->nmcd.lItemlParam;
				if (object) {
					// Background color highlighting.
					if (object->getDeleted()) {
						lplvcd->clrTextBk = settings.color_theme.highlight_deleted_object_packed_color;
						SetWindowLongA(hWnd, DWLP_MSGRESULT, CDRF_NEWFONT);
					}
					else if (object->getModified()) {
						// Modified color highlighting. Different colors for modified-master or mod-added object.
						lplvcd->clrTextBk = object->isFromMaster() ? settings.color_theme.highlight_modified_from_master_packed_color : settings.color_theme.highlight_modified_new_object_packed_color;
						SetWindowLongA(hWnd, DWLP_MSGRESULT, CDRF_NEWFONT);
					}
				}
			}
			context.setResult(TRUE);
		}
	}

	void PatchDialogProc_AfterNotify_TabControl_SelectionChanged(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		auto userData = (UserData*)GetWindowLongA(hWnd, GWL_USERDATA);

		auto hDlgConditionSexStatic = GetDlgItem(hWnd, CONTROL_ID_CONDITION_SEX_STATIC);
		auto hDlgConditionSexCombo = GetDlgItem(hWnd, CONTROL_ID_CONDITION_SEX_COMBO);
		auto hDlgJournalQuestNameCheckButton = GetDlgItem(hWnd, CONTROL_ID_CONDITION_JOURNAL_QUEST_NAME_CHECKBOX);
		auto hDlgJournalQuestFinishedCheckButton = GetDlgItem(hWnd, CONTROL_ID_CONDITION_JOURNAL_FINISHED_CHECKBOX);
		auto hDlgJournalQuestRestartCheckButton = GetDlgItem(hWnd, CONTROL_ID_CONDITION_JOURNAL_RESTART_CHECKBOX);

		// Restore visibility of the sex condition.
		if (userData->currentTypeTab == 4) {
			ShowWindow(hDlgConditionSexStatic, TRUE);

			ShowWindow(hDlgConditionSexCombo, TRUE);
			EnableWindow(hDlgConditionSexCombo, FALSE);
		}
		else {
			ShowWindow(hDlgJournalQuestNameCheckButton, TRUE);
			EnableWindow(hDlgJournalQuestNameCheckButton, FALSE);
			ShowWindow(hDlgJournalQuestFinishedCheckButton, TRUE);
			EnableWindow(hDlgJournalQuestFinishedCheckButton, FALSE);
			ShowWindow(hDlgJournalQuestRestartCheckButton, TRUE);
			EnableWindow(hDlgJournalQuestRestartCheckButton, FALSE);
		}
	}

	void PatchDialogProc_AfterNotify(DialogProcContext& context) {
		const auto param = context.getNotificationData();

		switch (param->code) {
		case NM_CUSTOMDRAW:
			PatchDialogProc_AfterNotify_CustomDraw(context);
			break;
		case TCN_SELCHANGE:
			PatchDialogProc_AfterNotify_TabControl_SelectionChanged(context);
			break;
		}
	}

	namespace ResizeConstants {
		constexpr auto BASIC_PADDING = 2;
		constexpr auto BIG_PADDING = 6;
		constexpr auto WINDOW_EDGE_PADDING = 10;
		constexpr auto LEFT_SECTION_WIDTH = 250;
		constexpr auto BOTTOM_RIGHT_SECTION_WIDTH = 150;
		constexpr auto BOTTOM_SECTION_HEIGHT = 500;
		constexpr auto BIG_BUTTON_HEIGHT = 26;
		constexpr auto STATIC_HEIGHT = 13;
		constexpr auto JOURNAL_CHECKBUTTON_WIDTH = 100;
		constexpr auto JOURNAL_CHECKBUTTON_HEIGHT = STATIC_HEIGHT;
		constexpr auto COMBO_HEIGHT = 21;
		constexpr auto EDIT_HEIGHT = 17;
		constexpr auto STATIC_COMBO_OFFSET = (COMBO_HEIGHT - STATIC_HEIGHT) / 2;

		constexpr auto SPEAKER_CONDITION_PADDING_TOP = 17;
		constexpr auto SPEAKER_CONDITION_PADDING_BOTTOM = 4;
		constexpr auto SPEAKER_CONDITION_HEIGHT = COMBO_HEIGHT * 9 + BASIC_PADDING * 8 + SPEAKER_CONDITION_PADDING_TOP + SPEAKER_CONDITION_PADDING_BOTTOM;
		constexpr auto TOP_INFO_TEXT_HEIGHT = (BOTTOM_SECTION_HEIGHT - SPEAKER_CONDITION_HEIGHT) / 2 - BASIC_PADDING;
		constexpr auto BOTTOM_RESULT_HEIGHT = BOTTOM_SECTION_HEIGHT - TOP_INFO_TEXT_HEIGHT - SPEAKER_CONDITION_HEIGHT - BASIC_PADDING * 2;

		constexpr auto CONDITION_STATIC_WIDTH = 55;
		constexpr auto CONDITION_COMBO_WIDTH = 200;

		constexpr auto FUNCTION_TYPE_WIDTH = 100;
		constexpr auto FUNCTION_CONDITION_WIDTH = 200;
		constexpr auto FUNCTION_COMPARISON_WIDTH = 40;
		constexpr auto FUNCTION_VALUE_WIDTH = 50;
		constexpr auto FUNCTION_TOTAL_WIDTH = FUNCTION_TYPE_WIDTH + FUNCTION_CONDITION_WIDTH + FUNCTION_COMPARISON_WIDTH + FUNCTION_VALUE_WIDTH + BASIC_PADDING * 3;

		constexpr auto DISPOSITION_INDEX_WIDTH = FUNCTION_VALUE_WIDTH;

		constexpr auto BOTTOM_SECTION_MIN_WIDTH = CONDITION_STATIC_WIDTH + CONDITION_COMBO_WIDTH + BIG_PADDING + FUNCTION_TOTAL_WIDTH + BASIC_PADDING * 4;
		constexpr auto TOTAL_MIN_WIDTH = LEFT_SECTION_WIDTH + BOTTOM_SECTION_MIN_WIDTH + BOTTOM_RIGHT_SECTION_WIDTH + BIG_PADDING * 2 + WINDOW_EDGE_PADDING * 2;
		constexpr auto TOTAL_MIN_HEIGHT = 720;
	}

	void ResizeLabeledStatic(HWND hParent, int iDlgStatic, int iDlgCombo, int x, int& y) {
		using namespace ResizeConstants;

		auto hDlgStatic = GetDlgItem(hParent, iDlgStatic);
		MoveWindow(hDlgStatic, x, y + STATIC_COMBO_OFFSET, CONDITION_STATIC_WIDTH, STATIC_HEIGHT, FALSE);

		auto hDlgCombo = GetDlgItem(hParent, iDlgCombo);
		MoveWindow(hDlgCombo, x + CONDITION_STATIC_WIDTH + BASIC_PADDING, y, CONDITION_COMBO_WIDTH, COMBO_HEIGHT, FALSE);

		y += COMBO_HEIGHT + BASIC_PADDING;
	}

	void ResizeFunctionCondition(HWND hParent, int iDlgType, int iDlgCondition, int iDlgComparison, int iDlgValue, int x, int& y) {
		using namespace ResizeConstants;

		auto hDlgType = GetDlgItem(hParent, iDlgType);
		MoveWindow(hDlgType, x, y, FUNCTION_TYPE_WIDTH, COMBO_HEIGHT, FALSE);
		x += FUNCTION_TYPE_WIDTH + BASIC_PADDING;

		auto hDlgCondition = GetDlgItem(hParent, iDlgCondition);
		MoveWindow(hDlgCondition, x, y, FUNCTION_CONDITION_WIDTH, COMBO_HEIGHT, FALSE);
		x += FUNCTION_CONDITION_WIDTH + BASIC_PADDING;

		auto hDlgComparison = GetDlgItem(hParent, iDlgComparison);
		MoveWindow(hDlgComparison, x, y, FUNCTION_COMPARISON_WIDTH, COMBO_HEIGHT, FALSE);
		x += FUNCTION_COMPARISON_WIDTH + BASIC_PADDING;

		auto hDlgValue = GetDlgItem(hParent, iDlgValue);
		MoveWindow(hDlgValue, x, y, FUNCTION_VALUE_WIDTH, COMBO_HEIGHT, FALSE);

		y += COMBO_HEIGHT + BASIC_PADDING;
	}

#define ResizeFunctionConditionHelper(hParent, i, x, y) ResizeFunctionCondition(hParent, CONTROL_ID_CONDITION_FUNCTION##i##_TYPE_COMBO, CONTROL_ID_CONDITION_FUNCTION##i##_VARIABLE_COMBO, CONTROL_ID_CONDITION_FUNCTION##i##_CONDITION_COMBO, CONTROL_ID_CONDITION_FUNCTION##i##_VALUE_EDIT, x, y);

	void PatchDialogProc_BeforeSize(DialogProcContext& context) {
		using namespace ResizeConstants;
		using winui::GetRectHeight;
		using winui::GetRectWidth;
		using winui::TabCtrl_GetInteriorRect;

		const auto hWnd = context.getWindowHandle();
		const auto lParam = context.getLParam();
		const auto clientWidth = LOWORD(lParam);
		const auto clientHeight = HIWORD(lParam);

		RECT tempRect = {};

		// Left section.
		{
			const auto currentX = WINDOW_EDGE_PADDING;
			auto currentY = WINDOW_EDGE_PADDING;

			constexpr auto FILTER_FOR_AREA_SIZE = STATIC_HEIGHT + COMBO_HEIGHT * 3 + BASIC_PADDING * 3;

			// Dialogue type tabs
			const auto topicsAreaSize = clientHeight - FILTER_FOR_AREA_SIZE - BASIC_PADDING * 2 - WINDOW_EDGE_PADDING * 2;
			auto hDlgTopicTabs = GetDlgItem(hWnd, CONTROL_ID_TOPIC_TABS);
			MoveWindow(hDlgTopicTabs, currentX, currentY, LEFT_SECTION_WIDTH, topicsAreaSize, FALSE);

			// Dialogue topics list
			auto hDlgTopicList = GetDlgItem(hWnd, CONTROL_ID_TOPIC_LIST);
			TabCtrl_GetInteriorRect(hDlgTopicTabs, &tempRect);
			MoveWindow(hDlgTopicList, tempRect.left, tempRect.top, GetRectWidth(&tempRect), GetRectHeight(&tempRect), FALSE);
			ListView_SetColumnWidth(hDlgTopicList, 0, GetRectWidth(&tempRect) - GetSystemMetrics(SM_CXVSCROLL) - BASIC_PADDING * 2);
			currentY += topicsAreaSize + BASIC_PADDING * 2;

			// Filter For static
			auto hDlgFilterForStatic = GetDlgItem(hWnd, CONTROL_ID_FILTER_FOR_STATIC);
			MoveWindow(hDlgFilterForStatic, currentX, currentY, LEFT_SECTION_WIDTH, STATIC_HEIGHT, FALSE);
			currentY += STATIC_HEIGHT + BASIC_PADDING;

			// Filter For combo
			auto hDlgFilterForCombo = GetDlgItem(hWnd, CONTROL_ID_FILTER_FOR_COMBO);
			MoveWindow(hDlgFilterForCombo, currentX, currentY, LEFT_SECTION_WIDTH, COMBO_HEIGHT, FALSE);
			currentY += COMBO_HEIGHT + BASIC_PADDING;

			// Cell filter setting combo
			auto hFilterCellSetting = GetDlgItem(hWnd, CONTROL_ID_FILTER_CELL_SETTING_COMBO);
			MoveWindow(hFilterCellSetting, currentX, currentY, LEFT_SECTION_WIDTH, COMBO_HEIGHT, FALSE);
			currentY += COMBO_HEIGHT + BASIC_PADDING;

			// Show modified only button
			auto hShowModifiedButton = GetDlgItem(hWnd, CONTROL_ID_SHOW_MODIFIED_ONLY_BUTTON);
			MoveWindow(hShowModifiedButton, currentX, currentY, LEFT_SECTION_WIDTH, COMBO_HEIGHT, FALSE);
		}

		// INFO list section
		{
			int currentX = WINDOW_EDGE_PADDING + LEFT_SECTION_WIDTH + BIG_PADDING;
			int currentY = WINDOW_EDGE_PADDING;

			const auto infoListWidth = clientWidth - LEFT_SECTION_WIDTH - BIG_PADDING - WINDOW_EDGE_PADDING * 2;

			auto hDlgInfoStatic = GetDlgItem(hWnd, CONTROL_ID_INFO_STATIC);
			MoveWindow(hDlgInfoStatic, currentX, currentY, infoListWidth, STATIC_HEIGHT, FALSE);
			currentY += STATIC_HEIGHT + BASIC_PADDING;

			auto hDlgInfoList = GetDlgItem(hWnd, CONTROL_ID_INFO_LIST);
			MoveWindow(hDlgInfoList, currentX, currentY, infoListWidth, clientHeight - currentY - BOTTOM_SECTION_HEIGHT - WINDOW_EDGE_PADDING - BASIC_PADDING, FALSE);
		}

		// INFO details section
		{
			// Calculate fixed sizes.
			const auto BOTTOM_MIDDLE_WIDTH = clientWidth - LEFT_SECTION_WIDTH - BOTTOM_RIGHT_SECTION_WIDTH - BIG_PADDING * 2 - WINDOW_EDGE_PADDING * 2;

			// Temp values.
			auto currentX = WINDOW_EDGE_PADDING + LEFT_SECTION_WIDTH + BIG_PADDING;
			auto currentY = clientHeight - BOTTOM_SECTION_HEIGHT - WINDOW_EDGE_PADDING;

			// Info text edit
			auto hDlgCurrentTextEdit = GetDlgItem(hWnd, CONTROL_ID_CURRENT_TEXT_EDIT);
			MoveWindow(hDlgCurrentTextEdit, currentX, currentY, BOTTOM_MIDDLE_WIDTH, TOP_INFO_TEXT_HEIGHT, FALSE);
			currentY += TOP_INFO_TEXT_HEIGHT + BASIC_PADDING;

			auto hDlgCurrentTextCharCount = GetDlgItem(hWnd, CONTROL_ID_CURRENT_TEXT_CHAR_COUNT);
			MoveWindow(hDlgCurrentTextCharCount, currentX + BOTTOM_MIDDLE_WIDTH - 48, currentY + 10, 18, 20, FALSE);

			auto hDlgCurrentTextMaxCharCount = GetDlgItem(hWnd, CONTROL_ID_CURRENT_TEXT_MAX_CHAR_COUNT);
			MoveWindow(hDlgCurrentTextMaxCharCount, currentX + BOTTOM_MIDDLE_WIDTH - 30, currentY + 10, 23, 20, FALSE);

			// Speaker Condition button (area)
			auto hDlgSpeakerConditionButton = GetDlgItem(hWnd, CONTROL_ID_SPEAKER_CONDITION_BUTTON);
			MoveWindow(hDlgSpeakerConditionButton, currentX, currentY, BOTTOM_MIDDLE_WIDTH, SPEAKER_CONDITION_HEIGHT, FALSE);

			// Individual conditions.
			const auto leftOfConditions = currentX;
			const auto topOfConditions = currentY;
			{
				currentX += BASIC_PADDING;
				currentY += SPEAKER_CONDITION_PADDING_TOP;

				// Left drop downs
				ResizeLabeledStatic(hWnd, CONTROL_ID_CONDITION_ID_STATIC, CONTROL_ID_CONDITION_ID_COMBO, currentX, currentY);
				ResizeLabeledStatic(hWnd, CONTROL_ID_CONDITION_RACE_STATIC, CONTROL_ID_CONDITION_RACE_COMBO, currentX, currentY);
				ResizeLabeledStatic(hWnd, CONTROL_ID_CONDITION_SEX_STATIC, CONTROL_ID_CONDITION_SEX_COMBO, currentX, currentY);
				ResizeLabeledStatic(hWnd, CONTROL_ID_CONDITION_CLASS_STATIC, CONTROL_ID_CONDITION_CLASS_COMBO, currentX, currentY);
				ResizeLabeledStatic(hWnd, CONTROL_ID_CONDITION_FACTION_STATIC, CONTROL_ID_CONDITION_FACTION_COMBO, currentX, currentY);
				ResizeLabeledStatic(hWnd, CONTROL_ID_CONDITION_RANK_STATIC, CONTROL_ID_CONDITION_RANK_COMBO, currentX, currentY);
				ResizeLabeledStatic(hWnd, CONTROL_ID_CONDITION_CELL_STATIC, CONTROL_ID_CONDITION_CELL_COMBO, currentX, currentY);
				ResizeLabeledStatic(hWnd, CONTROL_ID_CONDITION_PC_FACTION_STATIC, CONTROL_ID_CONDITION_PC_FACTION_COMBO, currentX, currentY);
				ResizeLabeledStatic(hWnd, CONTROL_ID_CONDITION_PC_RANK_STATIC, CONTROL_ID_CONDITION_PC_RANK_COMBO, currentX, currentY);

				// Disposition
				currentY = topOfConditions + SPEAKER_CONDITION_HEIGHT - SPEAKER_CONDITION_PADDING_BOTTOM - COMBO_HEIGHT * 6 - STATIC_HEIGHT - BASIC_PADDING * 6;

				// Right conditions
				currentX += CONDITION_STATIC_WIDTH + BASIC_PADDING + CONDITION_COMBO_WIDTH + BIG_PADDING;
				currentY = topOfConditions + SPEAKER_CONDITION_HEIGHT - SPEAKER_CONDITION_PADDING_BOTTOM - COMBO_HEIGHT * 6 - STATIC_HEIGHT - BASIC_PADDING * 6;

				// Function/Variable static
				auto hDlgFunctionVariableStatic = GetDlgItem(hWnd, CONTROL_ID_CONDITION_FUNCTION_VARIABLE_STATIC);
				MoveWindow(hDlgFunctionVariableStatic, currentX, currentY, 100, STATIC_HEIGHT, FALSE);
				currentY += STATIC_HEIGHT + BASIC_PADDING;

				// Functions
				const auto leftOfFunctions = currentX;
				const auto topOfFunctions = currentY;
				ResizeFunctionConditionHelper(hWnd, 1, currentX, currentY);
				ResizeFunctionConditionHelper(hWnd, 2, currentX, currentY);
				ResizeFunctionConditionHelper(hWnd, 3, currentX, currentY);
				ResizeFunctionConditionHelper(hWnd, 4, currentX, currentY);
				ResizeFunctionConditionHelper(hWnd, 5, currentX, currentY);
				ResizeFunctionConditionHelper(hWnd, 6, currentX, currentY);

				// Disposition static
				constexpr auto DISPOSITION_JOURNAL_STATIC_WIDTH = 26;
				currentX = leftOfFunctions + FUNCTION_TYPE_WIDTH + FUNCTION_CONDITION_WIDTH + FUNCTION_COMPARISON_WIDTH + BASIC_PADDING * 2 - DISPOSITION_JOURNAL_STATIC_WIDTH;
				currentY = topOfFunctions - COMBO_HEIGHT - BASIC_PADDING;
				auto hDlgDispositionJournalStatic = GetDlgItem(hWnd, CONTROL_ID_CONDITION_DISPOSITION_OR_JOURNAL_STATIC);
				MoveWindow(hDlgDispositionJournalStatic, currentX, currentY + STATIC_COMBO_OFFSET, DISPOSITION_JOURNAL_STATIC_WIDTH, STATIC_HEIGHT, FALSE);
				currentX += DISPOSITION_JOURNAL_STATIC_WIDTH + BASIC_PADDING;

				// Disposition edit
				auto hDlgDispositionJournalEdit = GetDlgItem(hWnd, CONTROL_ID_CONDITION_DISPOSITION_OR_JOURNAL_EDIT);
				MoveWindow(hDlgDispositionJournalEdit, currentX, currentY, DISPOSITION_INDEX_WIDTH, COMBO_HEIGHT, FALSE);

				// Quest Name check button
				currentX = leftOfConditions + BIG_PADDING + CONDITION_STATIC_WIDTH + BASIC_PADDING + CONDITION_COMBO_WIDTH + BIG_PADDING;
				currentY = topOfConditions + SPEAKER_CONDITION_PADDING_TOP;
				auto hDlgJournalQuestNameCheckButton = GetDlgItem(hWnd, CONTROL_ID_CONDITION_JOURNAL_QUEST_NAME_CHECKBOX);
				MoveWindow(hDlgJournalQuestNameCheckButton, currentX, currentY, JOURNAL_CHECKBUTTON_WIDTH, JOURNAL_CHECKBUTTON_HEIGHT, FALSE);
				currentY += JOURNAL_CHECKBUTTON_HEIGHT + BASIC_PADDING;

				// Quest Finished check button
				auto hDlgJournalQuestFinishedCheckButton = GetDlgItem(hWnd, CONTROL_ID_CONDITION_JOURNAL_FINISHED_CHECKBOX);
				MoveWindow(hDlgJournalQuestFinishedCheckButton, currentX, currentY, JOURNAL_CHECKBUTTON_WIDTH, JOURNAL_CHECKBUTTON_HEIGHT, FALSE);
				currentY += JOURNAL_CHECKBUTTON_HEIGHT + BASIC_PADDING;

				// Quest Restart check button
				auto hDlgJournalQuestRestartCheckButton = GetDlgItem(hWnd, CONTROL_ID_CONDITION_JOURNAL_RESTART_CHECKBOX);
				MoveWindow(hDlgJournalQuestRestartCheckButton, currentX, currentY, JOURNAL_CHECKBUTTON_WIDTH, JOURNAL_CHECKBUTTON_HEIGHT, FALSE);
			}
			currentX = leftOfConditions;
			currentY = topOfConditions + SPEAKER_CONDITION_HEIGHT + BASIC_PADDING;

			// Result static
			auto hDlgCurrentResultStatic = GetDlgItem(hWnd, CONTROL_ID_CURRENT_RESULT_STATIC);
			MoveWindow(hDlgCurrentResultStatic, currentX, currentY, BOTTOM_MIDDLE_WIDTH, STATIC_HEIGHT, FALSE);
			currentY += STATIC_HEIGHT + BASIC_PADDING;

			// Result edit
			auto hDlgCurrentResultEdit = GetDlgItem(hWnd, CONTROL_ID_CURRENT_RESULT_EDIT);
			MoveWindow(hDlgCurrentResultEdit, currentX, currentY, BOTTOM_MIDDLE_WIDTH, BOTTOM_RESULT_HEIGHT - STATIC_HEIGHT - BASIC_PADDING, FALSE);
		}

		// Bottom right section.
		{
			constexpr auto EXTRA_PADDING_ABOVE_OK_BUTTON = 16;
			const auto currentX = clientWidth - BOTTOM_RIGHT_SECTION_WIDTH - WINDOW_EDGE_PADDING;
			auto currentY = clientHeight - BOTTOM_SECTION_HEIGHT - WINDOW_EDGE_PADDING;

			// Shared By static
			auto hDlgSharedByStatic = GetDlgItem(hWnd, CONTROL_ID_SHARED_BY_STATIC);
			MoveWindow(hDlgSharedByStatic, currentX, currentY, BOTTOM_RIGHT_SECTION_WIDTH - 2, STATIC_HEIGHT, FALSE);
			currentY += STATIC_HEIGHT + BASIC_PADDING;

			// Shared By list
			constexpr auto SHARED_BY_LIST_HEIGHT = BOTTOM_SECTION_HEIGHT - BIG_BUTTON_HEIGHT * 6 - STATIC_HEIGHT - BASIC_PADDING * 7 - EXTRA_PADDING_ABOVE_OK_BUTTON;
			auto hDlgSharedByList = GetDlgItem(hWnd, CONTROL_ID_SHARED_BY_LIST);
			MoveWindow(hDlgSharedByList, currentX, currentY, BOTTOM_RIGHT_SECTION_WIDTH, SHARED_BY_LIST_HEIGHT, FALSE);
			currentY += SHARED_BY_LIST_HEIGHT + BASIC_PADDING;

			// Update Shared By button
			auto hDlgUpdateSharedByButton = GetDlgItem(hWnd, CONTROL_ID_UPDATE_SHARED_BY_BUTTON);
			MoveWindow(hDlgUpdateSharedByButton, currentX, currentY, BOTTOM_RIGHT_SECTION_WIDTH, BIG_BUTTON_HEIGHT, FALSE);
			currentY += BIG_BUTTON_HEIGHT + BASIC_PADDING;

			// Journal Preview button
			auto hDlgJournalPreviewButton = GetDlgItem(hWnd, CONTROL_ID_JOURNAL_PREVIEW_BUTTON);
			MoveWindow(hDlgJournalPreviewButton, currentX, currentY, BOTTOM_RIGHT_SECTION_WIDTH, BIG_BUTTON_HEIGHT, FALSE);
			currentY += BIG_BUTTON_HEIGHT + BASIC_PADDING;

			// Update Hyperlink button
			constexpr auto UPDATE_ALL_HYPERLINKS_BUTTON_WIDTH = 30;
			constexpr auto UPDATE_HYPERLINK_BUTTON_WIDTH = BOTTOM_RIGHT_SECTION_WIDTH - UPDATE_ALL_HYPERLINKS_BUTTON_WIDTH - 2;
			auto hDlgUpdateHyperlinkButton = GetDlgItem(hWnd, CONTROL_ID_UPDATE_HYPERLINKS_BUTTON);
			MoveWindow(hDlgUpdateHyperlinkButton, currentX, currentY, UPDATE_HYPERLINK_BUTTON_WIDTH, BIG_BUTTON_HEIGHT, FALSE);

			// Update All Hyperlinks button
			auto updateAllHyperlinksX = currentX + UPDATE_HYPERLINK_BUTTON_WIDTH + BASIC_PADDING;
			auto hDlgUpdateAllHyperlinksButton = GetDlgItem(hWnd, CONTROL_ID_UPDATE_ALL_HYPERLINKS_BUTTON);
			MoveWindow(hDlgUpdateAllHyperlinksButton, updateAllHyperlinksX, currentY, UPDATE_ALL_HYPERLINKS_BUTTON_WIDTH, BIG_BUTTON_HEIGHT, FALSE);
			currentY += BIG_BUTTON_HEIGHT + BASIC_PADDING;

			// Error Check Results button
			auto hDlgErrorCheckResultsButton = GetDlgItem(hWnd, CONTROL_ID_ERROR_CHECK_RESULTS_BUTTON);
			MoveWindow(hDlgErrorCheckResultsButton, currentX, currentY, BOTTOM_RIGHT_SECTION_WIDTH, BIG_BUTTON_HEIGHT, FALSE);
			currentY += BIG_BUTTON_HEIGHT + BASIC_PADDING;

			// Error Check Results button
			auto hDlgSoundFilenameButton = GetDlgItem(hWnd, CONTROL_ID_SOUND_FILENAME_BUTTON);
			MoveWindow(hDlgSoundFilenameButton, currentX, currentY, BOTTOM_RIGHT_SECTION_WIDTH, BIG_BUTTON_HEIGHT, FALSE);
			currentY += BIG_BUTTON_HEIGHT + BASIC_PADDING;

			// OK button
			auto hDlgOkButton = GetDlgItem(hWnd, CONTROL_ID_OK_BUTTON);
			MoveWindow(hDlgOkButton, currentX, clientHeight - BIG_BUTTON_HEIGHT - WINDOW_EDGE_PADDING, BOTTOM_RIGHT_SECTION_WIDTH, BIG_BUTTON_HEIGHT, FALSE);
		}

		// Force redraw. Embrace the flicker.
		RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

		// Store window size for later restoration.
		SIZE winSize = {};
		if (winui::GetWindowSize(hWnd, winSize)) {
			settings.dialogue_window.size = winSize;
		}

		context.setResult(TRUE);
	}

	void OnCellFilterChanged(HWND hWnd, HWND comboBox) {
		const auto userData = (UserData*)GetWindowLongA(hWnd, GWL_USERDATA);
		userData->cellFilterMode = (UserData::CellFilterMode)ComboBox_GetCurSel(comboBox);
		redisplayAllData(hWnd);
	}

	void PatchDialogProc_BeforeCommand(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		auto userData = (UserData*)GetWindowLongA(hWnd, GWL_USERDATA);
		const auto wParam = context.getWParam();
		const auto command = HIWORD(wParam);
		const auto id = LOWORD(wParam);
		switch (command) {
		case BN_CLICKED:
			switch (id) {
			case CONTROL_ID_SHOW_MODIFIED_ONLY_BUTTON:
				userData->modeShowModifiedOnly = SendDlgItemMessageA(hWnd, id, BM_GETCHECK, 0, 0);
				redisplayAllData(hWnd);
				break;
			}
			break;
		case EN_CHANGE:
			switch (id) {
			case CONTROL_ID_CURRENT_TEXT_EDIT:
				OnCurrentTextEditChanged(hWnd);
				break;
			}
			break;
		case CBN_SELCHANGE:
			switch (id) {
			case CONTROL_ID_FILTER_CELL_SETTING_COMBO:
				OnCellFilterChanged(hWnd, (HWND)context.getLParam());
				break;
			}
			break;
		}
	}

	const auto functionNames = reinterpret_cast<const char**>(0x6A5A38);
	const auto compareText = reinterpret_cast<const char**>(0x6A5A20);

	int GetInverseCompareOperator(int compareOp) {
		using CompareOp = DialogueInfo::Condition::CompareOp;
		switch (compareOp) {
		case CompareOp::Equal:
			return CompareOp::NotEqual;
		case CompareOp::NotEqual:
			return CompareOp::Equal;
		case CompareOp::GreaterThan:
			return CompareOp::LessThanOrEqual;
		case CompareOp::GreaterThanOrEqual:
			return CompareOp::LessThan;
		case CompareOp::LessThan:
			return CompareOp::GreaterThanOrEqual;
		case CompareOp::LessThanOrEqual:
			return CompareOp::GreaterThan;
		}
		return compareOp;
	}

	void PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_Object(DialogProcContext& context, NMLVDISPINFOA* displayInfo, DialogueInfo* info, DialogueInfo::Condition* condition, bool invert, const char* wrapper = nullptr) {
		if (condition->compareValue.object == nullptr) {
			context.setResult(FALSE);
			return;
		}

		const auto id = condition->compareValue.object->getObjectID();
		const auto compare = invert ? compareText[GetInverseCompareOperator(condition->compareOp)] : compareText[condition->compareOp];

		if (wrapper) {
			sprintf_s(displayInfo->item.pszText, displayInfo->item.cchTextMax, "%s(%s) %s %d", wrapper, id, compare, (int)condition->value);
		}
		else {
			sprintf_s(displayInfo->item.pszText, displayInfo->item.cchTextMax, "%s %s %d", id, compare, (int)condition->value);
		}

		context.setResult(FALSE);
	}

	void PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_ObjectDialogue(DialogProcContext& context, NMLVDISPINFOA* displayInfo, DialogueInfo* info, DialogueInfo::Condition* condition, bool invert) {
		if (condition->compareValue.dialogue == nullptr || condition->compareValue.dialogue->id == nullptr) {
			context.setResult(FALSE);
			return;
		}

		const auto id = condition->compareValue.dialogue->id;
		const auto compare = invert ? compareText[GetInverseCompareOperator(condition->compareOp)] : compareText[condition->compareOp];

		sprintf_s(displayInfo->item.pszText, displayInfo->item.cchTextMax, "%s %s %d", id, compare, (int)condition->value);

		context.setResult(FALSE);
	}

	void PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_String(DialogProcContext& context, NMLVDISPINFOA* displayInfo, DialogueInfo* info, DialogueInfo::Condition* condition, bool invert) {
		const auto compare = invert ? compareText[GetInverseCompareOperator(condition->compareOp)] : compareText[condition->compareOp];

		sprintf_s(displayInfo->item.pszText, displayInfo->item.cchTextMax, "%s %s %d", condition->compareValue.string, compare, (int)condition->value);

		context.setResult(FALSE);
	}

	void PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_Function(DialogProcContext& context, NMLVDISPINFOA* displayInfo, DialogueInfo* info, DialogueInfo::Condition* condition) {
		const auto function = functionNames[condition->compareValue.integer];
		const auto compare = compareText[condition->compareOp];

		sprintf_s(displayInfo->item.pszText, displayInfo->item.cchTextMax, "%s %s %d", function, compare, (int)condition->value);

		context.setResult(FALSE);
	}

	void PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar(DialogProcContext& context) {
		const auto displayInfo = context.getNotificationListViewDisplayInfo();
		const auto info = reinterpret_cast<DialogueInfo*>(displayInfo->item.lParam);
		const auto conditionIndex = displayInfo->item.iSubItem - 6;
		const auto condition = &info->conditions[conditionIndex];

		switch (condition->type) {
		case DialogueInfo::Condition::TypeFunction:
			PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_Function(context, displayInfo, info, condition);
			break;
		case DialogueInfo::Condition::TypeGlobal:
			PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_Object(context, displayInfo, info, condition, false);
			break;
		case DialogueInfo::Condition::TypeLocal:
			PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_String(context, displayInfo, info, condition, false);
			break;
		case DialogueInfo::Condition::TypeJournal:
			PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_ObjectDialogue(context, displayInfo, info, condition, false);
			break;
		case DialogueInfo::Condition::TypeItem:
			PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_Object(context, displayInfo, info, condition, false);
			break;
		case DialogueInfo::Condition::TypeDead:
			PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_Object(context, displayInfo, info, condition, false, "dead");
			break;
		case DialogueInfo::Condition::TypeNotID:
		case DialogueInfo::Condition::TypeNotFaction:
		case DialogueInfo::Condition::TypeNotClass:
		case DialogueInfo::Condition::TypeNotRace:
		case DialogueInfo::Condition::TypeNotCell:
			PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_Object(context, displayInfo, info, condition, true);
			break;
		case DialogueInfo::Condition::TypeNotLocal:
			PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar_String(context, displayInfo, info, condition, true);
			break;
		}
	}

	void PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo(DialogProcContext& context) {
		const auto lParam = context.getNotificationListViewDisplayInfo();
		switch (lParam->item.iSubItem) {
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo_FunVar(context);
			break;
		}
	}

	void PatchDialogProc_BeforeNotify_InfoList(DialogProcContext& context) {
		const auto lParam = context.getNotificationData();
		switch (lParam->code) {
		case LVN_GETDISPINFO:
			PatchDialogProc_BeforeNotify_InfoList_GetDisplayInfo(context);
			break;
		}
	}

	void PatchDialogProc_BeforeNotify(DialogProcContext& context) {
		auto message = context.getNotificationData();
		switch (message->idFrom) {
		case CONTROL_ID_INFO_LIST:
			PatchDialogProc_BeforeNotify_InfoList(context);
			break;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x4EAEA0);

		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_BeforeInitialize(context);
			break;
		case WM_DESTROY:
			PatchDialogProc_BeforeDestroy(context);
			break;
		case WM_SIZE:
			PatchDialogProc_BeforeSize(context);
			break;
		case WM_COMMAND:
			PatchDialogProc_BeforeCommand(context);
			break;
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
		case WM_GETMINMAXINFO:
			PatchDialogProc_GetMinMaxInfo(context);
			break;
		case WM_INITDIALOG:
			PatchDialogProc_AfterInitialize(context);
			break;
		case WM_NOTIFY:
			PatchDialogProc_AfterNotify(context);
			break;
		}

		return context.getResult();
	}

	void installPatches() {
		using memory::genCallEnforced;
		using memory::genCallUnprotected;
		using memory::genJumpEnforced;
		using memory::genJumpUnprotected;
		using memory::genNOPUnprotected;
		using memory::writeDoubleWordEnforced;
		using memory::writePatchCodeUnprotected;
		using memory::writeValueEnforced;

		// Patch: Optimize insertion of cell names.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			genNOPUnprotected(0x4EC686, 0x4EC68D - 0x4EC686);
			genCallUnprotected(0x4EC69A, reinterpret_cast<DWORD>(PatchOptimizeCellInsertion), 0x4EC6C5 - 0x4EC69A);
		}

		// Patch: Ensure variable combo box lists can always fit content.
		genCallUnprotected(0x4E7C14, reinterpret_cast<DWORD>(PatchEnsureVariableComboBoxListWidth_GetCachedHDC), 0x6);
		writeDoubleWordEnforced(0x4E7C28 + 0x2, 0x6D9DB0, reinterpret_cast<DWORD>(&PatchEnsureVariableComboBoxListWidth_SendDlgItemMessageAWrapper_ExternPointer));

		// Patch: Optimize displaying of INFO info.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			// Optimize populating local lists.
			genNOPUnprotected(0x4E7D5A, 0x4E7F59 - 0x4E7D5A);
			writePatchCodeUnprotected(0x4E7D5A, (BYTE*)PatchOptimizePopulatingLocalVariableNames_Setup, PatchOptimizePopulatingLocalVariableNames_Setup_Size);
			genCallUnprotected(0x4E7D5A + PatchOptimizePopulatingLocalVariableNames_Setup_CallOffset, reinterpret_cast<DWORD>(PatchOptimizePopulatingLocalVariableNames));

			// Optimize populating ID lists.
			genNOPUnprotected(0x4E8209, 0x4E828A - 0x4E8209);
			writePatchCodeUnprotected(0x4E8209, (BYTE*)PatchOptimizePopulatingCellVariableNames_Setup, PatchOptimizePopulatingCellVariableNames_Setup_Size);
			genCallUnprotected(0x4E8209 + 0x4, reinterpret_cast<DWORD>(PatchOptimizePopulatingCellVariableNames));

			// Optimize the display of the info itself.
			genJumpEnforced(0x4040BB, 0x4F1070, reinterpret_cast<DWORD>(PatchOptimizeDialogueInfoDisplaying));

			// Optimize the refresh of the topic list.
			genJumpEnforced(0x4031A7, 0x4E70E0, reinterpret_cast<DWORD>(PatchOptimizeTopicListRefresh));
		}

		// Patch: Allow filtering of topic list.
		genCallEnforced(0x4E7143, 0x404160, reinterpret_cast<DWORD>(PatchTopicListAddItem));

		// Patch: Change behavior of local variable filtering.
		genJumpEnforced(0x40484A, 0x4E7C00, reinterpret_cast<DWORD>(PatchFillConditionCombos));

		// Patch: Change behavior of cell filtering.
		genNOPUnprotected(0x4F1D25, 0x4F1DA6 - 0x4F1D25);
		writePatchCodeUnprotected(0x4F1D25, (BYTE*)PatchFilterCellBehavior_Setup, PatchFilterCellBehavior_SetupSize);
		writeValueEnforced<BYTE>(0x4F1DA6, 0x74, 0x75);
		genCallUnprotected(0x4F1D25 + 0x4, reinterpret_cast<DWORD>(PatchFilterCellBehavior));

		// Patch: Extend structure of the dialogue window user data.
		writeValueEnforced<BYTE>(0x4EBFF4 + 0x1, sizeof(UserData_Vanilla), sizeof(UserData));
		genCallUnprotected(0x4EC018, reinterpret_cast<DWORD>(SetExtendedUserData), 0x6);

		// Patch: Extend window message handling.
		genJumpEnforced(0x401334, 0x4EAEA0, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
