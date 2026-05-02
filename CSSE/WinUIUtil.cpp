#include "WinUIUtil.h"

#include "StringUtil.h"

namespace se::cs::winui {

	bool GetIsValidID(int iDlgId) {
		// NOTE: Better Construction Set UI will incorrectly set some IDs to 0xFFFF instead of 0xFFFFFFFF.
		return iDlgId != -1 && iDlgId != 0xFFFF;
	}

	struct GetControlByTextParam {
		HWND result = NULL;
		bool filterHasID = true;
		std::string_view text = "";
	};

	BOOL CALLBACK GetControlByTextEnumChildProc(HWND hChild, LPARAM lParam) {
		auto param = (GetControlByTextParam*)lParam;

		// Ignore elements that already have an ID.
		if (param->filterHasID && GetIsValidID(GetWindowLongA(hChild, GWLP_ID))) {
			return TRUE;
		}

		// Perform comparison.
		const auto text = GetWindowTextA(hChild);
		if (string::equal(text, param->text)) {
			param->result = hChild;
			return FALSE;
		}

		return TRUE;
	}

	HWND GetControlByText(HWND hParent, const std::string_view& text, bool ignoreResultsWithIDs) {
		// Make sure our buffer in GetControlByTextEnumChildProc is big enough.
		if (text.length() > 127) {
			throw std::invalid_argument("Buffer length insufficient to perform operation.");
		}

		GetControlByTextParam param;
		param.filterHasID = ignoreResultsWithIDs;
		param.text = text;

		EnumChildWindows(hParent, GetControlByTextEnumChildProc, (LPARAM)&param);
		return param.result;
	}

	bool SetWindowIdByValue(HWND hParent, const std::string_view text, int newID) {
		auto hWnd = GetControlByText(hParent, text, true);
		if (hWnd == NULL) {
			return false;
		}

		SetWindowLongA(hWnd, GWLP_ID, newID);
		return true;
	}

	bool GetWindowSize(HWND hWnd, SIZE& size) {
		RECT windowRect;
		if (!GetWindowRect(hWnd, &windowRect)) {
			return false;
		}

		size.cx = GetRectWidth(windowRect);
		size.cy = GetRectHeight(windowRect);
		return true;
	}

	void GetWindowRelativeRect(HWND hWnd, RECT* r) {
		// Get the screen position of the control.
		GetWindowRect(hWnd, r);

		// Adjust coordinates to be relative to parent.
		POINT pt1 = { r->left, r->top }, pt2 = { r->right, r->bottom };
		ScreenToClient(GetParent(hWnd), &pt1);
		ScreenToClient(GetParent(hWnd), &pt2);
		r->left = pt1.x;
		r->top = pt1.y;
		r->right = pt2.x;
		r->bottom = pt2.y;
	}

	void CenterWindow(HWND hWnd) {
		RECT windowRect = {};
		GetWindowRect(hWnd, &windowRect);

		const auto screenX = GetSystemMetrics(SM_CXSCREEN);
		const auto screenY = GetSystemMetrics(SM_CYSCREEN);

		const auto width = GetRectWidth(&windowRect);
		const auto height = GetRectHeight(&windowRect);

		MoveWindow(hWnd, (screenX - width) / 2, (screenY - height) / 2, width, height, FALSE);
	}

	void ResizeAndCenterWindow(HWND hWnd, int width, int height) {
		const auto screenX = GetSystemMetrics(SM_CXSCREEN);
		const auto screenY = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(hWnd, (screenX - width) / 2, (screenY - height) / 2, width, height, FALSE);
	}

	bool MoveWindow(HWND hWnd, int x, int y, bool repaint) {
		RECT windowRect;
		if (!GetWindowRect(hWnd, &windowRect)) {
			return false;
		}

		return MoveWindow(hWnd, x, y, GetRectWidth(windowRect), GetRectHeight(windowRect), repaint ? TRUE : FALSE);
	}

	void SetDialogFocus(HWND hWnd, int controlId) {
		const auto hDlgControl = GetDlgItem(hWnd, controlId);
		SendMessageA(hWnd, WM_NEXTDLGCTL, (WPARAM)hDlgControl, TRUE);
	}

	LONG GetStyle(HWND hWnd) {
		return GetWindowLongA(hWnd, GWL_STYLE);
	}

	void SetStyle(HWND hWnd, LONG lStyle) {
		SetWindowLongA(hWnd, GWL_STYLE, lStyle);
	}

	void AddStyles(HWND hWnd, LONG lStyle) {
		SetWindowLongA(hWnd, GWL_STYLE, GetWindowLongA(hWnd, GWL_STYLE) | lStyle);
	}

	void RemoveStyles(HWND hWnd, LONG lStyle) {
		SetWindowLongA(hWnd, GWL_STYLE, GetWindowLongA(hWnd, GWL_STYLE) & ~lStyle);
	}

	std::string GetWindowTextA(HWND hWnd) {
		std::string text;
		
		// We need to account for one extra character, a null-terminator that we'll have to manually remove.
		text.resize(GetWindowTextLengthA(hWnd) + 1);

		GetWindowTextA(hWnd, text.data(), text.capacity());

		// GetWindowTextA will cause a null terminator character to be appended to the string, which we need to pop off.
		text.pop_back();

		return std::move(text);
	}

	std::optional<int> GetDlgItemSignedInt(HWND hWnd, UINT nIDDlgItem) {
		BOOL successful;
		auto result = GetDlgItemInt(hWnd, nIDDlgItem, &successful, TRUE);
		if (!successful) {
			return {};
		}

		return (int)result;
	}

	std::optional<unsigned int> GetDlgItemUnsignedInt(HWND hWnd, UINT nIDDlgItem) {
		BOOL successful;
		auto result = GetDlgItemInt(hWnd, nIDDlgItem, &successful, FALSE);
		if (!successful) {
			return {};
		}

		return result;
	}

	BOOL GetOpenFileNameWithoutDirChangeA(LPOPENFILENAMEA param) {
		// Cache current directory because GetOpenFileName changes it for some absurd reason.
		// OFN_NOCHANGEDIR doesn't seem to act reliably in this situation.
		const auto currentDir = std::filesystem::current_path();

		auto result = GetOpenFileNameA(param);

		// Restore directory.
		std::filesystem::current_path(currentDir);

		return result;
	}

	//
	// ComboBox
	//

	bool ComboBox_HasStringExact(HWND hWnd, const char* string) {
		return ComboBox_FindStringExact(hWnd, -1, string) != CB_ERR;
	}

	void ComboBox_SetCurSelEx(HWND hWnd, int index) {
		ComboBox_SetCurSel(hWnd, index);
		SendMessageA(GetAncestor(hWnd, GA_PARENT), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hWnd, GWLP_ID), CBN_SELCHANGE), (LPARAM)hWnd);
	}

	int ComboBox_SelectStringExact(HWND hWnd, const char* string) {
		const auto index = ComboBox_FindStringExact(hWnd, 0, string);
		if (index == CB_ERR) {
			return CB_ERR;
		}

		return ComboBox_SetCurSel(hWnd, index);
	}

	//
	// ListView
	//

	int ListView_FindItemByParam(HWND hWnd, LPARAM lParam) {
		LVFINDINFOA findInfo = {};
		findInfo.flags = LVFI_PARAM;
		findInfo.lParam = lParam;
		return ListView_FindItem(hWnd, -1, &findInfo);
	}

	LPARAM ListView_GetItemData(HWND hWnd, int item, int subItem) {
		LV_ITEM lv_item = {};
		lv_item.mask = LVIF_PARAM;
		lv_item.iItem = item;
		lv_item.iSubItem = subItem;
		ListView_GetItem(hWnd, &lv_item);

		return lv_item.lParam;
	}

	bool ListView_SelectByParam(HWND hWnd, LPARAM lParam, bool ensureVisible) {
		const int index = ListView_FindItemByParam(hWnd, lParam);
		if (index == -1) {
			return false;
		}

		ListView_SetItemState(hWnd, index, LVIS_SELECTED, LVIS_SELECTED);

		if (ensureVisible) {
			ListView_EnsureVisible(hWnd, index, FALSE);
		}

		return true;
	}

	//
	// Static
	//

	bool Static_GetDesiredSize(HWND hWnd, SIZE& size) {
		size = {};

		// Get the static's text.
		const auto text = GetWindowTextA(hWnd);

		// Get actual size.
		auto hDC = GetDC(hWnd);
		if (!GetTextExtentPoint32(hDC, text.data(), text.length(), &size)) {
			return false;
		}

		return true;
	}

	//
	// TabView
	//

	void TabCtrl_GetInteriorRect(HWND hWnd, RECT* r) {
		// Perform base adjustments.
		GetClientRect(hWnd, r);
		TabCtrl_AdjustRect(hWnd, FALSE, r);

		// Get the offset of the control.
		RECT controlRect = {};
		GetWindowRelativeRect(hWnd, &controlRect);

		// Perform relative to parent adjustments.
		r->left += controlRect.left;
		r->right += controlRect.left;
		r->top += controlRect.top;
		r->bottom += controlRect.top;
	}

	void TabCtrl_SetCurSelEx(HWND hWnd, int index) {
		auto hParent = GetAncestor(hWnd, GA_PARENT);

		NMHDR nmhdr = {};
		nmhdr.hwndFrom = hWnd;
		nmhdr.idFrom = GetDlgCtrlID(hWnd);

		nmhdr.code = TCN_SELCHANGING;
		if (SendMessageA(hParent, WM_NOTIFY, (WPARAM)hWnd, (LPARAM)&nmhdr) == FALSE) {
			TabCtrl_SetCurSel(hWnd, index);

			const int foo = TCN_SELCHANGE;
			nmhdr.code = TCN_SELCHANGE;
			SendMessageA(hParent, WM_NOTIFY, (WPARAM)hWnd, (LPARAM)&nmhdr);
		}
	}

	//
	// Toolbar
	//

	void Toolbar_AddSeparator(HWND hWndToolbar, int iWidth) {
		TBBUTTON button = {};
		button.iBitmap = iWidth;
		button.idCommand = 0x0FFFFFFFF;
		button.fsState = TBSTATE_ENABLED;
		button.fsStyle = BTNS_SEP;
		SendMessageA(hWndToolbar, TB_ADDBUTTONS, 1, (LPARAM)&button);
	}

	void Toolbar_AddButton(HWND hWndToolbar, int idCommand, int iBitmap) {
		TBBUTTON button = {};
		button.iBitmap = iBitmap;
		button.idCommand = idCommand;
		button.fsState = TBSTATE_ENABLED;
		button.fsStyle = BTNS_BUTTON;
		SendMessageA(hWndToolbar, TB_ADDBUTTONS, 1, (LPARAM)&button);
	}
}
