#pragma once

namespace se::cs::winui {

	//
	// Generic extensions
	//

	bool GetIsValidID(int iDlgId);

	HWND GetControlByText(HWND hParent, const std::string_view& text, bool ignoreResultsWithIDs);
	bool SetWindowIdByValue(HWND hParent, const std::string_view text, int newID);

	inline LONG GetRectWidth(const RECT* rect) {
		return rect->right - rect->left;
	}

	inline LONG GetRectWidth(const RECT& rect) {
		return GetRectWidth(&rect);
	}

	inline LONG GetRectHeight(const RECT* rect) {
		return rect->bottom - rect->top;
	}

	inline LONG GetRectHeight(const RECT& rect) {
		return GetRectHeight(&rect);
	}

	bool GetWindowSize(HWND hWnd, SIZE& size);
	void GetWindowRelativeRect(HWND hWnd, RECT* r);

	void CenterWindow(HWND hWnd);
	void ResizeAndCenterWindow(HWND hWnd, int width, int height);
	bool MoveWindow(HWND hWnd, int x, int y, bool repaint = false);

	void SetDialogFocus(HWND hWnd, int controlId);

	LONG GetStyle(HWND hWnd);
	void SetStyle(HWND hWnd, LONG lStyle);
	void AddStyles(HWND hWnd, LONG lStyle);
	void RemoveStyles(HWND hWnd, LONG lStyle);

	std::string GetWindowTextA(HWND hWnd);
	std::optional<int> GetDlgItemSignedInt(HWND hWnd, UINT nIDDlgItem);
	std::optional<unsigned int> GetDlgItemUnsignedInt(HWND hWnd, UINT nIDDlgItem);

	BOOL GetOpenFileNameWithoutDirChangeA(LPOPENFILENAMEA param);

	//
	// ComboBox
	//

	// Determines if the exact string exists in the combo box.
	bool ComboBox_HasStringExact(HWND hWnd, const char* string);

	// Sets the current selection index as ComboBox_SetCurSel, with additional messaging.
	void ComboBox_SetCurSelEx(HWND hWnd, int index);

	// Selects the entry with the given exact text. Returns CB_ERR if no selection change was made.
	int ComboBox_SelectStringExact(HWND hWnd, const char* string);

	//
	// ListView
	//

	// Provides the index of the row that contains the given LPARAM value.
	int ListView_FindItemByParam(HWND hWnd, LPARAM lParam);

	// Gets the LPARAM of a given item/subitem from a list.
	LPARAM ListView_GetItemData(HWND hWnd, int item, int subItem);

	// Selects a row by a given LPARAM, and optionally ensures it is visible.
	bool ListView_SelectByParam(HWND hWnd, LPARAM lParam, bool ensureVisible = true);

	//
	// Static
	//

	bool Static_GetDesiredSize(HWND hWnd, SIZE& size);

	//
	// TabView
	//

	// Returns the window coordinates that are contained by the tab control.
	void TabCtrl_GetInteriorRect(HWND hWnd, RECT* out_rect);

	// Sets the current selection index as TabCtrl_SetCurSel, with additional messaging.
	// This can be blocked if the changing notification returns TRUE.
	void TabCtrl_SetCurSelEx(HWND hWnd, int index);

	//
	// Toolbar
	//

	void Toolbar_AddSeparator(HWND hWndToolbar, int iWidth = 0);
	void Toolbar_AddButton(HWND hWndToolbar, int idCommand, int iBitmap = I_IMAGENONE);

}
