#include "ThemeEngine.h"

#include "LogUtil.h"
#include "Settings.h"
#include "MemoryUtil.h"

#include <CommCtrl.h>
#include <dwmapi.h>
#include <uxtheme.h>

namespace se::cs::theme {
	namespace {
		constexpr auto TOOLBAR_IMAGE_LIST_PROP = "CSSE.Theme.ToolbarImageList";

		struct MenuItemThemeData {
			std::string text;
			bool topLevel = false;
			bool separator = false;
		};

		static std::vector<std::unique_ptr<MenuItemThemeData>> g_MenuItemData;
		static HFONT g_hMenuFont = nullptr;
		static std::optional<std::filesystem::file_time_type> g_LastThemeWriteTime;
		static bool g_IsRefreshing = false;
		static bool g_IsTheming = false;

		bool isTopLevelWindow(HWND hWnd) {
			return (GetWindowLongA(hWnd, GWL_STYLE) & WS_CHILD) == 0;
		}

		std::string getWindowClassName(HWND hWnd) {
			char className[64] = {};
			GetClassNameA(hWnd, className, sizeof(className));
			return className;
		}

		bool isWindowClass(HWND hWnd, const char* className) {
			char currentClassName[64] = {};
			GetClassNameA(hWnd, currentClassName, sizeof(currentClassName));
			return _stricmp(currentClassName, className) == 0;
		}

		COLORREF blendColor(COLORREF source, COLORREF target, double amount) {
			amount = std::clamp(amount, 0.0, 1.0);
			const auto blendChannel = [amount](BYTE sourceChannel, BYTE targetChannel) {
				return static_cast<BYTE>(std::clamp(
					static_cast<int>(std::lround(sourceChannel + (targetChannel - sourceChannel) * amount)),
					0,
					255
				));
			};

			return RGB(
				blendChannel(GetRValue(source), GetRValue(target)),
				blendChannel(GetGValue(source), GetGValue(target)),
				blendChannel(GetBValue(source), GetBValue(target))
			);
		}

		COLORREF darkenColor(COLORREF color, double amount) {
			return blendColor(color, RGB(0, 0, 0), amount);
		}

		COLORREF lightenColor(COLORREF color, double amount) {
			return blendColor(color, RGB(255, 255, 255), amount);
		}

		void ensureMenuFont() {
			if (g_hMenuFont) {
				return;
			}

			NONCLIENTMETRICSA metrics = {};
			metrics.cbSize = sizeof(metrics);
			if (SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0)) {
				g_hMenuFont = CreateFontIndirectA(&metrics.lfMenuFont);
			}

			if (!g_hMenuFont) {
				g_hMenuFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			}
		}

		void clearMenuThemeData() {
			g_MenuItemData.clear();
			if (g_hMenuFont && g_hMenuFont != GetStockObject(DEFAULT_GUI_FONT)) {
				DeleteObject(g_hMenuFont);
			}
			g_hMenuFont = nullptr;
		}
	}

	//
	// Brush cache — lazily creates solid brushes, destroys them on shutdown/theme change.
	//

	static std::unordered_map<COLORREF, HBRUSH> g_BrushCache;

	HBRUSH getCachedBrush(COLORREF color) {
		auto it = g_BrushCache.find(color);
		if (it != g_BrushCache.end()) {
			return it->second;
		}
		HBRUSH brush = CreateSolidBrush(color);
		g_BrushCache[color] = brush;
		return brush;
	}

	static void clearBrushCache() {
		for (auto& [color, brush] : g_BrushCache) {
			DeleteObject(brush);
		}
		g_BrushCache.clear();
	}

	//
	// Feature detection for DwmSetWindowAttribute dark mode (Windows 10 1903+).
	//

	using DwmSetWindowAttributeFn = HRESULT(WINAPI*)(HWND, DWORD, LPCVOID, DWORD);
	static DwmSetWindowAttributeFn g_pDwmSetWindowAttribute = nullptr;
	static bool g_DwmChecked = false;

	static void ensureDwmFunction() {
		if (g_DwmChecked) return;
		g_DwmChecked = true;

		HMODULE hDwm = LoadLibraryA("dwmapi.dll");
		if (hDwm) {
			g_pDwmSetWindowAttribute = (DwmSetWindowAttributeFn)GetProcAddress(hDwm, "DwmSetWindowAttribute");
		}
	}

	/// Applies the dark title bar attribute to a top-level window.
	/// Uses DWMWA_USE_IMMERSIVE_DARK_MODE (attribute 20), requires Windows 10 1903+.
	static void applyDarkTitleBar(HWND hWnd) {
		ensureDwmFunction();
		if (!g_pDwmSetWindowAttribute) return;

		if (!isTopLevelWindow(hWnd)) return;

		// Attribute 20 = DWMWA_USE_IMMERSIVE_DARK_MODE.
		BOOL value = isEnabled() && settings.color_theme.use_dark_title_bar;
		g_pDwmSetWindowAttribute(hWnd, 20, &value, sizeof(value));
	}

	//
	// UxTheme — SetWindowTheme to strip visual styles from common controls.
	//

	using SetWindowThemeFn = HRESULT(WINAPI*)(HWND, LPCWSTR, LPCWSTR);
	static SetWindowThemeFn g_pSetWindowTheme = nullptr;
	static bool g_UxThemeChecked = false;

	static void ensureUxThemeFunction() {
		if (g_UxThemeChecked) return;
		g_UxThemeChecked = true;

		HMODULE hUx = LoadLibraryA("uxtheme.dll");
		if (hUx) {
			g_pSetWindowTheme = (SetWindowThemeFn)GetProcAddress(hUx, "SetWindowTheme");
		}
	}

	//
	// SetPreferredAppMode — ordinal 135 in uxtheme.dll (Win10+).
	// ForceDark (2) makes Explorer::ScrollBar and other supported controls render dark.
	// AllowDarkModeForWindow — ordinal 133, enables dark mode for a specific HWND.
	//

	using SetPreferredAppModeFn = int(WINAPI*)(int);
	static SetPreferredAppModeFn g_pSetPreferredAppMode = nullptr;
	static bool g_AppModeChecked = false;

	using AllowDarkModeForWindowFn = bool(WINAPI*)(HWND, bool);
	static AllowDarkModeForWindowFn g_pAllowDarkModeForWindow = nullptr;
	static bool g_AllowDarkChecked = false;

	static void ensureAppModeFunctions() {
		HMODULE hUx = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (!hUx) return;

		if (!g_AppModeChecked) {
			g_AppModeChecked = true;
			g_pSetPreferredAppMode = (SetPreferredAppModeFn)GetProcAddress(hUx, MAKEINTRESOURCEA(135));
		}
		if (!g_AllowDarkChecked) {
			g_AllowDarkChecked = true;
			g_pAllowDarkModeForWindow = (AllowDarkModeForWindowFn)GetProcAddress(hUx, MAKEINTRESOURCEA(133));
		}
	}

	static void updatePreferredAppMode() {
		ensureAppModeFunctions();
		if (g_pSetPreferredAppMode) {
			// 2 = ForceDark, 0 = Default
			g_pSetPreferredAppMode(isEnabled() ? 2 : 0);
		}
	}

	static void applyAllowDarkMode(HWND hWnd) {
		if (!isEnabled()) return;
		ensureAppModeFunctions();
		if (g_pAllowDarkModeForWindow) {
			g_pAllowDarkModeForWindow(hWnd, true);
		}
	}

	//
	// Dark scrollbar fix — patches comctl32's delay-load IAT slot for OpenNcThemeData
	// (ordinal 49 in uxtheme.dll) to redirect "ScrollBar" → "Explorer::ScrollBar".
	// Explorer::ScrollBar has a dark variant that respects the app's preferred mode.
	// Technique from: https://github.com/ysc3839/win32-darkmode (MIT)
	//

	using fnOpenNcThemeData = HTHEME(WINAPI*)(HWND, LPCWSTR);
	static fnOpenNcThemeData _OpenNcThemeData = nullptr;

	static HTHEME WINAPI myOpenNcThemeData(HWND hWnd, LPCWSTR classList) {
		if (wcscmp(classList, L"ScrollBar") == 0) {
			hWnd = nullptr;
			classList = L"Explorer::ScrollBar";
		}
		return _OpenNcThemeData(hWnd, classList);
	}

	// Minimal PE helpers (inlined from IatHook.h, MIT licensed, ysc3839/win32-darkmode).
	template<typename T>
	T rva2va(void* base, DWORD rva) {
		return reinterpret_cast<T>(reinterpret_cast<ULONG_PTR>(base) + rva);
	}

	static PIMAGE_THUNK_DATA findDelayLoadThunkByOrdinal(void* moduleBase, const char* dllName, uint16_t ordinal) {
		auto* dosHdr = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase);
		auto* ntHdr = rva2va<PIMAGE_NT_HEADERS>(moduleBase, dosHdr->e_lfanew);
		auto& dataDir = ntHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];
		if (!dataDir.VirtualAddress) return nullptr;

		auto* desc = rva2va<PIMAGE_DELAYLOAD_DESCRIPTOR>(moduleBase, dataDir.VirtualAddress);
		for (; desc->DllNameRVA; ++desc) {
			if (_stricmp(rva2va<LPCSTR>(moduleBase, desc->DllNameRVA), dllName) != 0) continue;

			auto* nameThunk = rva2va<PIMAGE_THUNK_DATA>(moduleBase, desc->ImportNameTableRVA);
			auto* addrThunk = rva2va<PIMAGE_THUNK_DATA>(moduleBase, desc->ImportAddressTableRVA);
			for (; nameThunk->u1.Ordinal; ++nameThunk, ++addrThunk) {
				if (IMAGE_SNAP_BY_ORDINAL(nameThunk->u1.Ordinal) &&
					IMAGE_ORDINAL(nameThunk->u1.Ordinal) == ordinal)
					return addrThunk;
			}
		}
		return nullptr;
	}

	static void fixDarkScrollBar() {
		static bool s_Applied = false;
		if (s_Applied) return;
		s_Applied = true;

		HMODULE hUxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (!hUxtheme) return;

		_OpenNcThemeData = (fnOpenNcThemeData)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(49));
		if (!_OpenNcThemeData) return;

		HMODULE hComctl = LoadLibraryExW(L"comctl32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (!hComctl) return;

		auto* addr = findDelayLoadThunkByOrdinal(hComctl, "uxtheme.dll", 49);
		if (!addr) return;

		DWORD oldProtect = 0;
		if (VirtualProtect(addr, sizeof(IMAGE_THUNK_DATA), PAGE_READWRITE, &oldProtect)) {
			addr->u1.Function = reinterpret_cast<ULONG_PTR>(myOpenNcThemeData);
			VirtualProtect(addr, sizeof(IMAGE_THUNK_DATA), oldProtect, &oldProtect);
			log::stream << "Theme engine: patched comctl32 OpenNcThemeData for dark scrollbars." << std::endl;
		}
	}

	//
	// Helper: check common Win32 class names.
	//

	static bool isPushButtonStyle(LONG style) {
		const auto buttonType = style & BS_TYPEMASK;
		// BS_OWNERDRAW (0xB) overwrites the type bits when ORed onto BS_PUSHBUTTON (0x0)
		// or BS_DEFPUSHBUTTON (0x1), so recognise it here so drawOwnerDrawButton continues
		// to handle buttons that were already converted to owner-draw by themeControl.
		return buttonType == BS_PUSHBUTTON
			|| buttonType == BS_DEFPUSHBUTTON
			|| buttonType == BS_OWNERDRAW
			|| (style & BS_PUSHLIKE) != 0;
	}

	static bool isRichEditControl(HWND hWnd) {
		const auto className = getWindowClassName(hWnd);
		return _stricmp(className.c_str(), "RichEdit20A") == 0
			|| _stricmp(className.c_str(), "RichEdit20W") == 0
			|| _stricmp(className.c_str(), "RICHEDIT50W") == 0;
	}

	static void themeRichEditControl(HWND hWnd) {
		if (!isEnabled() || !isRichEditControl(hWnd)) {
			return;
		}

		SendMessageA(hWnd, EM_SETBKGNDCOLOR, 0, settings.color_theme.packed_edit_bg);

		CHARFORMATA format = {};
		format.cbSize = sizeof(format);
		format.dwMask = CFM_COLOR;
		format.crTextColor = settings.color_theme.packed_edit_text;
		SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&format));
	}

	static void themeMenuRecursive(HMENU hMenu, bool topLevel) {
		if (!hMenu) {
			return;
		}

		const auto menuItemCount = GetMenuItemCount(hMenu);
		for (auto itemIndex = 0; itemIndex < menuItemCount; ++itemIndex) {
			char buffer[256] = {};
			MENUITEMINFOA info = {};
			info.cbSize = sizeof(info);
			info.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_SUBMENU | MIIM_DATA;
			info.dwTypeData = buffer;
			info.cch = sizeof(buffer);

			if (!GetMenuItemInfoA(hMenu, itemIndex, TRUE, &info)) {
				continue;
			}

			if ((info.fType & MFT_SEPARATOR) != 0) {
				continue;
			}

			auto* themeData = reinterpret_cast<MenuItemThemeData*>(info.dwItemData);
			if (themeData == nullptr) {
				auto newThemeData = std::make_unique<MenuItemThemeData>();
				themeData = newThemeData.get();
				g_MenuItemData.push_back(std::move(newThemeData));
			}

			themeData->text = buffer;
			themeData->topLevel = topLevel;
			themeData->separator = false;

			MENUITEMINFOA ownerDrawInfo = {};
			ownerDrawInfo.cbSize = sizeof(ownerDrawInfo);
			ownerDrawInfo.fMask = MIIM_FTYPE | MIIM_DATA;
			ownerDrawInfo.fType = (info.fType & ~MFT_STRING) | MFT_OWNERDRAW;
			ownerDrawInfo.dwItemData = reinterpret_cast<ULONG_PTR>(themeData);
			SetMenuItemInfoA(hMenu, itemIndex, TRUE, &ownerDrawInfo);

			if (info.hSubMenu) {
				themeMenuRecursive(info.hSubMenu, false);
			}
		}
	}

	static void invalidateWindowTree(HWND hWnd) {
		if (!IsWindow(hWnd)) {
			return;
		}

		RedrawWindow(hWnd, nullptr, nullptr, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	}

	static void themeToolbarWindow(HWND hWndToolbar) {
		if (!isEnabled()) {
			return;
		}

		SendMessageA(hWndToolbar, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);
	}

	static void themeStatusBarWindow(HWND hWndStatusBar) {
		if (!isEnabled()) {
			return;
		}

		SendMessageA(hWndStatusBar, SB_SETBKCOLOR, 0, settings.color_theme.packed_statusbar_bg);
	}

	static void themeTabControlWindow(HWND hWndTab) {
		if (!isEnabled()) {
			return;
		}

		if (g_pSetWindowTheme) {
			// Tabs need classic rendering when owner-drawn; themed Explorer tabs keep painting
			// their body/background with system colors and leave visible white seams.
			g_pSetWindowTheme(hWndTab, L"", L"");
		}

		SetWindowLongA(hWndTab, GWL_STYLE, GetWindowLongA(hWndTab, GWL_STYLE) | TCS_OWNERDRAWFIXED);
		SetWindowPos(hWndTab, nullptr, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	static bool eraseTabControlBackground(HWND hWnd, HDC hdc) {
		if (!isEnabled() || !isWindowClass(hWnd, WC_TABCONTROL)) {
			return false;
		}

		RECT clientRect = {};
		GetClientRect(hWnd, &clientRect);
		FillRect(hdc, &clientRect, getCachedBrush(settings.color_theme.packed_tab_inactive_bg));

		RECT displayRect = clientRect;
		if (TabCtrl_AdjustRect(hWnd, FALSE, &displayRect)) {
			FrameRect(hdc, &displayRect, getCachedBrush(darkenColor(settings.color_theme.packed_tab_inactive_bg, 0.22)));
		}

		return true;
	}

	static bool isReportListView(HWND hWnd) {
		return isWindowClass(hWnd, WC_LISTVIEW) && (GetWindowLongA(hWnd, GWL_STYLE) & LVS_TYPEMASK) == LVS_REPORT;
	}

	static COLORREF getDividerColor(COLORREF backgroundColor) {
		const auto brightness = GetRValue(backgroundColor) + GetGValue(backgroundColor) + GetBValue(backgroundColor);
		return brightness < (128 * 3)
			? lightenColor(backgroundColor, 0.18)
			: darkenColor(backgroundColor, 0.14);
	}

	static void drawVerticalDivider(HDC hdc, int x, int top, int bottom, HPEN pen) {
		MoveToEx(hdc, x, top, nullptr);
		LineTo(hdc, x, bottom);
	}

	static void drawHorizontalDivider(HDC hdc, int left, int right, int y, HPEN pen) {
		MoveToEx(hdc, left, y, nullptr);
		LineTo(hdc, right, y);
	}

	static void overlayListViewDividers(HWND hWnd) {
		if (!isEnabled() || !isReportListView(hWnd)) {
			return;
		}

		RECT clientRect = {};
		GetClientRect(hWnd, &clientRect);

		auto hHeader = ListView_GetHeader(hWnd);
		RECT headerRect = {};
		if (hHeader && GetWindowRect(hHeader, &headerRect)) {
			MapWindowPoints(nullptr, hWnd, reinterpret_cast<LPPOINT>(&headerRect), 2);
		}

		const int contentTop = hHeader ? headerRect.bottom : clientRect.top;
		if (contentTop >= clientRect.bottom) {
			return;
		}

		const auto listViewBackground = settings.color_theme.packed_listview_bg;
		const auto dividerColor = getDividerColor(listViewBackground);
		const auto hdc = GetDC(hWnd);
		if (!hdc) {
			return;
		}

		const auto pen = CreatePen(PS_SOLID, 1, dividerColor);
		auto oldPen = reinterpret_cast<HPEN>(SelectObject(hdc, pen));

		const int horizontalScroll = GetScrollPos(hWnd, SB_HORZ);
		const int columnCount = hHeader ? Header_GetItemCount(hHeader) : 0;
		int columnX = -horizontalScroll;
		for (int columnIndex = 0; columnIndex < columnCount - 1; ++columnIndex) {
			columnX += ListView_GetColumnWidth(hWnd, columnIndex);
			if (columnX > clientRect.left && columnX < clientRect.right) {
				drawVerticalDivider(hdc, columnX - 1, contentTop, clientRect.bottom, pen);
			}
		}

		const auto extendedStyle = ListView_GetExtendedListViewStyle(hWnd);
		if ((extendedStyle & LVS_EX_GRIDLINES) != 0) {
			const int itemCount = ListView_GetItemCount(hWnd);
			const int firstIndex = ListView_GetTopIndex(hWnd);
			const int visibleCount = ListView_GetCountPerPage(hWnd) + 1;
			const int lastIndex = std::min(itemCount, firstIndex + visibleCount);

			for (int itemIndex = firstIndex; itemIndex < lastIndex; ++itemIndex) {
				RECT itemRect = {};
				if (!ListView_GetItemRect(hWnd, itemIndex, &itemRect, LVIR_BOUNDS)) {
					continue;
				}

				const int dividerY = itemRect.bottom - 1;
				if (dividerY >= contentTop && dividerY < clientRect.bottom) {
					drawHorizontalDivider(hdc, clientRect.left, clientRect.right, dividerY, pen);
				}
			}
		}

		SelectObject(hdc, oldPen);
		DeleteObject(pen);
		ReleaseDC(hWnd, hdc);
	}

	static void drawThemedTabItem(HDC hdc, HWND hTab, int itemIndex, RECT rect, bool selected, bool focused) {
		TCITEMA item = {};
		char buffer[128] = {};
		item.mask = TCIF_TEXT;
		item.pszText = buffer;
		item.cchTextMax = sizeof(buffer);
		TabCtrl_GetItem(hTab, itemIndex, &item);

		COLORREF backgroundColor = selected
			? (isEnabled() ? settings.color_theme.packed_tab_active_bg : GetSysColor(COLOR_WINDOW))
			: (isEnabled() ? settings.color_theme.packed_tab_inactive_bg : GetSysColor(COLOR_BTNFACE));
		COLORREF textColor = isEnabled() ? settings.color_theme.packed_text_color : GetSysColor(COLOR_BTNTEXT);

		if (selected) {
			rect.bottom -= 1;
		}

		FillRect(hdc, &rect, getCachedBrush(backgroundColor));
		FrameRect(hdc, &rect, getCachedBrush(darkenColor(backgroundColor, 0.20)));

		rect.left += 8;
		rect.right -= 8;
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, textColor);
		DrawTextA(hdc, buffer, -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

		if (focused) {
			RECT focusRect = rect;
			InflateRect(&focusRect, -2, -2);
			DrawFocusRect(hdc, &focusRect);
		}
	}

	static bool paintTabControl(HWND hWnd) {
		if (!isEnabled() || !isWindowClass(hWnd, WC_TABCONTROL)) {
			return false;
		}

		PAINTSTRUCT paintStruct = {};
		HDC hdc = BeginPaint(hWnd, &paintStruct);
		if (!hdc) {
			return true;
		}

		RECT clientRect = {};
		GetClientRect(hWnd, &clientRect);
		FillRect(hdc, &clientRect, getCachedBrush(settings.color_theme.packed_tab_inactive_bg));

		RECT displayRect = clientRect;
		if (TabCtrl_AdjustRect(hWnd, FALSE, &displayRect)) {
			FrameRect(hdc, &displayRect, getCachedBrush(darkenColor(settings.color_theme.packed_tab_inactive_bg, 0.22)));
		}

		const int selectedIndex = TabCtrl_GetCurSel(hWnd);
		const int itemCount = TabCtrl_GetItemCount(hWnd);
		for (int itemIndex = 0; itemIndex < itemCount; ++itemIndex) {
			if (itemIndex == selectedIndex) {
				continue;
			}

			RECT itemRect = {};
			if (TabCtrl_GetItemRect(hWnd, itemIndex, &itemRect)) {
				drawThemedTabItem(hdc, hWnd, itemIndex, itemRect, false, false);
			}
		}

		if (selectedIndex >= 0) {
			RECT selectedRect = {};
			if (TabCtrl_GetItemRect(hWnd, selectedIndex, &selectedRect)) {
				const bool focused = GetFocus() == hWnd;
				drawThemedTabItem(hdc, hWnd, selectedIndex, selectedRect, true, focused);
			}
		}

		EndPaint(hWnd, &paintStruct);
		return true;
	}

	static void themeControl(HWND hWnd) {
		ensureUxThemeFunction();

		// Prevent reentrancy to avoid infinite recursion when messages like
		// TB_SETEXTENDEDSTYLE trigger WM_THEMECHANGED during theming.
		if (g_IsTheming) {
			return;
		}

		g_IsTheming = true;

		if (isWindowClass(hWnd, WC_LISTVIEW)) {
			if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hWnd, isEnabled() ? L"Explorer" : L"", nullptr);
			}

			const auto& ct = settings.color_theme;
			ListView_SetBkColor(hWnd, ct.packed_listview_bg);
			ListView_SetTextColor(hWnd, ct.packed_listview_text);
			ListView_SetTextBkColor(hWnd, ct.packed_listview_bg);

			const auto hHeader = ListView_GetHeader(hWnd);
			if (hHeader && g_pSetWindowTheme) {
				g_pSetWindowTheme(hHeader, isEnabled() ? L"Explorer" : L"", nullptr);
			}
		}
		else if (isWindowClass(hWnd, WC_TREEVIEW)) {
			if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hWnd, isEnabled() ? L"Explorer" : L"", nullptr);
			}

			const auto& ct = settings.color_theme;
			TreeView_SetBkColor(hWnd, ct.packed_treeview_bg);
			TreeView_SetTextColor(hWnd, ct.packed_treeview_text);
		}
		else if (isWindowClass(hWnd, WC_TABCONTROL)) {
			if (isEnabled()) {
				themeTabControlWindow(hWnd);
			}
			else if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hWnd, L"Explorer", nullptr);
			}
		}
		else if (isWindowClass(hWnd, WC_COMBOBOX)) {
			if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hWnd, isEnabled() ? L"Explorer" : L"", nullptr);
			}
		}
		else if (isWindowClass(hWnd, WC_BUTTON)) {
			if (isEnabled() && isPushButtonStyle(GetWindowLongA(hWnd, GWL_STYLE))) {
				SetWindowLongA(hWnd, GWL_STYLE, GetWindowLongA(hWnd, GWL_STYLE) | BS_OWNERDRAW);
			}
		}
		else if (isWindowClass(hWnd, REBARCLASSNAMEA)) {
			if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hWnd, isEnabled() ? L"Explorer" : L"", nullptr);
			}
		}
		else if (isWindowClass(hWnd, TOOLBARCLASSNAMEA)) {
			if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hWnd, isEnabled() ? L"Explorer" : L"", nullptr);
			}
			themeToolbarWindow(hWnd);
		}
		else if (isWindowClass(hWnd, STATUSCLASSNAMEA)) {
			if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hWnd, isEnabled() ? L"Explorer" : L"", nullptr);
			}
			themeStatusBarWindow(hWnd);
		}
		else if (isRichEditControl(hWnd)) {
			if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hWnd, isEnabled() ? L"Explorer" : L"", nullptr);
			}
			themeRichEditControl(hWnd);
		}

		if (isEnabled()) {
			InvalidateRect(hWnd, nullptr, TRUE);
		}

		g_IsTheming = false;
	}

	//
	// Child control theming — called on WM_INITDIALOG to theme list/tree views.
	//

	static BOOL CALLBACK themeChildProc(HWND hwndChild, LPARAM lParam) {
		themeControl(hwndChild);
		return TRUE;
	}

	static void themeChildControls(HWND hWnd) {
		themeControl(hWnd);
		EnumChildWindows(hWnd, themeChildProc, 0);

		if (isTopLevelWindow(hWnd)) {
			themeMenuRecursive(GetMenu(hWnd), true);
			DrawMenuBar(hWnd);
		}
	}

	static BOOL CALLBACK refreshThreadWindowProc(HWND hWnd, LPARAM) {
		refreshWindow(hWnd);
		return TRUE;
	}

	static std::optional<std::filesystem::file_time_type> getThemeFileWriteTime() {
		const auto settingsPath = settings.file_location();
		if (!std::filesystem::exists(settingsPath)) {
			return {};
		}

		return std::filesystem::last_write_time(settingsPath);
	}

	static void pollForExternalThemeChanges() {
		if (g_IsRefreshing) {
			return;
		}

		const auto currentWriteTime = getThemeFileWriteTime();
		if (!currentWriteTime || !g_LastThemeWriteTime || *currentWriteTime == *g_LastThemeWriteTime) {
			return;
		}

		log::stream << "[Theme] pollForExternalThemeChanges: file changed, reloading. "
			<< "enabled_before=" << settings.color_theme.enabled
			<< " preset_before=" << settings.color_theme.preset << std::endl;

		try {
			settings.load();
			g_LastThemeWriteTime = currentWriteTime;
			log::stream << "[Theme] reloaded. enabled_after=" << settings.color_theme.enabled
				<< " preset_after=" << settings.color_theme.preset << std::endl;
			refresh();
			log::stream << "Theme engine: reloaded theme settings from csse.toml." << std::endl;
		}
		catch (const std::exception& e) {
			log::stream << "Theme engine: failed to hot-reload theme settings: " << e.what() << std::endl;
		}
	}

	//
	// WM_CTLCOLOR* handling — sets text/background colors on control DCs.
	//

	static LRESULT handleCtlColor(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		HDC hdc = (HDC)wParam;
		HWND hControl = (HWND)lParam;
		auto& ct = settings.color_theme;

		COLORREF textColor = ct.packed_text_color;
		COLORREF bgColor = ct.packed_control_bg;

		switch (msg) {
		case WM_CTLCOLORDLG:
			bgColor = ct.packed_window_bg;
			break;
		case WM_CTLCOLORSTATIC:
			textColor = IsWindowEnabled(hControl) ? ct.packed_text_color : ct.packed_text_disabled;
			bgColor = isWindowClass(hControl, WC_COMBOBOX) ? ct.packed_edit_bg : ct.packed_window_bg;
			break;
		case WM_CTLCOLOREDIT:
			textColor = ct.packed_edit_text;
			bgColor = ct.packed_edit_bg;
			break;
		case WM_CTLCOLORLISTBOX:
			textColor = ct.packed_listview_text;
			bgColor = ct.packed_listview_bg;
			break;
		case WM_CTLCOLORBTN:
			textColor = ct.packed_button_text;
			bgColor = ct.packed_button_bg;
			break;
		default:
			break;
		}

		SetBkMode(hdc, OPAQUE);
		SetTextColor(hdc, textColor);
		SetBkColor(hdc, bgColor);
		return (LRESULT)getCachedBrush(bgColor);
	}

	//
	// Render window detection — avoid painting over the D3D viewport.
	//

	static bool isRenderViewport(HWND hWnd) {
		// The render viewport is a child window of the render window dialog.
		// We identify it by checking if it has the "D3DWndClass" or similar class,
		// or more robustly, by checking if it's a known render class.
		char className[64] = {};
		GetClassNameA(hWnd, className, sizeof(className));

		// The vanilla CS uses "D3DWindow" or the default D3D class.
		if (_stricmp(className, "D3DWindow") == 0) return true;
		if (_stricmp(className, "AfxFrameOrView") == 0) return true;

		return false;
	}

	static bool isStatusBarCustomDraw(const NMHDR* header) {
		return header != nullptr && isWindowClass(header->hwndFrom, STATUSCLASSNAMEA);
	}

	static bool isToolbarCustomDraw(const NMHDR* header) {
		return header != nullptr && isWindowClass(header->hwndFrom, TOOLBARCLASSNAMEA);
	}

	static bool isHeaderCustomDraw(const NMHDR* header) {
		return header != nullptr && isWindowClass(header->hwndFrom, WC_HEADERA);
	}

	static LRESULT handleHeaderCustomDraw(LPNMCUSTOMDRAW customDraw) {
		if (customDraw->dwDrawStage == CDDS_PREPAINT) {
			return CDRF_NOTIFYITEMDRAW;
		}

		if (customDraw->dwDrawStage != CDDS_ITEMPREPAINT) {
			return CDRF_DODEFAULT;
		}

		const auto hHeader = customDraw->hdr.hwndFrom;
		char buffer[256] = {};
		HDITEMA headerItem = {};
		headerItem.mask = HDI_TEXT | HDI_FORMAT;
		headerItem.pszText = buffer;
		headerItem.cchTextMax = sizeof(buffer);
		Header_GetItem(hHeader, static_cast<int>(customDraw->dwItemSpec), &headerItem);

		auto rect = customDraw->rc;
		const auto fillColor = isEnabled() ? settings.color_theme.packed_listview_header_bg : GetSysColor(COLOR_BTNFACE);
		const auto textColor = isEnabled() ? settings.color_theme.packed_listview_text : GetSysColor(COLOR_BTNTEXT);

		FillRect(customDraw->hdc, &rect, getCachedBrush(fillColor));
		FrameRect(customDraw->hdc, &rect, getCachedBrush(settings.color_theme.packed_border_color));

		SelectObject(customDraw->hdc, GetStockObject(DEFAULT_GUI_FONT));
		SetBkMode(customDraw->hdc, TRANSPARENT);
		SetTextColor(customDraw->hdc, textColor);

		rect.left += 8;
		rect.right -= 8;

		UINT drawFormat = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
		if ((headerItem.fmt & HDF_CENTER) != 0) {
			drawFormat |= DT_CENTER;
		}
		else if ((headerItem.fmt & HDF_RIGHT) != 0) {
			drawFormat |= DT_RIGHT;
		}
		else {
			drawFormat |= DT_LEFT;
		}

		DrawTextA(customDraw->hdc, buffer, -1, &rect, drawFormat);
		return CDRF_SKIPDEFAULT;
	}

	static LRESULT handleToolbarCustomDraw(LPNMTBCUSTOMDRAW toolbarDraw) {
		if (!isEnabled()) {
			return CDRF_DODEFAULT;
		}

		const auto& ct = settings.color_theme;
		toolbarDraw->clrText = ct.packed_button_text;
		toolbarDraw->clrTextHighlight = ct.packed_selection_text;
		toolbarDraw->clrBtnFace = ct.packed_toolbar_bg;
		toolbarDraw->clrBtnHighlight = lightenColor(ct.packed_toolbar_bg, 0.10);
		toolbarDraw->clrHighlightHotTrack = ct.packed_selection_bg;
		toolbarDraw->clrMark = ct.packed_selection_bg;

		switch (toolbarDraw->nmcd.dwDrawStage) {
		case CDDS_PREPAINT:
			return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		case CDDS_ITEMPREPAINT:
			return TBCDRF_NOBACKGROUND | TBCDRF_NOEDGES | TBCDRF_HILITEHOTTRACK | TBCDRF_USECDCOLORS;
		default:
			return CDRF_DODEFAULT;
		}
	}

	static LRESULT handleStatusBarCustomDraw(LPNMCUSTOMDRAW customDraw) {
		if (customDraw->dwDrawStage == CDDS_PREPAINT) {
			return CDRF_NOTIFYITEMDRAW;
		}

		if (customDraw->dwDrawStage != CDDS_ITEMPREPAINT) {
			return CDRF_DODEFAULT;
		}

		const auto hStatusBar = customDraw->hdr.hwndFrom;
		const auto paneIndex = static_cast<int>(customDraw->dwItemSpec);
		auto rect = customDraw->rc;
		const auto backgroundColor = isEnabled() ? settings.color_theme.packed_statusbar_bg : GetSysColor(COLOR_BTNFACE);
		const auto textColor = isEnabled() ? settings.color_theme.packed_statusbar_text : GetSysColor(COLOR_BTNTEXT);

		FillRect(customDraw->hdc, &rect, getCachedBrush(backgroundColor));
		FrameRect(customDraw->hdc, &rect, getCachedBrush(settings.color_theme.packed_border_color));

		char buffer[256] = {};
		const auto textLengthAndFlags = SendMessageA(hStatusBar, SB_GETTEXTA, paneIndex, reinterpret_cast<LPARAM>(buffer));
		const auto textLength = LOWORD(textLengthAndFlags);

		SelectObject(customDraw->hdc, GetStockObject(DEFAULT_GUI_FONT));
		SetBkMode(customDraw->hdc, TRANSPARENT);
		SetTextColor(customDraw->hdc, textColor);

		rect.left += 6;
		rect.right -= 6;
		DrawTextA(customDraw->hdc, buffer, textLength, &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
		return CDRF_SKIPDEFAULT;
	}

	static bool measureMenuItem(MEASUREITEMSTRUCT* measureItem) {
		if (measureItem->CtlType != ODT_MENU) {
			return false;
		}

		auto* themeData = reinterpret_cast<MenuItemThemeData*>(measureItem->itemData);
		if (!themeData) {
			return false;
		}

		ensureMenuFont();

		HDC screenDC = GetDC(nullptr);
		HFONT oldFont = (HFONT)SelectObject(screenDC, g_hMenuFont);

		SIZE textSize = {};
		GetTextExtentPoint32A(screenDC, themeData->text.c_str(), (int)themeData->text.length(), &textSize);

		SelectObject(screenDC, oldFont);
		ReleaseDC(nullptr, screenDC);

		measureItem->itemHeight = themeData->topLevel ? 24 : 22;
		measureItem->itemWidth = themeData->topLevel ? textSize.cx + 24 : textSize.cx + 56;
		return true;
	}

	static bool drawMenuItem(const DRAWITEMSTRUCT* drawItem) {
		if (drawItem->CtlType != ODT_MENU) {
			return false;
		}

		auto* themeData = reinterpret_cast<MenuItemThemeData*>(drawItem->itemData);
		if (!themeData) {
			return false;
		}

		ensureMenuFont();

		const bool disabled = (drawItem->itemState & ODS_DISABLED) != 0;
		const bool selected = (drawItem->itemState & ODS_SELECTED) != 0;
		const bool checked = (drawItem->itemState & ODS_CHECKED) != 0;

		COLORREF backgroundColor = isEnabled() ? settings.color_theme.packed_menu_bg : GetSysColor(COLOR_MENU);
		COLORREF textColor = isEnabled() ? settings.color_theme.packed_menu_text : GetSysColor(COLOR_MENUTEXT);

		if (disabled) {
			textColor = isEnabled() ? settings.color_theme.packed_text_disabled : GetSysColor(COLOR_GRAYTEXT);
		}
		else if (selected) {
			backgroundColor = isEnabled() ? settings.color_theme.packed_selection_bg : GetSysColor(COLOR_HIGHLIGHT);
			textColor = isEnabled() ? settings.color_theme.packed_selection_text : GetSysColor(COLOR_HIGHLIGHTTEXT);
		}

		auto rect = drawItem->rcItem;
		FillRect(drawItem->hDC, &rect, getCachedBrush(backgroundColor));

		if (checked && !themeData->topLevel) {
			RECT checkRect = rect;
			checkRect.right = checkRect.left + 20;
			FillRect(drawItem->hDC, &checkRect, getCachedBrush(lightenColor(backgroundColor, isEnabled() ? 0.08 : 0.12)));
			DrawFrameControl(drawItem->hDC, &checkRect, DFC_MENU, DFCS_MENUCHECK);
			rect.left = checkRect.right + 4;
		}
		else if (!themeData->topLevel) {
			rect.left += 24;
		}

		HFONT oldFont = (HFONT)SelectObject(drawItem->hDC, g_hMenuFont);
		SetBkMode(drawItem->hDC, TRANSPARENT);
		SetTextColor(drawItem->hDC, textColor);

		if (themeData->topLevel) {
			DrawTextA(drawItem->hDC, themeData->text.c_str(), -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
		else {
			auto acceleratorSeparator = themeData->text.find('\t');
			std::string label = themeData->text.substr(0, acceleratorSeparator);
			std::string shortcut = acceleratorSeparator == std::string::npos ? "" : themeData->text.substr(acceleratorSeparator + 1);

			DrawTextA(drawItem->hDC, label.c_str(), -1, &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
			if (!shortcut.empty()) {
				DrawTextA(drawItem->hDC, shortcut.c_str(), -1, &rect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
			}
		}

		SelectObject(drawItem->hDC, oldFont);
		return true;
	}

	static bool drawOwnerDrawButton(const DRAWITEMSTRUCT* drawItem) {
		if (drawItem->CtlType != ODT_BUTTON) {
			return false;
		}

		auto hButton = drawItem->hwndItem;
		const auto style = GetWindowLongA(hButton, GWL_STYLE);
		if (!isPushButtonStyle(style)) {
			return false;
		}

		const bool disabled = (drawItem->itemState & ODS_DISABLED) != 0;
		const bool pressed = (drawItem->itemState & ODS_SELECTED) != 0 || SendMessageA(hButton, BM_GETCHECK, 0, 0) == BST_CHECKED;
		const bool focused = (drawItem->itemState & ODS_FOCUS) != 0;

		COLORREF backgroundColor = isEnabled() ? settings.color_theme.packed_button_bg : GetSysColor(COLOR_BTNFACE);
		COLORREF textColor = isEnabled() ? settings.color_theme.packed_button_text : GetSysColor(COLOR_BTNTEXT);

		if (disabled) {
			textColor = isEnabled() ? settings.color_theme.packed_text_disabled : GetSysColor(COLOR_GRAYTEXT);
		}
		else if (pressed) {
			backgroundColor = isEnabled() ? settings.color_theme.packed_selection_bg : GetSysColor(COLOR_HIGHLIGHT);
			textColor = isEnabled() ? settings.color_theme.packed_selection_text : GetSysColor(COLOR_HIGHLIGHTTEXT);
		}

		auto rect = drawItem->rcItem;
		FillRect(drawItem->hDC, &rect, getCachedBrush(backgroundColor));
		FrameRect(drawItem->hDC, &rect, getCachedBrush(settings.color_theme.packed_border_color));

		auto image = SendMessageA(hButton, BM_GETIMAGE, IMAGE_ICON, 0);
		if (image != 0) {
			const auto hIcon = reinterpret_cast<HICON>(image);
			const auto iconX = rect.left + (rect.right - rect.left - 16) / 2;
			const auto iconY = rect.top + (rect.bottom - rect.top - 16) / 2;
			DrawIconEx(drawItem->hDC, iconX, iconY, hIcon, 16, 16, 0, nullptr, DI_NORMAL);
		}
		else {
			char buffer[256] = {};
			GetWindowTextA(hButton, buffer, sizeof(buffer));
			SetBkMode(drawItem->hDC, TRANSPARENT);
			SetTextColor(drawItem->hDC, textColor);
			DrawTextA(drawItem->hDC, buffer, -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
		}

		if (focused) {
			auto focusRect = rect;
			InflateRect(&focusRect, -3, -3);
			DrawFocusRect(drawItem->hDC, &focusRect);
		}

		return true;
	}

	static bool drawOwnerDrawTab(const DRAWITEMSTRUCT* drawItem) {
		if (drawItem->CtlType != ODT_TAB) {
			return false;
		}

		auto hTab = drawItem->hwndItem;
		auto itemIndex = static_cast<int>(drawItem->itemID);
		const bool selected = TabCtrl_GetCurSel(hTab) == itemIndex;
		drawThemedTabItem(drawItem->hDC, hTab, itemIndex, drawItem->rcItem, selected, (drawItem->itemState & ODS_FOCUS) != 0);
		return true;
	}

	static HBITMAP createThemedBitmapVariant(HINSTANCE instance, UINT bitmapResourceId) {
		auto hBitmap = (HBITMAP)LoadImageA(instance, MAKEINTRESOURCEA(bitmapResourceId), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		if (!hBitmap || !isEnabled()) {
			return hBitmap;
		}

		const auto darkBackground = settings.color_theme.packed_toolbar_bg;
		if ((GetRValue(darkBackground) + GetGValue(darkBackground) + GetBValue(darkBackground)) > (128 * 3)) {
			return hBitmap;
		}

		BITMAP bitmapInfo = {};
		if (GetObjectA(hBitmap, sizeof(bitmapInfo), &bitmapInfo) == 0 || bitmapInfo.bmWidth <= 0 || bitmapInfo.bmHeight <= 0) {
			return hBitmap;
		}

		BITMAPINFO dibInfo = {};
		dibInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		dibInfo.bmiHeader.biWidth = bitmapInfo.bmWidth;
		dibInfo.bmiHeader.biHeight = -bitmapInfo.bmHeight;
		dibInfo.bmiHeader.biPlanes = 1;
		dibInfo.bmiHeader.biBitCount = 32;
		dibInfo.bmiHeader.biCompression = BI_RGB;

		std::vector<unsigned int> pixels(bitmapInfo.bmWidth * bitmapInfo.bmHeight);
		auto screenDC = GetDC(nullptr);
		if (!GetDIBits(screenDC, hBitmap, 0, bitmapInfo.bmHeight, pixels.data(), &dibInfo, DIB_RGB_COLORS)) {
			ReleaseDC(nullptr, screenDC);
			return hBitmap;
		}

		const auto transparentKey = pixels.front() & 0x00FFFFFF;
		const auto iconColor = settings.color_theme.packed_button_text;

		for (auto& pixel : pixels) {
			const auto rgb = pixel & 0x00FFFFFF;
			if (rgb == transparentKey) {
				continue;
			}

			const auto red = GetRValue(rgb);
			const auto green = GetGValue(rgb);
			const auto blue = GetBValue(rgb);
			const auto luminance = (0.299 * red + 0.587 * green + 0.114 * blue) / 255.0;
			const auto blended = blendColor(rgb, iconColor, luminance < 0.45 ? 0.80 : 0.45);
			pixel = (pixel & 0xFF000000) | (blended & 0x00FFFFFF);
		}

		SetDIBits(screenDC, hBitmap, 0, bitmapInfo.bmHeight, pixels.data(), &dibInfo, DIB_RGB_COLORS);
		ReleaseDC(nullptr, screenDC);
		return hBitmap;
	}

	static void destroyToolbarImageList(HWND hWnd) {
		auto hImageList = reinterpret_cast<HIMAGELIST>(RemovePropA(hWnd, TOOLBAR_IMAGE_LIST_PROP));
		if (hImageList) {
			ImageList_Destroy(hImageList);
		}
	}

	//
	// ThemeSubclassProc — the heart of the theming system.
	// Installed on every window created in the process via CBT hook.
	//

	static LRESULT CALLBACK ThemeSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
		if (isTopLevelWindow(hWnd) && (msg == WM_ACTIVATEAPP || msg == WM_NCACTIVATE)) {
			pollForExternalThemeChanges();
		}

		switch (msg) {
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORBTN:
			if (isEnabled()) {
				return handleCtlColor(hWnd, msg, wParam, lParam);
			}
			break;

		case WM_ERASEBKGND:
		{
			if (!isEnabled()) {
				break;
			}

			if (eraseTabControlBackground(hWnd, (HDC)wParam)) {
				return 1;
			}

			// Skip the render viewport to avoid painting over D3D.
			if (isRenderViewport(hWnd)) {
				break;
			}

			HDC hdc = (HDC)wParam;
			RECT rc;
			GetClientRect(hWnd, &rc);
			FillRect(hdc, &rc, getCachedBrush(settings.color_theme.packed_window_bg));
			return 1; // We handled it.
		}

		case WM_PAINT:
			if (isEnabled() && isReportListView(hWnd)) {
				auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
				overlayListViewDividers(hWnd);
				return result;
			}

			if (paintTabControl(hWnd)) {
				return 0;
			}
			break;

		case WM_NOTIFY:
		{
			auto* header = reinterpret_cast<NMHDR*>(lParam);
			if (header && header->code == NM_CUSTOMDRAW) {
				if (isHeaderCustomDraw(header)) {
					return handleHeaderCustomDraw(reinterpret_cast<LPNMCUSTOMDRAW>(lParam));
				}
				if (isToolbarCustomDraw(header)) {
					return handleToolbarCustomDraw(reinterpret_cast<LPNMTBCUSTOMDRAW>(lParam));
				}
				if (isStatusBarCustomDraw(header)) {
					return handleStatusBarCustomDraw(reinterpret_cast<LPNMCUSTOMDRAW>(lParam));
				}
			}
			break;
		}

		case WM_MEASUREITEM:
			if (measureMenuItem(reinterpret_cast<MEASUREITEMSTRUCT*>(lParam))) {
				return TRUE;
			}
			break;

		case WM_DRAWITEM:
		{
			const auto* drawItem = reinterpret_cast<const DRAWITEMSTRUCT*>(lParam);
			if (drawMenuItem(drawItem) || drawOwnerDrawButton(drawItem) || drawOwnerDrawTab(drawItem)) {
				return TRUE;
			}
			break;
		}

		case WM_CREATE:
			if (isEnabled()) {
				themeControl(hWnd);
			}
			break;

		case WM_INITDIALOG:
		{
			// Let the original handler run first.
			LRESULT result = DefSubclassProc(hWnd, msg, wParam, lParam);

			// Apply dark title bar and theme child controls.
			if (settings.color_theme.use_dark_title_bar) {
				applyDarkTitleBar(hWnd);
			}
			themeChildControls(hWnd);

			return result;
		}

		case WM_NCPAINT:
		{
			// Let the default handler run first so scrollbars and other NC chrome
			// are painted correctly, then overdraw the sunken 3D border (WS_EX_CLIENTEDGE)
			// with our flat themed border color.
			//
			// Only apply to input/display controls with a genuine sunken border.
			// Explicitly skip buttons, tab controls, toolbars, etc., which either
			// have no WS_EX_CLIENTEDGE or draw their own border via owner-draw.
			LRESULT result = DefSubclassProc(hWnd, msg, wParam, lParam);

			if (isEnabled() && (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_CLIENTEDGE)) {
				const bool isInputControl =
					isWindowClass(hWnd, WC_EDIT) ||
					isWindowClass(hWnd, WC_LISTVIEW) ||
					isWindowClass(hWnd, WC_TREEVIEW) ||
					isWindowClass(hWnd, WC_COMBOBOX) ||
					isRichEditControl(hWnd);

				if (isInputControl) {
					HDC hdc = GetWindowDC(hWnd);
					if (hdc) {
						RECT rc;
						GetWindowRect(hWnd, &rc);
						OffsetRect(&rc, -rc.left, -rc.top);

						// WS_EX_CLIENTEDGE is 2 pixels wide; fill both with our border color.
						const HBRUSH hbr = getCachedBrush(settings.color_theme.packed_border_color);
						FrameRect(hdc, &rc, hbr);
						InflateRect(&rc, -1, -1);
						FrameRect(hdc, &rc, hbr);

						ReleaseDC(hWnd, hdc);
					}
				}
			}
			return result;
		}

		case WM_THEMECHANGED:
		case WM_SETTINGCHANGE:
			if (isEnabled()) {
				themeChildControls(hWnd);
			}
			break;

		case WM_NCDESTROY:
			destroyToolbarImageList(hWnd);
			RemoveWindowSubclass(hWnd, ThemeSubclassProc, uIdSubclass);
			break;
		}

		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	//
	// CBT hook — catches window creation process-wide (thread-local, zero overhead).
	//

	static HHOOK g_hCBTHook = nullptr;

	static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam) {
		if (nCode == HCBT_CREATEWND) {
			HWND hWnd = (HWND)wParam;

			// Subclass the window for theming.
			SetWindowSubclass(hWnd, ThemeSubclassProc, 1, 0);

			applyAllowDarkMode(hWnd);
			themeControl(hWnd);

			// Apply dark title bar to top-level windows immediately.
			if (isEnabled() && settings.color_theme.use_dark_title_bar) {
				applyDarkTitleBar(hWnd);
			}
		}

		return CallNextHookEx(g_hCBTHook, nCode, wParam, lParam);
	}

	//
	// Public API
	//

	bool isEnabled() {
		return settings.color_theme.enabled;
	}

	void refreshWindow(HWND hWnd) {
		if (!IsWindow(hWnd)) {
			return;
		}

		g_IsRefreshing = true;
		clearBrushCache();
		applyDarkTitleBar(hWnd);
		if (isEnabled()) {
			themeChildControls(hWnd);
			themeRichEditControl(hWnd);
		}
		invalidateWindowTree(hWnd);
		g_LastThemeWriteTime = getThemeFileWriteTime();
		g_IsRefreshing = false;
	}

	void refresh() {
		log::stream << "[Theme] refresh() called. enabled=" << settings.color_theme.enabled
			<< " preset=" << settings.color_theme.preset << std::endl;
		g_IsRefreshing = true;
		updatePreferredAppMode();
		clearBrushCache();
		EnumThreadWindows(GetCurrentThreadId(), refreshThreadWindowProc, 0);
		g_LastThemeWriteTime = getThemeFileWriteTime();
		g_IsRefreshing = false;
		log::stream << "[Theme] refresh() done." << std::endl;
	}

	HBITMAP loadThemedBitmap(HINSTANCE instance, UINT bitmapResourceId) {
		return createThemedBitmapVariant(instance, bitmapResourceId);
	}

	void applyToolbarBitmap(HWND hWndToolbar, HINSTANCE instance, UINT bitmapResourceId, int bitmapWidth, int bitmapHeight, int bitmapCount) {
		destroyToolbarImageList(hWndToolbar);

		auto hBitmap = loadThemedBitmap(instance, bitmapResourceId);
		if (!hBitmap) {
			return;
		}

		auto imageList = ImageList_Create(bitmapWidth, bitmapHeight, ILC_COLOR24 | ILC_MASK, bitmapCount, 0);
		if (!imageList) {
			DeleteObject(hBitmap);
			return;
		}

		ImageList_AddMasked(imageList, hBitmap, CLR_DEFAULT);
		DeleteObject(hBitmap);

		SendMessageA(hWndToolbar, TB_SETIMAGELIST, 0, reinterpret_cast<LPARAM>(imageList));
		SetPropA(hWndToolbar, TOOLBAR_IMAGE_LIST_PROP, imageList);
		InvalidateRect(hWndToolbar, nullptr, TRUE);
	}

	void install() {
		if (g_hCBTHook) {
			return;
		}

		// Patch comctl32's scrollbar theming before any windows are created.
		if (isEnabled()) {
			fixDarkScrollBar();
		}
		updatePreferredAppMode();

		g_hCBTHook = SetWindowsHookExA(WH_CBT, CBTProc, nullptr, GetCurrentThreadId());
		g_LastThemeWriteTime = getThemeFileWriteTime();
		if (g_hCBTHook) {
			log::stream << "Theme engine: installed CBT hook." << std::endl;
		}
		else {
			log::stream << "Theme engine: FAILED to install CBT hook. Error: " << GetLastError() << std::endl;
		}
	}

	void shutdown() {
		if (g_hCBTHook) {
			UnhookWindowsHookEx(g_hCBTHook);
			g_hCBTHook = nullptr;
		}
		updatePreferredAppMode();
		clearBrushCache();
		clearMenuThemeData();
	}
}
