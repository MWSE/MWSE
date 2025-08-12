#include "DialogCellWindow.h"

#include "CSCell.h"
#include "CSRecordHandler.h"
#include "CSReference.h"

#include "EditBasicExtended.h"

#include "MemoryUtil.h"
#include "LogUtil.h"
#include "StringUtil.h"
#include "WinUIUtil.h"

#include "Settings.h"

#include "DialogProcContext.h"

namespace se::cs::dialog::cell_window {
	using gCellViewWindowHandle = memory::ExternalGlobal<HWND, 0x6CE954>;
	using gActiveEditCell = memory::ExternalGlobal<Cell*, 0x6CDFF4>;

	const auto CS_AddAllToRefsListView = reinterpret_cast<void(__cdecl*)(HWND, const ReferenceList*)>(0x401442);
	const auto CS_RefreshCellListView = reinterpret_cast<void(__cdecl*)(HWND)>(0x4037C4);

	static HWND cellWindowSearchControl = NULL;

	static std::string currentSearchText;
	static std::optional<std::regex> currentSearchRegex;
	static bool modeShowModifiedOnly = false;

	using gSuppressItemChangePropagation = memory::ExternalGlobal<bool, 0x6CDFF8>;
	static int lastCellTopIndex = -1;

	void __cdecl PatchSpeedUpCellViewDialog(HWND hWnd) {
		SendMessageA(hWnd, WM_SETREDRAW, FALSE, NULL);

		lastCellTopIndex = ListView_GetTopIndex(hWnd);

		// Call the original function.
		const auto CS_refreshCellListView = reinterpret_cast<void(__cdecl*)(HWND)>(0x40E250);
		CS_refreshCellListView(hWnd);

		SendMessageA(hWnd, WM_SETREDRAW, TRUE, NULL);
	}

	void __cdecl PatchSpeedUpCellObjectViewDialog(HWND hWnd, const ReferenceList* references) {
		if (references == &gActiveEditCell::get()->unknown_0x40) {
			SendMessageA(hWnd, WM_SETREDRAW, FALSE, NULL);
		}

		const auto CS_addAllToRefsListView = reinterpret_cast<void(__cdecl*)(HWND, const ReferenceList*)>(0x40E5B0);
		CS_addAllToRefsListView(hWnd, references);

		if (references == &gActiveEditCell::get()->unknown_0x30) {
			SendMessageA(hWnd, WM_SETREDRAW, TRUE, NULL);
		}
	}

	bool matchDispatcher(const std::string_view& haystack) {
		if (currentSearchRegex) {
			return std::regex_search(haystack.data(), currentSearchRegex.value());
		}
		else if (settings.object_window.search_settings.case_sensitive) {
			return string::contains(haystack, currentSearchText);
		}
		else {
			return string::cicontains(haystack, currentSearchText);
		}
	}

	bool FilterCellList_CellMatchesSearch(const Cell* cell) {
		if (modeShowModifiedOnly && !cell->getModified()) {
			return false;
		}
		if (currentSearchText.empty()) {
			return true;
		}

		return matchDispatcher(cell->getEditorId());
	}

	bool FilterRefsList_RefMatchesSearch(const Reference* reference) {
		if (modeShowModifiedOnly && !reference->getModified()) {
			return false;
		}
		return true;
	}

	Cell* __fastcall PatchFilterCellList(RecordHandler* recordHandler, DWORD edx, int index) {
		auto cell = recordHandler->getCellByIndex(index);
		return FilterCellList_CellMatchesSearch(cell) ? cell : nullptr;
	}

	const auto CS_addRefrToRefsListView = reinterpret_cast<void(__cdecl*)(HWND, const Reference*)>(0x40E4C0);
	void PatchFilterRefsList(HWND hWnd, const Reference* reference) {
		if (FilterRefsList_RefMatchesSearch(reference)) {
			CS_addRefrToRefsListView(hWnd, reference);
		}
	}

	void RefreshRefsListView(HWND hWnd) {
		auto refsListView = GetDlgItem(hWnd, CONTROL_ID_REFS_LIST_VIEW);
		auto cell = gActiveEditCell::get();

		SendMessageA(refsListView, WM_SETREDRAW, FALSE, NULL);

		SendMessageA(refsListView, LVM_DELETEALLITEMS, 0, 0);
		if (cell) {
			CS_AddAllToRefsListView(refsListView, &cell->unknown_0x40);
			CS_AddAllToRefsListView(refsListView, &cell->unknown_0x30);
		}

		SendMessageA(refsListView, WM_SETREDRAW, TRUE, NULL);
		RedrawWindow(refsListView, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	void __cdecl SelectCell(Cell* cell) {
		if (cell == nullptr) {
			lastCellTopIndex = -1;
			return;
		}

		gSuppressItemChangePropagation::set(true);

		const auto hWnd = gCellViewWindowHandle::get();
		const auto cellListView = GetDlgItem(hWnd, CONTROL_ID_CELL_LIST_VIEW);

		// Always force it to sort.
		SendMessageA(cellListView, LVM_SORTITEMS, memory::ExternalGlobal<WPARAM, 0x6CDF50>::get(), 0x4023CE);

		// Hacky way to restore view... ensure the last item is visible, then the old top index.
		if (lastCellTopIndex != -1) {
			const auto lastIndex = ListView_GetItemCount(cellListView);
			ListView_EnsureVisible(cellListView, lastIndex - 1, TRUE);
			ListView_EnsureVisible(cellListView, lastCellTopIndex, TRUE);
			lastCellTopIndex = -1;
		}

		// Also make sure the selected cell is preserved and visible.
		const auto activeCell = gActiveEditCell::get();
		LVFINDINFOA findInfo = { LVFI_PARAM, NULL, (LPARAM)cell, {}, {} };
		const auto index = ListView_FindItem(cellListView, 0, &findInfo);
		if (index > 0) {
			ListView_SetItemState(cellListView, index, LVIS_SELECTED, LVIS_SELECTED);
			ListView_EnsureVisible(cellListView, index, TRUE);
		}

		gSuppressItemChangePropagation::set(false);
	}

	void RefreshCellListView(HWND hWnd) {
		auto cellListView = GetDlgItem(hWnd, CONTROL_ID_CELL_LIST_VIEW);
		CS_RefreshCellListView(cellListView);
	}

	void OnFilterEditChanged(HWND hWnd) {
		using namespace se::cs::winui;

		// Get current search text.
		auto newText = GetWindowTextA(cellWindowSearchControl);

		// Transform the search text to lowercase and clear stray characters.
		string::to_lower(newText);

		if (!string::equal(currentSearchText, newText)) {
			currentSearchText = std::move(newText);

			// Regex crunching can be slow, so only do it once.
			if (settings.object_window.search_settings.use_regex) {
				auto flags = std::regex_constants::extended | std::regex_constants::optimize | std::regex_constants::nosubs;
				if (!settings.object_window.search_settings.case_sensitive) {
					flags |= std::regex_constants::icase;
				}

				try {
					currentSearchRegex = std::regex(currentSearchText, flags);
				}
				catch (const std::regex_error& e) {
					log::stream << "Regex error when searching cell refs window: " << e.what() << std::endl;
					currentSearchRegex = {};

					// TODO: Paint the background of the search input red or something.
				}
			}
			else {
				currentSearchRegex = {};
			}

			// Search affects cell list.
			RefreshCellListView(hWnd);
			SelectCell(gActiveEditCell::get());
		}
	}

	void CALLBACK PatchDialogProc_AfterSize(DialogProcContext& context) {
		using winui::GetRectHeight;
		using winui::GetRectWidth;
		using winui::GetWindowRelativeRect;

		const auto hDlg = context.getWindowHandle();

		auto cellListView = GetDlgItem(hDlg, CONTROL_ID_CELL_LIST_VIEW);
		auto refsListLabel = GetDlgItem(hDlg, CONTROL_ID_REFS_LABEL);
		auto refsListView = GetDlgItem(hDlg, CONTROL_ID_REFS_LIST_VIEW);
		auto showModifiedButton = GetDlgItem(hDlg, CONTROL_ID_SHOW_MODIFIED_ONLY_BUTTON);
		auto searchLabel = GetDlgItem(hDlg, CONTROL_ID_FILTER_LABEL);
		auto searchEdit = GetDlgItem(hDlg, CONTROL_ID_FILTER_EDIT);

		// Update globals.
		cellWindowSearchControl = searchEdit;

		const auto lParam = context.getLParam();
		const auto mainWidth = LOWORD(lParam);
		const auto mainHeight = HIWORD(lParam);

		constexpr auto BASIC_PADDING = 2;
		constexpr auto REFERENCE_LIST_TITLE_PADDING = BASIC_PADDING * 4;
		constexpr auto STATIC_HEIGHT = 13;
		constexpr auto EDIT_HEIGHT = 21;
		constexpr auto STATIC_COMBO_OFFSET = (EDIT_HEIGHT - STATIC_HEIGHT) / 2;

		// Make room for our new search bar.
		const auto viewWidth = (mainWidth - BASIC_PADDING * 3) / 2;
		MoveWindow(cellListView, BASIC_PADDING, BASIC_PADDING, viewWidth, mainHeight - EDIT_HEIGHT - BASIC_PADDING * 3, TRUE);
		MoveWindow(refsListLabel, BASIC_PADDING * 2 + viewWidth, REFERENCE_LIST_TITLE_PADDING, viewWidth, STATIC_HEIGHT, TRUE);
		MoveWindow(refsListView, BASIC_PADDING * 2 + viewWidth, REFERENCE_LIST_TITLE_PADDING * 2 + STATIC_HEIGHT, viewWidth, mainHeight - EDIT_HEIGHT - BASIC_PADDING * 2 - REFERENCE_LIST_TITLE_PADDING * 2 - STATIC_HEIGHT, TRUE);

		// Update the search bar placement.
		int currentY = mainHeight - EDIT_HEIGHT - BASIC_PADDING;
		auto searchEditWidth = std::min<int>(mainWidth - BASIC_PADDING * 2, 300);
		MoveWindow(showModifiedButton, 9, currentY, 160, EDIT_HEIGHT, TRUE);
		MoveWindow(searchLabel, (mainWidth - 21) - searchEditWidth - 54 - BASIC_PADDING, currentY + STATIC_COMBO_OFFSET, 54, STATIC_HEIGHT, TRUE);
		MoveWindow(searchEdit, (mainWidth - 21) - searchEditWidth, currentY, searchEditWidth, EDIT_HEIGHT, FALSE);

		RedrawWindow(hDlg, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	void CALLBACK PatchDialogProc_AfterCreate(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		auto hInstance = (HINSTANCE)GetWindowLongA(hWnd, GWLP_HINSTANCE);

		// Make it so only one row in the cell list can be selected at a time.
		const auto hCellList = GetDlgItem(hWnd, CONTROL_ID_CELL_LIST_VIEW);
		winui::AddStyles(hCellList, LVS_SINGLESEL);

		// Ensure our custom filter box is added.
		if (cellWindowSearchControl == NULL) {

			auto hDlgShowModifiedOnly = CreateWindowExA(NULL, WC_BUTTON, "Show modified only", BS_AUTOCHECKBOX | BS_PUSHLIKE | WS_CHILD | WS_VISIBLE | WS_GROUP, 0, 0, 0, 0, hWnd, (HMENU)CONTROL_ID_SHOW_MODIFIED_ONLY_BUTTON, hInstance, NULL);
			auto hDlgFilterStatic = CreateWindowExA(NULL, WC_STATIC, "Filter cells:", SS_RIGHT | WS_CHILD | WS_VISIBLE | WS_GROUP, 0, 0, 0, 0, hWnd, (HMENU)CONTROL_ID_FILTER_LABEL, hInstance, NULL);
			auto hDlgFilterEdit = CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDIT, "", ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 0, 0, 0, 0, hWnd, (HMENU)CONTROL_ID_FILTER_EDIT, hInstance, NULL);
			if (hDlgFilterEdit) {
				SetWindowSubclass(hDlgFilterEdit, ui_subclass::edit::BasicExtendedProc, NULL, NULL);

				auto font = SendMessageA(hWnd, WM_GETFONT, FALSE, FALSE);
				SendMessageA(hDlgShowModifiedOnly, WM_SETFONT, font, MAKELPARAM(TRUE, FALSE));
				SendMessageA(hDlgFilterStatic, WM_SETFONT, font, MAKELPARAM(TRUE, FALSE));
				SendMessageA(hDlgFilterEdit, WM_SETFONT, font, MAKELPARAM(TRUE, FALSE));
			}
			else {
				log::stream << "ERROR: Could not create search control!" << std::endl;
			}
		}
	}

	void PatchDialogProc_BeforeCommand(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto wParam = context.getWParam();
		const auto command = HIWORD(wParam);
		const auto id = LOWORD(wParam);

		switch (command) {
		case BN_CLICKED:
			switch (id) {
			case CONTROL_ID_SHOW_MODIFIED_ONLY_BUTTON:
				modeShowModifiedOnly = SendDlgItemMessageA(hWnd, id, BM_GETCHECK, 0, 0);
				RefreshCellListView(hWnd);
				SelectCell(gActiveEditCell::get());
				RefreshRefsListView(hWnd);
				break;
			}
			break;
		case EN_CHANGE:
			switch (id) {
			case CONTROL_ID_FILTER_EDIT:
				OnFilterEditChanged(hWnd);
				break;
			}
			break;
		}
	}

	void OnNotifyFromListView(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		const auto hdr = (NMHDR*)context.getLParam();

		if (hdr->code == NM_CUSTOMDRAW && settings.object_window.highlight_modified_items) {
			LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)hdr;

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

	void PatchDialogProc_BeforeNotify(DialogProcContext& context) {
		switch (context.getWParam()) {
		case CONTROL_ID_CELL_LIST_VIEW:
		case CONTROL_ID_REFS_LIST_VIEW:
			OnNotifyFromListView(context);
			break;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x40EFA0);

		// Handle pre-patches.
		switch (msg) {
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

		// Handle post-patches.
		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_AfterCreate(context);
			break;
		case WM_SIZE:
			PatchDialogProc_AfterSize(context);
			break;
		}

		return context.getResult();
	}

	void installPatches() {
		using memory::genCallEnforced;
		using memory::genJumpEnforced;

		// Patch: Optimize displaying of cell view window.
		genJumpEnforced(0x4037C4, 0x40E250, reinterpret_cast<DWORD>(PatchSpeedUpCellViewDialog));

		// Patch: Preserve position when selecting new cells.
		genJumpEnforced(0x404953, 0x4105D0, reinterpret_cast<DWORD>(SelectCell));

		// Patch: Optimize displaying of cell objects view window.
		genJumpEnforced(0x401442, 0x40E5B0, reinterpret_cast<DWORD>(PatchSpeedUpCellObjectViewDialog));

		// Patch: Extend window messages.
		genJumpEnforced(0x401898, 0x40EFA0, reinterpret_cast<DWORD>(PatchDialogProc));

		// Patch: Allow filtering of cell list.
		genCallEnforced(0x40E29C, 0x401230, reinterpret_cast<DWORD>(PatchFilterCellList));

		// Patch: Allow filtering of refs list.
		genCallEnforced(0x40E5C7, 0x4033EB, reinterpret_cast<DWORD>(PatchFilterRefsList));
	}
}
