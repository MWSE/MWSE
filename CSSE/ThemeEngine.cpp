#include "ThemeEngine.h"

#include "LogUtil.h"
#include "Settings.h"

#include <dwmapi.h>
#include <uxtheme.h>

namespace se::cs::theme {
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

		// Only apply to top-level windows (not child controls).
		LONG style = GetWindowLongA(hWnd, GWL_STYLE);
		if ((style & WS_CHILD) != 0) return;

		// Attribute 20 = DWMWA_USE_IMMERSIVE_DARK_MODE.
		BOOL value = TRUE;
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
	// Helper: check common Win32 class names.
	//

	static bool isClassNameMatch(HWND hWnd, const char* target) {
		char className[64] = {};
		GetClassNameA(hWnd, className, sizeof(className));
		return _stricmp(className, target) == 0;
	}

	//
	// Child control theming — called on WM_INITDIALOG to theme list/tree views.
	//

	static BOOL CALLBACK themeChildProc(HWND hwndChild, LPARAM lParam) {
		char className[64] = {};
		GetClassNameA(hwndChild, className, sizeof(className));
		auto& ct = settings.color_theme;

		if (_stricmp(className, WC_LISTVIEW) == 0) {
			// Strip visual styles so we can control colors directly.
			if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hwndChild, L"", L"");
			}
			ListView_SetBkColor(hwndChild, ct.packed_listview_bg);
			ListView_SetTextColor(hwndChild, ct.packed_listview_text);
			ListView_SetTextBkColor(hwndChild, ct.packed_listview_bg);
		}
		else if (_stricmp(className, WC_TREEVIEW) == 0) {
			if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hwndChild, L"", L"");
			}
			TreeView_SetBkColor(hwndChild, ct.packed_treeview_bg);
			TreeView_SetTextColor(hwndChild, ct.packed_treeview_text);
		}
		else if (_stricmp(className, WC_TABCONTROL) == 0) {
			if (g_pSetWindowTheme) {
				g_pSetWindowTheme(hwndChild, L"", L"");
			}
		}

		return TRUE; // Continue enumeration.
	}

	static void themeChildControls(HWND hWnd) {
		ensureUxThemeFunction();
		EnumChildWindows(hWnd, themeChildProc, 0);
	}

	//
	// WM_CTLCOLOR* handling — sets text/background colors on control DCs.
	//

	static LRESULT handleCtlColor(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		HDC hdc = (HDC)wParam;
		auto& ct = settings.color_theme;

		COLORREF textColor = ct.packed_text_color;
		COLORREF bgColor = ct.packed_control_bg;

		switch (msg) {
		case WM_CTLCOLORDLG:
			bgColor = ct.packed_window_bg;
			break;
		case WM_CTLCOLORSTATIC:
			textColor = ct.packed_text_color;
			bgColor = ct.packed_window_bg;
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

	//
	// ThemeSubclassProc — the heart of the theming system.
	// Installed on every window created in the process via CBT hook.
	//

	static LRESULT CALLBACK ThemeSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
		// If theming was disabled at runtime, just pass through.
		if (!isEnabled()) {
			return DefSubclassProc(hWnd, msg, wParam, lParam);
		}

		switch (msg) {
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORBTN:
			return handleCtlColor(hWnd, msg, wParam, lParam);

		case WM_ERASEBKGND:
		{
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

		case WM_NCDESTROY:
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
		if (nCode == HCBT_CREATEWND && isEnabled()) {
			HWND hWnd = (HWND)wParam;

			// Subclass the window for theming.
			SetWindowSubclass(hWnd, ThemeSubclassProc, 1, 0);

			// Apply dark title bar to top-level windows immediately.
			if (settings.color_theme.use_dark_title_bar) {
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

	void install() {
		if (!isEnabled()) {
			log::stream << "Theme engine: disabled by settings." << std::endl;
			return;
		}

		g_hCBTHook = SetWindowsHookExA(WH_CBT, CBTProc, nullptr, GetCurrentThreadId());
		if (g_hCBTHook) {
			log::stream << "Theme engine: installed CBT hook (preset: " << settings.color_theme.preset << ")." << std::endl;
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
		clearBrushCache();
	}
}
