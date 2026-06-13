#include "DarkMode.h"

#include "CSSE.h"
#include "IconOverride.h"
#include "LogUtil.h"
#include "Settings.h"

#include <uxtheme.h>
#include <dwmapi.h>
#include <richedit.h>

#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "dwmapi.lib")

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

// Attribute value used by Windows 10 builds 17763..18984.
constexpr auto DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1 = 19;

namespace se::cs::darkmode {
	// Logs every window the creation hook classifies. Useful when diagnosing
	// controls that fail to pick up the theme.
	constexpr auto LOG_WINDOW_DISPATCH = false;

	static bool active = false;

	bool isActive() {
		return active;
	}

	//
	// GDI resources, created once on activation and kept for the process lifetime.
	//

	static HBRUSH backgroundBrush = nullptr;
	static HBRUSH workspaceBrush = nullptr;
	static HBRUSH surfaceBrush = nullptr;
	static HBRUSH controlBrush = nullptr;
	static HBRUSH controlHotBrush = nullptr;
	static HBRUSH borderBrush = nullptr;
	static HPEN borderPen = nullptr;

	static void createDrawingResources() {
		backgroundBrush = CreateSolidBrush(palette::background);
		workspaceBrush = CreateSolidBrush(palette::workspace);
		surfaceBrush = CreateSolidBrush(palette::surface);
		controlBrush = CreateSolidBrush(palette::control);
		controlHotBrush = CreateSolidBrush(palette::controlHot);
		borderBrush = CreateSolidBrush(palette::border);
		borderPen = CreatePen(PS_SOLID, 1, palette::border);
	}

	//
	// OS support checks. Dark mode requires the private uxtheme exports that
	// shipped with Windows 10 1809 (build 17763).
	//

	static DWORD getWindowsBuildNumber() {
		using RtlGetVersionFn = LONG(WINAPI*)(PRTL_OSVERSIONINFOW);
		const auto ntdll = GetModuleHandleW(L"ntdll.dll");
		const auto rtlGetVersion = reinterpret_cast<RtlGetVersionFn>(GetProcAddress(ntdll, "RtlGetVersion"));
		if (rtlGetVersion == nullptr) {
			return 0;
		}

		RTL_OSVERSIONINFOW version = { sizeof(version) };
		if (rtlGetVersion(&version) != 0) {
			return 0;
		}
		if (version.dwMajorVersion < 10) {
			return 0;
		}
		return version.dwBuildNumber;
	}

	static bool isRunningUnderWine() {
		return GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "wine_get_version") != nullptr;
	}

	//
	// Private uxtheme exports (by ordinal, Windows 10 1809+).
	//

	enum class PreferredAppMode : int {
		Default = 0,
		AllowDark = 1,
		ForceDark = 2,
	};

	using RefreshImmersiveColorPolicyStateFn = void(WINAPI*)();
	using AllowDarkModeForWindowFn = bool(WINAPI*)(HWND, BOOL);
	using AllowDarkModeForAppFn = bool(WINAPI*)(BOOL);
	using SetPreferredAppModeFn = PreferredAppMode(WINAPI*)(PreferredAppMode);
	using FlushMenuThemesFn = void(WINAPI*)();
	using OpenNcThemeDataFn = HTHEME(WINAPI*)(HWND, LPCWSTR);

	static RefreshImmersiveColorPolicyStateFn refreshImmersiveColorPolicyState = nullptr;
	static AllowDarkModeForWindowFn allowDarkModeForWindow = nullptr;
	static FlushMenuThemesFn flushMenuThemes = nullptr;
	static OpenNcThemeDataFn openNcThemeData = nullptr;

	static bool loadAndEnableDarkModeAPIs(DWORD buildNumber) {
		const auto uxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (uxtheme == nullptr) {
			return false;
		}

		refreshImmersiveColorPolicyState = reinterpret_cast<RefreshImmersiveColorPolicyStateFn>(GetProcAddress(uxtheme, MAKEINTRESOURCEA(104)));
		allowDarkModeForWindow = reinterpret_cast<AllowDarkModeForWindowFn>(GetProcAddress(uxtheme, MAKEINTRESOURCEA(133)));
		flushMenuThemes = reinterpret_cast<FlushMenuThemesFn>(GetProcAddress(uxtheme, MAKEINTRESOURCEA(136)));
		openNcThemeData = reinterpret_cast<OpenNcThemeDataFn>(GetProcAddress(uxtheme, MAKEINTRESOURCEA(49)));
		const auto ordinal135 = GetProcAddress(uxtheme, MAKEINTRESOURCEA(135));

		if (allowDarkModeForWindow == nullptr || openNcThemeData == nullptr || ordinal135 == nullptr) {
			return false;
		}

		// Ordinal 135 is AllowDarkModeForApp on 1809, SetPreferredAppMode on 1903+.
		if (buildNumber >= 18362) {
			reinterpret_cast<SetPreferredAppModeFn>(ordinal135)(PreferredAppMode::ForceDark);
		}
		else {
			reinterpret_cast<AllowDarkModeForAppFn>(ordinal135)(TRUE);
		}

		if (refreshImmersiveColorPolicyState) {
			refreshImmersiveColorPolicyState();
		}
		if (flushMenuThemes) {
			flushMenuThemes();
		}

		return true;
	}

	struct DelayImportDescriptor {
		DWORD attributes;
		DWORD dllName;
		DWORD moduleHandle;
		DWORD importAddressTable;
		DWORD importNameTable;
		DWORD boundImportAddressTable;
		DWORD unloadImportAddressTable;
		DWORD timeStamp;
	};

	static HTHEME WINAPI openNcThemeDataHook(HWND hWnd, LPCWSTR classList) {
		if (classList && wcscmp(classList, L"ScrollBar") == 0) {
			return openNcThemeData(nullptr, L"Explorer::ScrollBar");
		}
		return openNcThemeData(hWnd, classList);
	}

	static bool hookComctl32ScrollBarTheme() {
		const auto module = LoadLibraryExW(L"comctl32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (module == nullptr) {
			return false;
		}

		const auto base = reinterpret_cast<BYTE*>(module);
		const auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
		const auto ntHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dosHeader->e_lfanew);
		const auto& directory = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];
		if (directory.VirtualAddress == 0) {
			return false;
		}

		const auto descriptors = reinterpret_cast<DelayImportDescriptor*>(base + directory.VirtualAddress);
		for (auto descriptor = descriptors; descriptor->dllName != 0; ++descriptor) {
			const auto usesRvas = (descriptor->attributes & 1) != 0;
			const auto resolveAddress = [base, usesRvas](DWORD address) {
				return usesRvas ? base + address : reinterpret_cast<BYTE*>(address);
			};
			const auto dllName = reinterpret_cast<const char*>(resolveAddress(descriptor->dllName));
			if (_stricmp(dllName, "uxtheme.dll") != 0) {
				continue;
			}

			auto importAddress = reinterpret_cast<IMAGE_THUNK_DATA*>(resolveAddress(descriptor->importAddressTable));
			auto importName = reinterpret_cast<IMAGE_THUNK_DATA*>(resolveAddress(descriptor->importNameTable));
			for (; importName->u1.AddressOfData != 0; ++importAddress, ++importName) {
				if (!IMAGE_SNAP_BY_ORDINAL(importName->u1.Ordinal) || IMAGE_ORDINAL(importName->u1.Ordinal) != 49) {
					continue;
				}

				DWORD oldProtect = 0;
				if (!VirtualProtect(&importAddress->u1.Function, sizeof(importAddress->u1.Function), PAGE_READWRITE, &oldProtect)) {
					return false;
				}
				importAddress->u1.Function = reinterpret_cast<ULONG_PTR>(openNcThemeDataHook);
				VirtualProtect(&importAddress->u1.Function, sizeof(importAddress->u1.Function), oldProtect, &oldProtect);
				FlushInstructionCache(GetCurrentProcess(), &importAddress->u1.Function, sizeof(importAddress->u1.Function));
				return true;
			}
		}
		return false;
	}

	static void allowDarkAndSetTheme(HWND hWnd, const wchar_t* theme) {
		if (allowDarkModeForWindow) {
			allowDarkModeForWindow(hWnd, TRUE);
		}
		SetWindowTheme(hWnd, theme, nullptr);
	}

	static void applyDarkTitleBar(HWND hWnd) {
		BOOL useDark = TRUE;
		if (FAILED(DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDark, sizeof(useDark)))) {
			DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1, &useDark, sizeof(useDark));
		}
	}

	//
	// comctl32 v6 activation. The CS executable has no manifest, so without this
	// it binds to comctl32 5.82 and SetWindowTheme has nothing to theme. The
	// context is activated on the main thread before any window exists and stays
	// active for the lifetime of the process.
	//

	static bool activateCommonControlsV6() {
		wchar_t dllPath[MAX_PATH] = {};
		if (GetModuleFileNameW(application.m_hInstance, dllPath, MAX_PATH) == 0) {
			return false;
		}

		ACTCTXW context = { sizeof(context) };
		context.dwFlags = ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_HMODULE_VALID;
		context.lpSource = dllPath;
		context.hModule = application.m_hInstance;
		context.lpResourceName = MAKEINTRESOURCEW(IDR_COMCTL32_V6_MANIFEST);

		const auto handle = CreateActCtxW(&context);
		if (handle == INVALID_HANDLE_VALUE) {
			return false;
		}

		ULONG_PTR cookie = 0;
		return ActivateActCtx(handle, &cookie) != FALSE;
	}

	//
	// Theme mode resolution.
	//

	static bool systemPrefersDarkApps() {
		DWORD value = 1;
		DWORD size = sizeof(value);
		RegGetValueA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", "AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &value, &size);
		return value == 0;
	}

	static bool resolveWantsDarkMode() {
		const auto& mode = settings.color_theme.mode;
		if (mode == "dark") {
			return true;
		}
		else if (mode == "auto") {
			return systemPrefersDarkApps();
		}
		else if (mode != "light") {
			log::stream << "Dark mode: unknown color_theme mode '" << mode << "'. Expected 'light', 'dark', or 'auto'." << std::endl;
		}
		return false;
	}

	//
	// Subclass procedures. Windows are picked up at creation time by a CBT hook
	// and subclassed by class name. Heavier theming is deferred to WM_CREATE,
	// when the window is fully constructed.
	//

	static constexpr UINT_PTR SUBCLASS_ID = 0x4353444D; // 'CSDM'
	static constexpr char PROP_DARKENED[] = "CSSE:DarkMode";

	// Brushes returned by default WM_CTLCOLOR* handling, which are safe to
	// replace. Anything else is an intentional brush from the application, such
	// as the color preview swatches in the lighting and region dialogs.
	static bool isOverridableBrush(LRESULT brush) {
		return brush == NULL
			|| brush == reinterpret_cast<LRESULT>(GetSysColorBrush(COLOR_BTNFACE))
			|| brush == reinterpret_cast<LRESULT>(GetSysColorBrush(COLOR_WINDOW))
			|| brush == reinterpret_cast<LRESULT>(GetStockObject(WHITE_BRUSH));
	}

	static HFONT getMessageFont(HWND hWnd) {
		const auto font = GetWindowFont(hWnd);
		return font ? font : reinterpret_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
	}

	// Replaces the light 3D sunken edge that WS_EX_CLIENTEDGE controls draw in
	// their non-client area. Called after default WM_NCPAINT handling.
	static void paintDarkClientEdge(HWND hWnd) {
		if (!(GetWindowLongA(hWnd, GWL_EXSTYLE) & WS_EX_CLIENTEDGE)) {
			return;
		}

		const auto hdc = GetWindowDC(hWnd);
		if (hdc == nullptr) {
			return;
		}

		RECT rect = {};
		GetWindowRect(hWnd, &rect);
		OffsetRect(&rect, -rect.left, -rect.top);
		FrameRect(hdc, &rect, borderBrush);
		InflateRect(&rect, -1, -1);
		FrameRect(hdc, &rect, surfaceBrush);
		ReleaseDC(hWnd, hdc);
	}

	//
	// Toolbar custom draw, handled by the parent's subclass.
	//

	static LRESULT onToolbarCustomDraw(NMTBCUSTOMDRAW* customDraw) {
		switch (customDraw->nmcd.dwDrawStage) {
		case CDDS_PREPAINT:
			FillRect(customDraw->nmcd.hdc, &customDraw->nmcd.rc, controlBrush);
			return CDRF_NOTIFYITEMDRAW;
		case CDDS_ITEMPREPAINT:
			customDraw->clrText = palette::text;
			return TBCDRF_USECDCOLORS;
		}
		return CDRF_DODEFAULT;
	}

	//
	// Header custom draw, handled by the parent list view's subclass.
	//

	static LRESULT onHeaderCustomDraw(NMCUSTOMDRAW* customDraw) {
		const auto hWnd = customDraw->hdr.hwndFrom;
		const auto hdc = customDraw->hdc;

		switch (customDraw->dwDrawStage) {
		case CDDS_PREPAINT: {
			// Fill everything up front so the area past the last column is dark.
			RECT clientRect = {};
			GetClientRect(hWnd, &clientRect);
			FillRect(hdc, &clientRect, controlBrush);
			return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		}
		case CDDS_ITEMPREPAINT: {
			auto itemRect = customDraw->rc;

			char text[260] = {};
			HDITEMA item = {};
			item.mask = HDI_TEXT;
			item.pszText = text;
			item.cchTextMax = sizeof(text);
			SendMessageA(hWnd, HDM_GETITEMA, customDraw->dwItemSpec, reinterpret_cast<LPARAM>(&item));

			FillRect(hdc, &itemRect, (customDraw->uItemState & CDIS_SELECTED) ? controlHotBrush : controlBrush);

			// Right edge separator.
			RECT separator = { itemRect.right - 1, itemRect.top, itemRect.right, itemRect.bottom };
			FillRect(hdc, &separator, borderBrush);

			const auto previousFont = SelectObject(hdc, getMessageFont(hWnd));
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, palette::text);
			itemRect.left += 6;
			itemRect.right -= 6;
			DrawTextA(hdc, text, -1, &itemRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
			SelectObject(hdc, previousFont);

			return CDRF_SKIPDEFAULT;
		}
		case CDDS_POSTPAINT: {
			RECT clientRect = {};
			GetClientRect(hWnd, &clientRect);

			const auto columnCount = Header_GetItemCount(hWnd);
			if (columnCount <= 0) {
				FillRect(hdc, &clientRect, controlBrush);
				return CDRF_DODEFAULT;
			}

			RECT lastItemRect = {};
			if (Header_GetItemRect(hWnd, columnCount - 1, &lastItemRect) && lastItemRect.right < clientRect.right) {
				RECT trailingRect = { lastItemRect.right, clientRect.top, clientRect.right, clientRect.bottom };
				FillRect(hdc, &trailingRect, controlBrush);
			}
			return CDRF_DODEFAULT;
		}
		}
		return CDRF_DODEFAULT;
	}

	//
	// Dark menu bar. The themed menu bar ignores dark app mode, so the main
	// window draws it through the undocumented WM_UAHDRAWMENU* messages that
	// user32 sends on Windows 10+. Structure layouts are community-documented.
	//

	constexpr UINT WM_UAHDRAWMENU = 0x0091;
	constexpr UINT WM_UAHDRAWMENUITEM = 0x0092;

	struct UAHMenu {
		HMENU hMenu;
		HDC hdc;
		DWORD dwFlags;
	};

	struct UAHMenuItemMetrics {
		union {
			struct { DWORD cx, cy; } rgSizeBar[2];
			struct { DWORD cx, cy; } rgSizePopup[4];
		};
	};

	struct UAHMenuPopupMetrics {
		DWORD rgCx[4];
		DWORD fUpdateMaxWidths : 2;
	};

	struct UAHMenuItem {
		int position;
		UAHMenuItemMetrics umim;
		UAHMenuPopupMetrics umpm;
	};

	struct UAHDrawMenuItem {
		DRAWITEMSTRUCT dis;
		UAHMenu um;
		UAHMenuItem umi;
	};

	static bool getMenuBarRect(HWND hWnd, RECT& out_rect) {
		MENUBARINFO barInfo = { sizeof(barInfo) };
		if (!GetMenuBarInfo(hWnd, OBJID_MENU, 0, &barInfo)) {
			return false;
		}
		RECT windowRect = {};
		GetWindowRect(hWnd, &windowRect);
		out_rect = barInfo.rcBar;
		OffsetRect(&out_rect, -windowRect.left, -windowRect.top);
		return true;
	}

	static void onUAHDrawMenuBar(HWND hWnd, const UAHMenu* menu) {
		RECT barRect = {};
		if (getMenuBarRect(hWnd, barRect)) {
			barRect.top -= 1;
			FillRect(menu->hdc, &barRect, backgroundBrush);
		}
	}

	static void onUAHDrawMenuBarItem(HWND, UAHDrawMenuItem* item) {
		char text[260] = {};
		MENUITEMINFOA info = { sizeof(info) };
		info.fMask = MIIM_STRING;
		info.dwTypeData = text;
		info.cch = sizeof(text) - 1;
		GetMenuItemInfoA(item->um.hMenu, item->umi.position, TRUE, &info);

		const auto state = item->dis.itemState;
		const auto background = (state & (ODS_HOTLIGHT | ODS_SELECTED)) ? controlHotBrush : backgroundBrush;
		FillRect(item->dis.hDC, &item->dis.rcItem, background);

		SetBkMode(item->dis.hDC, TRANSPARENT);
		SetTextColor(item->dis.hDC, (state & (ODS_GRAYED | ODS_DISABLED | ODS_INACTIVE)) ? palette::textDisabled : palette::text);
		UINT drawFlags = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
		if (state & ODS_NOACCEL) {
			drawFlags |= DT_HIDEPREFIX;
		}
		DrawTextA(item->dis.hDC, text, -1, &item->dis.rcItem, drawFlags);
	}

	// user32 paints light frame lines directly above and below the menu bar as
	// part of the non-client area; repaint them to match the bar.
	static void paintMenuBarFrameLines(HWND hWnd) {
		RECT barRect = {};
		if (!getMenuBarRect(hWnd, barRect)) {
			return;
		}
		const auto hdc = GetWindowDC(hWnd);
		if (hdc) {
			RECT topLine = { barRect.left, barRect.top - 1, barRect.right, barRect.top };
			FillRect(hdc, &topLine, backgroundBrush);
			RECT bottomLine = { barRect.left, barRect.bottom, barRect.right, barRect.bottom + 1 };
			FillRect(hdc, &bottomLine, backgroundBrush);
			ReleaseDC(hWnd, hdc);
		}
	}

	static bool drawMfcLink(const DRAWITEMSTRUCT* item) {
		char className[64] = {};
		GetClassNameA(item->hwndItem, className, sizeof(className));
		if (_stricmp(className, "MfcLink") != 0) {
			return false;
		}

		char text[512] = {};
		GetWindowTextA(item->hwndItem, text, sizeof(text));

		FillRect(item->hDC, &item->rcItem, backgroundBrush);
		const auto previousFont = SelectObject(item->hDC, getMessageFont(item->hwndItem));
		SetBkMode(item->hDC, TRANSPARENT);
		SetTextColor(item->hDC, item->itemState & ODS_DISABLED ? palette::textDisabled : RGB(0x4C, 0xA0, 0xFF));

		auto textRect = item->rcItem;
		DrawTextA(item->hDC, text, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		if (item->itemState & ODS_FOCUS) {
			DrawFocusRect(item->hDC, &item->rcItem);
		}

		SelectObject(item->hDC, previousFont);
		return true;
	}

	//
	// Dialog windows, including the main editor window and the record edit
	// window classes cloned from #32770.
	//

	static bool isRegionPainterDialog(HWND hWnd) {
		char title[128] = {};
		GetWindowTextA(hWnd, title, sizeof(title));
		return strcmp(title, "Region Painter") == 0;
	}

	static LRESULT CALLBACK regionPainterCanvasSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
	static constexpr UINT_PTR REGION_PAINTER_READY_TIMER = SUBCLASS_ID + 1;

	static LRESULT CALLBACK dialogSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR dwRefData) {
		const bool isMainEditorWindow = dwRefData == 1;
		const bool isPlainDarkWindow = dwRefData == 2;
		const bool isUser32Dialog = dwRefData == 3;
		const bool hasMenuBar = GetMenu(hWnd) != nullptr;

		switch (msg) {
		case WM_INITDIALOG: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			if (isRegionPainterDialog(hWnd)) {
				const auto canvas = GetDlgItem(hWnd, 101);
				SetWindowSubclass(canvas, regionPainterCanvasSubclassProc, SUBCLASS_ID, 0);
				SetTimer(hWnd, REGION_PAINTER_READY_TIMER, 1, nullptr);
			}
			return result;
		}
		case WM_CREATE:
			SetPropA(hWnd, PROP_DARKENED, reinterpret_cast<HANDLE>(1));
			if (allowDarkModeForWindow) {
				allowDarkModeForWindow(hWnd, TRUE);
			}
			if (!(GetWindowLongA(hWnd, GWL_STYLE) & WS_CHILD)) {
				applyDarkTitleBar(hWnd);
			}
			if (GetWindowLongA(hWnd, GWL_STYLE) & (WS_HSCROLL | WS_VSCROLL)) {
				const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
				allowDarkAndSetTheme(hWnd, L"DarkMode_Explorer");
				return result;
			}
			break;
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORBTN: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			if (!isOverridableBrush(result)) {
				return result;
			}
			const auto hdc = reinterpret_cast<HDC>(wParam);
			const auto control = reinterpret_cast<HWND>(lParam);
			SetTextColor(hdc, IsWindowEnabled(control) ? palette::text : palette::textDisabled);
			SetBkColor(hdc, palette::background);
			return reinterpret_cast<LRESULT>(backgroundBrush);
		}
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			if (!isOverridableBrush(result)) {
				return result;
			}
			const auto hdc = reinterpret_cast<HDC>(wParam);
			SetTextColor(hdc, palette::text);
			SetBkColor(hdc, palette::surface);
			return reinterpret_cast<LRESULT>(surfaceBrush);
		}
		case WM_ERASEBKGND:
			// The main editor window erases with COLOR_APPWORKSPACE; dialogs
			// erase through WM_CTLCOLORDLG and need no help here.
			if (isMainEditorWindow || isPlainDarkWindow || isUser32Dialog) {
				RECT clientRect = {};
				GetClientRect(hWnd, &clientRect);
				FillRect(reinterpret_cast<HDC>(wParam), &clientRect, isMainEditorWindow ? workspaceBrush : backgroundBrush);
				return 1;
			}
			break;
		case WM_PAINT:
			if (isUser32Dialog) {
				PAINTSTRUCT paint = {};
				const auto hdc = BeginPaint(hWnd, &paint);
				FillRect(hdc, &paint.rcPaint, backgroundBrush);
				EndPaint(hWnd, &paint);
				return 0;
			}
			break;
		case WM_PRINTCLIENT:
			if (isUser32Dialog) {
				RECT clientRect = {};
				GetClientRect(hWnd, &clientRect);
				FillRect(reinterpret_cast<HDC>(wParam), &clientRect, backgroundBrush);
				return 0;
			}
			break;
		case WM_TIMER:
			if (wParam == REGION_PAINTER_READY_TIMER) {
				KillTimer(hWnd, REGION_PAINTER_READY_TIMER);
				SendMessageA(GetDlgItem(hWnd, 101), WM_PAINT, 0, 0);
				return 0;
			}
			break;
		case WM_NOTIFY: {
			const auto hdr = reinterpret_cast<NMHDR*>(lParam);
			if (hdr->code == NM_CUSTOMDRAW) {
				char className[64] = {};
				GetClassNameA(hdr->hwndFrom, className, sizeof(className));
				if (_stricmp(className, TOOLBARCLASSNAMEA) == 0) {
					return onToolbarCustomDraw(reinterpret_cast<NMTBCUSTOMDRAW*>(lParam));
				}
			}
			break;
		}
		case WM_DRAWITEM:
			if (drawMfcLink(reinterpret_cast<DRAWITEMSTRUCT*>(lParam))) {
				return TRUE;
			}
			break;
		case WM_UAHDRAWMENU:
			if (hasMenuBar) {
				onUAHDrawMenuBar(hWnd, reinterpret_cast<UAHMenu*>(lParam));
				return TRUE;
			}
			break;
		case WM_UAHDRAWMENUITEM:
			if (hasMenuBar) {
				onUAHDrawMenuBarItem(hWnd, reinterpret_cast<UAHDrawMenuItem*>(lParam));
				return TRUE;
			}
			break;
		case WM_NCPAINT:
		case WM_NCACTIVATE:
			if (hasMenuBar) {
				const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
				paintMenuBarFrameLines(hWnd);
				return result;
			}
			break;
		case WM_NCDESTROY:
			KillTimer(hWnd, REGION_PAINTER_READY_TIMER);
			RemovePropA(hWnd, PROP_DARKENED);
			RemoveWindowSubclass(hWnd, dialogSubclassProc, SUBCLASS_ID);
			break;
		}

		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	static LRESULT CALLBACK regionPainterCanvasSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_PAINT:
			// The editor renders this control from its parent dialog procedure.
			// Suppress SS_BLACKRECT's later default paint, which overwrites it.
			SendMessageA(GetParent(hWnd), WM_PAINT, 0, 0);
			return 0;
		case WM_ERASEBKGND:
			return 1;
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, regionPainterCanvasSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	// Toolbars without CCS_NODIVIDER reserve the top edge of their non-client
	// area for a classic etched divider drawn with light system colors.
	static void paintDarkToolbarDivider(HWND hWnd) {
		if (GetWindowLongA(hWnd, GWL_STYLE) & CCS_NODIVIDER) {
			return;
		}

		const auto hdc = GetWindowDC(hWnd);
		if (hdc == nullptr) {
			return;
		}

		RECT rect = {};
		GetWindowRect(hWnd, &rect);
		rect.right -= rect.left;
		rect.left = 0;
		rect.top = 0;
		rect.bottom = GetSystemMetrics(SM_CYEDGE);
		FillRect(hdc, &rect, controlBrush);
		ReleaseDC(hWnd, hdc);
	}

	static LRESULT CALLBACK toolbarSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_CREATE: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			allowDarkAndSetTheme(hWnd, L"DarkMode_Explorer");
			return result;
		}
		case WM_NCPAINT: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			paintDarkToolbarDivider(hWnd);
			return result;
		}
		case WM_CTLCOLORSTATIC: {
			const auto hdc = reinterpret_cast<HDC>(wParam);
			SetTextColor(hdc, palette::text);
			SetBkColor(hdc, palette::control);
			return reinterpret_cast<LRESULT>(controlBrush);
		}
		case WM_ERASEBKGND: {
			RECT clientRect = {};
			GetClientRect(hWnd, &clientRect);
			FillRect(reinterpret_cast<HDC>(wParam), &clientRect, controlBrush);
			return 1;
		}
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, toolbarSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	//
	// Simple controls that only need a theme applied once they exist. The theme
	// name is passed through dwRefData.
	//

	static LRESULT CALLBACK themeOnCreateSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR dwRefData) {
		switch (msg) {
		case WM_CREATE: {
			// Let the control finish initializing before theming it.
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			allowDarkAndSetTheme(hWnd, reinterpret_cast<const wchar_t*>(dwRefData));
			return result;
		}
		case WM_NCPAINT: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			paintDarkClientEdge(hWnd);
			return result;
		}
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, themeOnCreateSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	static void refreshComboListTheme(HWND hWnd) {
		allowDarkAndSetTheme(hWnd, L"DarkMode_Explorer");
		SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_FRAME);
	}

	static LRESULT CALLBACK comboListSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_CREATE: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			refreshComboListTheme(hWnd);
			return result;
		}
		case WM_SHOWWINDOW: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			if (wParam) {
				refreshComboListTheme(hWnd);
			}
			return result;
		}
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, comboListSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	//
	// List views: dark scrollbars and colors, plus custom drawn headers.
	//

	static bool isRegionPainterPalette(HWND hWnd) {
		if (GetDlgCtrlID(hWnd) != 1678) {
			return false;
		}

		return isRegionPainterDialog(GetParent(hWnd));
	}

	static void makeLegacyImageListOpaque(HIMAGELIST imageList) {
		IMAGEINFO imageInfo = {};
		if (!imageList || !ImageList_GetImageInfo(imageList, 0, &imageInfo)) {
			return;
		}

		BITMAP bitmap = {};
		if (GetObjectA(imageInfo.hbmImage, sizeof(bitmap), &bitmap) != sizeof(bitmap) || bitmap.bmBitsPixel != 32) {
			return;
		}

		BITMAPINFO bitmapInfo = {};
		bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biWidth = bitmap.bmWidth;
		bitmapInfo.bmiHeader.biHeight = -bitmap.bmHeight;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;

		std::vector<DWORD> pixels(bitmap.bmWidth * bitmap.bmHeight);
		const auto hdc = GetDC(nullptr);
		const auto scanLines = GetDIBits(hdc, imageInfo.hbmImage, 0, bitmap.bmHeight, pixels.data(), &bitmapInfo, DIB_RGB_COLORS);
		if (scanLines == bitmap.bmHeight) {
			const auto hasAlpha = std::any_of(pixels.begin(), pixels.end(), [](DWORD pixel) {
				return (pixel & 0xFF000000) != 0;
			});
			if (!hasAlpha) {
				for (auto& pixel : pixels) {
					pixel |= 0xFF000000;
				}
				SetDIBits(hdc, imageInfo.hbmImage, 0, bitmap.bmHeight, pixels.data(), &bitmapInfo, DIB_RGB_COLORS);
			}
		}
		ReleaseDC(nullptr, hdc);
	}

	static bool isDataFilesList(HWND hWnd) {
		if (GetDlgCtrlID(hWnd) != 1056) {
			return false;
		}

		char title[64] = {};
		GetWindowTextA(GetParent(hWnd), title, sizeof(title));
		return strcmp(title, "Data Files") == 0;
	}

	// The data files list marks rows with check box icons whose strokes are
	// pure black, invisible on the dark list surface. Recolor the black
	// strokes to the theme text color, leaving colored pixels alone.
	static void recolorImageListBlackGlyphs(HIMAGELIST imageList) {
		IMAGEINFO imageInfo = {};
		if (!imageList || !ImageList_GetImageInfo(imageList, 0, &imageInfo) || imageInfo.hbmMask == nullptr) {
			return;
		}

		BITMAP bitmap = {};
		if (GetObjectA(imageInfo.hbmImage, sizeof(bitmap), &bitmap) != sizeof(bitmap)) {
			return;
		}

		BITMAPINFO bitmapInfo = {};
		bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biWidth = bitmap.bmWidth;
		bitmapInfo.bmiHeader.biHeight = -bitmap.bmHeight;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;

		std::vector<DWORD> pixels(bitmap.bmWidth * bitmap.bmHeight);
		std::vector<DWORD> maskPixels(pixels.size());
		const auto hdc = GetDC(nullptr);
		const auto imageLines = GetDIBits(hdc, imageInfo.hbmImage, 0, bitmap.bmHeight, pixels.data(), &bitmapInfo, DIB_RGB_COLORS);
		const auto maskLines = GetDIBits(hdc, imageInfo.hbmMask, 0, bitmap.bmHeight, maskPixels.data(), &bitmapInfo, DIB_RGB_COLORS);
		if (imageLines == bitmap.bmHeight && maskLines == bitmap.bmHeight) {
			const DWORD stroke = (GetRValue(palette::text) << 16) | (GetGValue(palette::text) << 8) | GetBValue(palette::text);
			for (size_t i = 0; i < pixels.size(); ++i) {
				// Mask black = opaque; transparent pixels must stay black so
				// the masked blit does not tint the background.
				const bool opaque = (maskPixels[i] & 0xFFFFFF) == 0;
				if (opaque && (pixels[i] & 0xFFFFFF) == 0) {
					pixels[i] = stroke;
				}
			}
			SetDIBits(hdc, imageInfo.hbmImage, 0, bitmap.bmHeight, pixels.data(), &bitmapInfo, DIB_RGB_COLORS);
		}
		ReleaseDC(nullptr, hdc);
	}

	static LRESULT CALLBACK listViewSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR dwRefData) {
		const bool isRegionPalette = dwRefData == 1;

		switch (msg) {
		case WM_CREATE: {
			// Let the control finish initializing before theming it.
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			allowDarkAndSetTheme(hWnd, L"DarkMode_Explorer");
			ListView_SetBkColor(hWnd, palette::surface);
			ListView_SetTextBkColor(hWnd, palette::surface);
			ListView_SetTextColor(hWnd, palette::text);
			if (!isRegionPalette) {
				ListView_SetExtendedListViewStyleEx(hWnd, LVS_EX_GRIDLINES, 0);
			}
			return result;
		}
		case LVM_SETEXTENDEDLISTVIEWSTYLE:
			// Grid lines are drawn with light system colors that cannot be
			// themed; suppress them.
			if (!isRegionPalette) {
				if (wParam != 0) {
					wParam |= LVS_EX_GRIDLINES;
				}
				lParam &= ~LVS_EX_GRIDLINES;
			}
			break;
		case LVM_SETIMAGELIST:
			if (isRegionPalette && wParam == LVSIL_SMALL) {
				makeLegacyImageListOpaque(reinterpret_cast<HIMAGELIST>(lParam));
			}
			else if (wParam == LVSIL_SMALL && isDataFilesList(hWnd) && !iconoverride::hasDataFilesCheckOverrides()) {
				recolorImageListBlackGlyphs(reinterpret_cast<HIMAGELIST>(lParam));
			}
			break;
		case WM_NOTIFY: {
			const auto hdr = reinterpret_cast<NMHDR*>(lParam);
			if (hdr->hwndFrom == ListView_GetHeader(hWnd)) {
				if (hdr->code == NM_CUSTOMDRAW) {
					return onHeaderCustomDraw(reinterpret_cast<NMCUSTOMDRAW*>(lParam));
				}
				switch (hdr->code) {
				// The comctl32 v6 header notifies in Unicode, regardless of
				// this module's ANSI build.
				case HDN_ITEMCHANGINGA:
				case HDN_ITEMCHANGINGW:
				case HDN_ITEMCHANGEDA:
				case HDN_ITEMCHANGEDW:
				case HDN_TRACKA:
				case HDN_TRACKW:
				case HDN_ENDTRACKA:
				case HDN_ENDTRACKW:
					InvalidateRect(hdr->hwndFrom, nullptr, TRUE);
					InvalidateRect(hWnd, nullptr, TRUE);
					break;
				}
			}
			break;
		}
		case WM_NCPAINT: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			paintDarkClientEdge(hWnd);
			return result;
		}
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, listViewSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	static LRESULT CALLBACK treeViewSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_CREATE: {
			// Let the control finish initializing before theming it.
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			allowDarkAndSetTheme(hWnd, L"DarkMode_Explorer");
			TreeView_SetBkColor(hWnd, palette::surface);
			TreeView_SetTextColor(hWnd, palette::text);
			TreeView_SetLineColor(hWnd, palette::border);
			return result;
		}
		case WM_NCPAINT: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			paintDarkClientEdge(hWnd);
			return result;
		}
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, treeViewSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	static void applyRichEditColors(HWND hWnd, bool recolorAll = false) {
		SendMessageA(hWnd, EM_SETBKGNDCOLOR, FALSE, palette::surface);

		CHARFORMATA format = {};
		format.cbSize = sizeof(format);
		format.dwMask = CFM_COLOR;
		format.crTextColor = palette::text;
		SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_DEFAULT, reinterpret_cast<LPARAM>(&format));
		if (recolorAll) {
			SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&format));
		}
	}

	static LRESULT CALLBACK richEditSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_CREATE: {
			// Let the control finish initializing before theming it.
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			allowDarkAndSetTheme(hWnd, L"DarkMode_Explorer");
			applyRichEditColors(hWnd);
			return result;
		}
		case WM_SETTEXT: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			applyRichEditColors(hWnd, true);
			return result;
		}
		case WM_ENABLE: {
			// Disabled rich edits revert to a system-color background.
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			applyRichEditColors(hWnd, true);
			return result;
		}
		case WM_PAINT:
			// The script editor starts with an empty disabled rich edit, which
			// ignores EM_SETBKGNDCOLOR and paints with COLOR_3DFACE.
			if (!IsWindowEnabled(hWnd) && GetWindowTextLengthA(hWnd) == 0) {
				PAINTSTRUCT paint = {};
				const auto hdc = BeginPaint(hWnd, &paint);
				RECT clientRect = {};
				GetClientRect(hWnd, &clientRect);
				FillRect(hdc, &clientRect, surfaceBrush);
				EndPaint(hWnd, &paint);
				return 0;
			}
			break;
		case WM_NCPAINT: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			paintDarkClientEdge(hWnd);
			return result;
		}
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, richEditSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	//
	// Statics. Border styles (the color swatches and preview frames in the
	// cell, lighting, and preview windows) draw light 3D non-client edges
	// that cannot be themed; repaint the border rings in the dark border
	// color. Borderless statics are unaffected.
	//

	static void paintDarkStaticBorder(HWND hWnd) {
		RECT windowRect = {};
		GetWindowRect(hWnd, &windowRect);
		POINT clientOrigin = {};
		ClientToScreen(hWnd, &clientOrigin);

		const auto frame = clientOrigin.y - windowRect.top;
		if (frame <= 0) {
			return;
		}

		const auto hdc = GetWindowDC(hWnd);
		if (hdc == nullptr) {
			return;
		}

		OffsetRect(&windowRect, -windowRect.left, -windowRect.top);
		for (auto i = 0; i < frame; ++i) {
			FrameRect(hdc, &windowRect, borderBrush);
			InflateRect(&windowRect, -1, -1);
		}
		ReleaseDC(hWnd, hdc);
	}

	static LRESULT CALLBACK staticSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_NCPAINT: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			paintDarkStaticBorder(hWnd);
			return result;
		}
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, staticSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	//
	// Buttons. Push buttons, check boxes, and radio buttons render correctly
	// with the DarkMode_Explorer theme, but themed group boxes draw their label
	// text black, so those are painted manually.
	//

	static bool isGroupBox(HWND hWnd) {
		return (GetWindowLongA(hWnd, GWL_STYLE) & BS_TYPEMASK) == BS_GROUPBOX;
	}

	static bool isRadioButton(HWND hWnd) {
		const auto type = GetWindowLongA(hWnd, GWL_STYLE) & BS_TYPEMASK;
		return type == BS_RADIOBUTTON || type == BS_AUTORADIOBUTTON;
	}

	static bool isD3DDriverTypeButton(HWND hWnd) {
		const auto id = GetDlgCtrlID(hWnd);
		if (id != 1002 && id != 1003) {
			return false;
		}

		char title[128] = {};
		GetWindowTextA(GetParent(hWnd), title, sizeof(title));
		return strcmp(title, "TES3 Direct3D Driver Selection") == 0;
	}

	static void paintGroupBox(HWND hWnd, HDC hdc) {
		RECT clientRect = {};
		GetClientRect(hWnd, &clientRect);

		char text[260] = {};
		GetWindowTextA(hWnd, text, sizeof(text));

		const auto font = getMessageFont(hWnd);
		const auto previousFont = SelectObject(hdc, font);

		SIZE textSize = {};
		GetTextExtentPoint32A(hdc, text, static_cast<int>(strlen(text)), &textSize);

		// Frame, dropped below the caption's center line.
		auto frameRect = clientRect;
		frameRect.top += textSize.cy / 2;
		const auto previousPen = SelectObject(hdc, borderPen);
		const auto previousBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
		RoundRect(hdc, frameRect.left, frameRect.top, frameRect.right, frameRect.bottom, 4, 4);
		SelectObject(hdc, previousBrush);
		SelectObject(hdc, previousPen);

		if (text[0] != '\0') {
			RECT textRect = { clientRect.left + 8, clientRect.top, clientRect.left + 8 + textSize.cx + 4, clientRect.top + textSize.cy };
			FillRect(hdc, &textRect, backgroundBrush);
			textRect.left += 2;
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, IsWindowEnabled(hWnd) ? palette::text : palette::textDisabled);
			DrawTextA(hdc, text, -1, &textRect, DT_LEFT | DT_TOP | DT_SINGLELINE);
		}

		SelectObject(hdc, previousFont);
	}

	static void paintRadioButton(HWND hWnd, HDC hdc) {
		RECT clientRect = {};
		GetClientRect(hWnd, &clientRect);
		FillRect(hdc, &clientRect, backgroundBrush);

		const auto enabled = IsWindowEnabled(hWnd) != FALSE;
		const auto buttonState = static_cast<UINT>(SendMessageA(hWnd, BM_GETSTATE, 0, 0));
		const auto checked = SendMessageA(hWnd, BM_GETCHECK, 0, 0) != BST_UNCHECKED;
		const auto hot = (buttonState & BST_HOT) != 0;
		const auto pressed = (buttonState & BST_PUSHED) != 0;

		// BP_RADIOBUTTON states are unchecked normal/hot/pressed/disabled,
		// followed by checked normal/hot/pressed/disabled.
		auto themeState = !enabled ? 4 : pressed ? 3 : hot ? 2 : 1;
		if (checked) {
			themeState += 4;
		}

		SIZE glyphSize = { GetSystemMetrics(SM_CXMENUCHECK), GetSystemMetrics(SM_CYMENUCHECK) };
		const auto theme = OpenThemeData(hWnd, L"Button");
		if (theme) {
			GetThemePartSize(theme, hdc, 2, themeState, nullptr, TS_DRAW, &glyphSize);
		}

		const auto style = GetWindowLongA(hWnd, GWL_STYLE);
		const auto leftText = (style & BS_LEFTTEXT) != 0;
		RECT glyphRect = {
			leftText ? clientRect.right - glyphSize.cx : clientRect.left,
			clientRect.top + (clientRect.bottom - clientRect.top - glyphSize.cy) / 2,
			leftText ? clientRect.right : clientRect.left + glyphSize.cx,
			0,
		};
		glyphRect.bottom = glyphRect.top + glyphSize.cy;

		if (theme) {
			DrawThemeBackground(theme, hdc, 2, themeState, &glyphRect, nullptr);
			CloseThemeData(theme);
		}
		else {
			DrawFrameControl(hdc, &glyphRect, DFC_BUTTON, DFCS_BUTTONRADIO
				| (checked ? DFCS_CHECKED : 0)
				| (!enabled ? DFCS_INACTIVE : 0)
				| (pressed ? DFCS_PUSHED : 0));
		}

		char text[260] = {};
		GetWindowTextA(hWnd, text, sizeof(text));
		RECT textRect = clientRect;
		if (leftText) {
			textRect.right = glyphRect.left - 4;
		}
		else {
			textRect.left = glyphRect.right + 4;
		}

		const auto previousFont = SelectObject(hdc, getMessageFont(hWnd));
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, enabled ? palette::text : palette::textDisabled);
		auto drawFlags = DT_VCENTER | DT_SINGLELINE;
		drawFlags |= leftText ? DT_RIGHT : DT_LEFT;
		DrawTextA(hdc, text, -1, &textRect, drawFlags);
		SelectObject(hdc, previousFont);

		if (buttonState & BST_FOCUS) {
			DrawFocusRect(hdc, &textRect);
		}
	}

	static LRESULT CALLBACK buttonSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_CREATE: {
			// Let the control finish initializing before theming it.
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			if (isD3DDriverTypeButton(hWnd)) {
				SetWindowTheme(hWnd, L"", L"");
			}
			else {
				allowDarkAndSetTheme(hWnd, L"DarkMode_Explorer");
			}
			return result;
		}
		case WM_PAINT: {
			if (isRadioButton(hWnd)) {
				PAINTSTRUCT paint = {};
				const auto hdc = BeginPaint(hWnd, &paint);
				paintRadioButton(hWnd, hdc);
				EndPaint(hWnd, &paint);
				return 0;
			}
			else if (isGroupBox(hWnd)) {
				PAINTSTRUCT paint = {};
				const auto hdc = BeginPaint(hWnd, &paint);
				paintGroupBox(hWnd, hdc);
				EndPaint(hWnd, &paint);
				return 0;
			}
			break;
		}
		case WM_ENABLE:
		case BM_SETCHECK:
		case BM_SETSTATE:
		case BM_SETSTYLE:
			if (isGroupBox(hWnd) || isRadioButton(hWnd)) {
				InvalidateRect(hWnd, nullptr, TRUE);
			}
			break;
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, buttonSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	//
	// Tab controls have no dark theme support at all and are fully custom drawn.
	//

	static void paintTabControl(HWND hWnd, HDC hdc) {
		RECT clientRect = {};
		GetClientRect(hWnd, &clientRect);
		FillRect(hdc, &clientRect, backgroundBrush);

		// Border around the display area. Button-style tabs draw no page frame.
		if (!(GetWindowLongA(hWnd, GWL_STYLE) & TCS_BUTTONS)) {
			auto pageRect = clientRect;
			TabCtrl_AdjustRect(hWnd, FALSE, &pageRect);
			InflateRect(&pageRect, 1, 1);
			FrameRect(hdc, &pageRect, borderBrush);
		}

		const int count = TabCtrl_GetItemCount(hWnd);
		const int selected = TabCtrl_GetCurSel(hWnd);
		const auto previousFont = SelectObject(hdc, getMessageFont(hWnd));
		SetBkMode(hdc, TRANSPARENT);

		for (int i = 0; i < count; ++i) {
			RECT itemRect = {};
			TabCtrl_GetItemRect(hWnd, i, &itemRect);

			FillRect(hdc, &itemRect, (i == selected) ? controlHotBrush : controlBrush);
			FrameRect(hdc, &itemRect, borderBrush);

			char text[128] = {};
			TCITEMA item = {};
			item.mask = TCIF_TEXT;
			item.pszText = text;
			item.cchTextMax = sizeof(text);
			SendMessageA(hWnd, TCM_GETITEMA, i, reinterpret_cast<LPARAM>(&item));

			SetTextColor(hdc, palette::text);
			DrawTextA(hdc, text, -1, &itemRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		}

		SelectObject(hdc, previousFont);
	}

	static LRESULT CALLBACK tabControlSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_CREATE: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			if (allowDarkModeForWindow) {
				allowDarkModeForWindow(hWnd, TRUE);
			}
			return result;
		}
		case WM_PAINT: {
			PAINTSTRUCT paint = {};
			const auto hdc = BeginPaint(hWnd, &paint);
			paintTabControl(hWnd, hdc);
			EndPaint(hWnd, &paint);
			return 0;
		}
		case WM_ERASEBKGND:
			return 1;
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, tabControlSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	//
	// Status bar, fully custom drawn.
	//

	static void paintStatusBar(HWND hWnd, HDC hdc) {
		RECT clientRect = {};
		GetClientRect(hWnd, &clientRect);
		FillRect(hdc, &clientRect, controlBrush);

		RECT topEdge = { clientRect.left, clientRect.top, clientRect.right, clientRect.top + 1 };
		FillRect(hdc, &topEdge, borderBrush);

		const int parts = static_cast<int>(SendMessageA(hWnd, SB_GETPARTS, 0, 0));
		const auto previousFont = SelectObject(hdc, getMessageFont(hWnd));
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, palette::text);

		for (int i = 0; i < parts; ++i) {
			RECT partRect = {};
			SendMessageA(hWnd, SB_GETRECT, i, reinterpret_cast<LPARAM>(&partRect));

			if (i + 1 < parts) {
				RECT separator = { partRect.right - 1, partRect.top + 2, partRect.right, partRect.bottom - 2 };
				FillRect(hdc, &separator, borderBrush);
			}

			char text[260] = {};
			const auto length = LOWORD(SendMessageA(hWnd, SB_GETTEXTLENGTHA, i, 0));
			if (length == 0 || length >= sizeof(text)) {
				continue;
			}
			SendMessageA(hWnd, SB_GETTEXTA, i, reinterpret_cast<LPARAM>(text));

			partRect.left += 4;
			partRect.right -= 4;
			DrawTextA(hdc, text, -1, &partRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
		}

		SelectObject(hdc, previousFont);
	}

	static LRESULT CALLBACK statusBarSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_PAINT: {
			PAINTSTRUCT paint = {};
			const auto hdc = BeginPaint(hWnd, &paint);
			paintStatusBar(hWnd, hdc);
			EndPaint(hWnd, &paint);
			return 0;
		}
		case WM_ERASEBKGND:
			return 1;
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, statusBarSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	//
	// MDI client area: the workspace background behind the editor's child
	// windows, which otherwise erases with COLOR_APPWORKSPACE.
	//

	static LRESULT CALLBACK mdiClientSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_ERASEBKGND: {
			RECT clientRect = {};
			GetClientRect(hWnd, &clientRect);
			FillRect(reinterpret_cast<HDC>(wParam), &clientRect, workspaceBrush);
			return 1;
		}
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, mdiClientSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	//
	// Window creation dispatch.
	//

	static bool isDialogClass(const char* className) {
		static constexpr const char* dialogClasses[] = {
			"#32770",
			// Record edit window classes registered in WinMain as clones of the
			// dialog class.
			"ActivatorClass", "AlchemyClass", "ArmorClass", "CreatureClass",
			"LockPickClass", "NPCClass", "WeaponClass", "FaceClass",
			"PlaneClass", "MonitorClass", "ViewerClass", "SpeakerClass",
			"LandClass",
		};
		for (const auto dialogClass : dialogClasses) {
			if (_stricmp(className, dialogClass) == 0) {
				return true;
			}
		}
		return false;
	}

	// Theme editor, CSSE, and common dialogs. USER32 owns system-provided
	// dialogs such as message boxes opened by the editor.
	static bool isOwnDialog(const CREATESTRUCTA* createStruct) {
		return createStruct->hInstance == nullptr
			|| createStruct->hInstance == GetModuleHandleA(nullptr)
			|| createStruct->hInstance == application.m_hInstance
			|| createStruct->hInstance == GetModuleHandleA("comdlg32.dll")
			|| createStruct->hInstance == GetModuleHandleA("user32.dll");
	}

	static bool hasDarkenedAncestor(HWND hWnd) {
		const auto desktop = GetDesktopWindow();
		for (auto parent = hWnd; parent && parent != desktop; parent = GetAncestor(parent, GA_PARENT)) {
			if (GetPropA(parent, PROP_DARKENED)) {
				return true;
			}
		}
		return false;
	}

	static void logDispatch(HWND hWnd, const char* className, const char* decision, bool subclassed) {
		if constexpr (LOG_WINDOW_DISPATCH) {
			log::stream << "Dark mode: hwnd 0x" << std::hex << reinterpret_cast<DWORD>(hWnd) << std::dec
				<< " class '" << className << "' -> " << decision
				<< (subclassed ? "" : " (subclass failed)") << std::endl;
		}
	}

	static void onWindowCreating(HWND hWnd, CREATESTRUCTA* createStruct) {
		char className[64] = {};
		if (GetClassNameA(hWnd, className, sizeof(className)) == 0) {
			return;
		}

		if (isDialogClass(className)) {
			if (isOwnDialog(createStruct)) {
				const auto isUser32Dialog = createStruct->hInstance == GetModuleHandleA("user32.dll");
				const auto subclassed = SetWindowSubclass(hWnd, dialogSubclassProc, SUBCLASS_ID, isUser32Dialog ? 3 : 0);
				logDispatch(hWnd, className, isUser32Dialog ? "USER32 dialog" : "dialog", subclassed);
			}
			else {
				logDispatch(hWnd, className, "skipped (external dialog)", true);
			}
			return;
		}

		if (_stricmp(className, "TES3 Editor Class") == 0) {
			const auto subclassed = SetWindowSubclass(hWnd, dialogSubclassProc, SUBCLASS_ID, 1);
			logDispatch(hWnd, className, "main editor window", subclassed);
			return;
		}
		if (_stricmp(className, "CSSE_LayersWindow") == 0) {
			const auto subclassed = SetWindowSubclass(hWnd, dialogSubclassProc, SUBCLASS_ID, 2);
			logDispatch(hWnd, className, "layers window", subclassed);
			return;
		}

		// Tooltips are transient popups; theme them regardless of ancestry.
		if (_stricmp(className, TOOLTIPS_CLASSA) == 0) {
			SetWindowSubclass(hWnd, themeOnCreateSubclassProc, SUBCLASS_ID, reinterpret_cast<DWORD_PTR>(L"DarkMode_Explorer"));
			return;
		}
		if (_stricmp(className, "ComboLBox") == 0) {
			// Combo scrollbars are non-client and may be added only when the
			// popup opens, so establish dark mode before WM_NCCREATE as well.
			allowDarkAndSetTheme(hWnd, L"DarkMode_Explorer");
			SetWindowSubclass(hWnd, comboListSubclassProc, SUBCLASS_ID, 0);
			return;
		}

		// Everything below is a child control; only theme it when it lives in a
		// window we have darkened.
		if (!hasDarkenedAncestor(createStruct->hwndParent)) {
			logDispatch(hWnd, className, "skipped (no darkened ancestor)", true);
			return;
		}

		if (_stricmp(className, WC_LISTVIEWA) == 0) {
			const auto regionPalette = isRegionPainterPalette(hWnd);
			logDispatch(hWnd, className, "list view", SetWindowSubclass(hWnd, listViewSubclassProc, SUBCLASS_ID, regionPalette));
		}
		else if (_stricmp(className, WC_TREEVIEWA) == 0) {
			logDispatch(hWnd, className, "tree view", SetWindowSubclass(hWnd, treeViewSubclassProc, SUBCLASS_ID, 0));
		}
		else if (_stricmp(className, WC_TABCONTROLA) == 0) {
			logDispatch(hWnd, className, "tab control", SetWindowSubclass(hWnd, tabControlSubclassProc, SUBCLASS_ID, 0));
		}
		else if (_stricmp(className, "Button") == 0) {
			logDispatch(hWnd, className, "button", SetWindowSubclass(hWnd, buttonSubclassProc, SUBCLASS_ID, 0));
		}
		else if (_stricmp(className, "ComboBox") == 0) {
			logDispatch(hWnd, className, "combo box", SetWindowSubclass(hWnd, themeOnCreateSubclassProc, SUBCLASS_ID, reinterpret_cast<DWORD_PTR>(L"DarkMode_CFD")));
		}
		else if (_stricmp(className, TOOLBARCLASSNAMEA) == 0) {
			logDispatch(hWnd, className, "toolbar", SetWindowSubclass(hWnd, toolbarSubclassProc, SUBCLASS_ID, 0));
		}
		else if (_stricmp(className, "Edit") == 0 || _stricmp(className, "ListBox") == 0 || _stricmp(className, "ScrollBar") == 0 || _stricmp(className, UPDOWN_CLASSA) == 0) {
			logDispatch(hWnd, className, "themed control", SetWindowSubclass(hWnd, themeOnCreateSubclassProc, SUBCLASS_ID, reinterpret_cast<DWORD_PTR>(L"DarkMode_Explorer")));
		}
		else if (_stricmp(className, "Static") == 0) {
			logDispatch(hWnd, className, "static", SetWindowSubclass(hWnd, staticSubclassProc, SUBCLASS_ID, 0));
		}
		else if (_stricmp(className, STATUSCLASSNAMEA) == 0) {
			logDispatch(hWnd, className, "status bar", SetWindowSubclass(hWnd, statusBarSubclassProc, SUBCLASS_ID, 0));
		}
		else if (_stricmp(className, "MDIClient") == 0) {
			logDispatch(hWnd, className, "mdi client", SetWindowSubclass(hWnd, mdiClientSubclassProc, SUBCLASS_ID, 0));
		}
		else if (_stricmp(className, "RICHEDIT") == 0 || _stricmp(className, RICHEDIT_CLASSA) == 0) {
			logDispatch(hWnd, className, "rich edit", SetWindowSubclass(hWnd, richEditSubclassProc, SUBCLASS_ID, 0));
		}
		else {
			logDispatch(hWnd, className, "unhandled class", true);
		}
	}

	static HHOOK cbtHook = nullptr;

	static LRESULT CALLBACK cbtHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
		if (nCode == HCBT_CREATEWND) {
			onWindowCreating(reinterpret_cast<HWND>(wParam), reinterpret_cast<CBT_CREATEWND*>(lParam)->lpcs);
		}
		return CallNextHookEx(cbtHook, nCode, wParam, lParam);
	}

	//
	// Toolbar images.
	//

	static void paintPropertyGridHeader(HWND hWnd, HDC hdc) {
		RECT clientRect = {};
		GetClientRect(hWnd, &clientRect);
		FillRect(hdc, &clientRect, controlBrush);

		const auto previousFont = SelectObject(hdc, getMessageFont(hWnd));
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, palette::text);

		const auto itemCount = Header_GetItemCount(hWnd);
		for (int i = 0; i < itemCount; ++i) {
			RECT itemRect = {};
			if (!Header_GetItemRect(hWnd, i, &itemRect)) {
				continue;
			}

			FillRect(hdc, &itemRect, controlBrush);
			FrameRect(hdc, &itemRect, borderBrush);

			char text[260] = {};
			HDITEMA item = {};
			item.mask = HDI_TEXT | HDI_FORMAT;
			item.pszText = text;
			item.cchTextMax = sizeof(text);
			Header_GetItem(hWnd, i, &item);

			itemRect.left += 5;
			itemRect.right -= 5;
			auto flags = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
			flags |= (item.fmt & HDF_RIGHT) ? DT_RIGHT : (item.fmt & HDF_CENTER) ? DT_CENTER : DT_LEFT;
			DrawTextA(hdc, text, -1, &itemRect, flags);
		}

		SelectObject(hdc, previousFont);
	}

	static LRESULT CALLBACK propertyGridHeaderSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_PAINT: {
			PAINTSTRUCT paint = {};
			const auto hdc = BeginPaint(hWnd, &paint);
			paintPropertyGridHeader(hWnd, hdc);
			EndPaint(hWnd, &paint);
			return 0;
		}
		case WM_ERASEBKGND:
			return 1;
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, propertyGridHeaderSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	//
	// Scroll bar controls. Unlike non-client scroll bars, the stand-alone
	// scroll bar control ignores the DarkMode_Explorer theme, so the settings
	// grid's scroll bar is fully custom drawn and custom tracked. Mouse input
	// is not forwarded to the control, because the native tracking loop
	// paints the light scroll bar directly to the screen.
	//

	static constexpr UINT_PTR SCROLLBAR_REPEAT_TIMER = SUBCLASS_ID + 2;

	// Tracking state for the scroll bar holding mouse capture. Only one
	// window can hold capture at a time.
	static int scrollBarThumbGrabOffset = -1;
	static WORD scrollBarRepeatCode = 0;

	struct ScrollBarLayout {
		SCROLLINFO info = { sizeof(SCROLLINFO), SIF_POS | SIF_PAGE | SIF_RANGE };
		RECT upArrow = {};
		RECT downArrow = {};
		RECT track = {};
		RECT thumb = {};
		bool scrollable = false;
	};

	static ScrollBarLayout getScrollBarLayout(HWND hWnd) {
		ScrollBarLayout layout;
		GetScrollInfo(hWnd, SB_CTL, &layout.info);

		RECT client = {};
		GetClientRect(hWnd, &client);
		auto arrowHeight = GetSystemMetrics(SM_CYVSCROLL);
		if (arrowHeight * 2 > client.bottom - client.top) {
			arrowHeight = (client.bottom - client.top) / 2;
		}
		layout.upArrow = { client.left, client.top, client.right, client.top + arrowHeight };
		layout.downArrow = { client.left, client.bottom - arrowHeight, client.right, client.bottom };
		layout.track = { client.left, layout.upArrow.bottom, client.right, layout.downArrow.top };

		const auto& info = layout.info;
		const auto page = static_cast<int>(info.nPage);
		const auto range = info.nMax - info.nMin + 1;
		const auto trackHeight = layout.track.bottom - layout.track.top;
		if (page <= 0 || range <= page || trackHeight <= 0) {
			return layout;
		}
		layout.scrollable = true;

		auto thumbHeight = MulDiv(trackHeight, page, range);
		if (thumbHeight < 8) {
			thumbHeight = 8;
		}
		if (thumbHeight > trackHeight) {
			thumbHeight = trackHeight;
		}

		const auto scrollRange = info.nMax - page + 1 - info.nMin;
		const auto thumbTop = layout.track.top + MulDiv(info.nPos - info.nMin, trackHeight - thumbHeight, scrollRange > 0 ? scrollRange : 1);
		layout.thumb = { client.left, thumbTop, client.right, thumbTop + thumbHeight };
		return layout;
	}

	static void paintScrollBarArrow(HDC hdc, const RECT& rect, bool pointsUp, bool enabled) {
		auto halfWidth = (rect.right - rect.left) / 5;
		if (halfWidth < 2) {
			halfWidth = 2;
		}
		const auto halfHeight = (halfWidth + 1) / 2;
		const auto centerX = (rect.left + rect.right) / 2;
		const auto centerY = (rect.top + rect.bottom) / 2;
		const auto baseY = pointsUp ? centerY + halfHeight : centerY - halfHeight;
		const auto apexY = pointsUp ? centerY - halfHeight : centerY + halfHeight;
		const POINT points[3] = {
			{ centerX - halfWidth, baseY },
			{ centerX + halfWidth, baseY },
			{ centerX, apexY },
		};

		const auto color = enabled ? palette::text : palette::textDisabled;
		const auto previousPen = SelectObject(hdc, GetStockObject(DC_PEN));
		const auto previousBrush = SelectObject(hdc, GetStockObject(DC_BRUSH));
		SetDCPenColor(hdc, color);
		SetDCBrushColor(hdc, color);
		Polygon(hdc, points, 3);
		SelectObject(hdc, previousBrush);
		SelectObject(hdc, previousPen);
	}

	static void paintScrollBar(HWND hWnd, HDC hdc) {
		const auto layout = getScrollBarLayout(hWnd);

		RECT client = {};
		GetClientRect(hWnd, &client);
		FillRect(hdc, &client, surfaceBrush);

		paintScrollBarArrow(hdc, layout.upArrow, true, layout.scrollable);
		paintScrollBarArrow(hdc, layout.downArrow, false, layout.scrollable);

		if (layout.scrollable) {
			FillRect(hdc, &layout.thumb, controlHotBrush);
		}
	}

	static void sendScrollCommand(HWND hWnd, WORD code, WORD position = 0) {
		SendMessageA(GetParent(hWnd), WM_VSCROLL, MAKEWPARAM(code, position), reinterpret_cast<LPARAM>(hWnd));
	}

	static LRESULT CALLBACK scrollBarSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
		switch (msg) {
		case WM_PAINT: {
			PAINTSTRUCT paint = {};
			const auto hdc = BeginPaint(hWnd, &paint);
			paintScrollBar(hWnd, hdc);
			EndPaint(hWnd, &paint);
			return 0;
		}
		case WM_ERASEBKGND:
			return 1;
		// State changes repaint without the default handling's redraw, which
		// can draw the light scroll bar directly.
		case SBM_SETPOS: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, FALSE);
			InvalidateRect(hWnd, nullptr, FALSE);
			return result;
		}
		case SBM_SETSCROLLINFO: {
			const auto result = DefSubclassProc(hWnd, msg, FALSE, lParam);
			InvalidateRect(hWnd, nullptr, FALSE);
			return result;
		}
		case SBM_SETRANGEREDRAW: {
			const auto result = DefSubclassProc(hWnd, SBM_SETRANGE, wParam, lParam);
			InvalidateRect(hWnd, nullptr, FALSE);
			return result;
		}
		case SBM_ENABLE_ARROWS: {
			const auto result = DefSubclassProc(hWnd, msg, wParam, lParam);
			InvalidateRect(hWnd, nullptr, FALSE);
			return result;
		}
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK: {
			const auto layout = getScrollBarLayout(hWnd);
			if (!layout.scrollable) {
				return 0;
			}
			const POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			SetCapture(hWnd);
			if (PtInRect(&layout.thumb, point)) {
				scrollBarThumbGrabOffset = point.y - layout.thumb.top;
			}
			else {
				if (point.y < layout.upArrow.bottom) {
					scrollBarRepeatCode = SB_LINEUP;
				}
				else if (point.y >= layout.downArrow.top) {
					scrollBarRepeatCode = SB_LINEDOWN;
				}
				else {
					scrollBarRepeatCode = (point.y < layout.thumb.top) ? SB_PAGEUP : SB_PAGEDOWN;
				}
				sendScrollCommand(hWnd, scrollBarRepeatCode);
				SetTimer(hWnd, SCROLLBAR_REPEAT_TIMER, 350, nullptr);
			}
			return 0;
		}
		case WM_MOUSEMOVE:
			if (GetCapture() == hWnd && scrollBarThumbGrabOffset >= 0) {
				const auto layout = getScrollBarLayout(hWnd);
				const auto slack = (layout.track.bottom - layout.track.top) - (layout.thumb.bottom - layout.thumb.top);
				if (layout.scrollable && slack > 0) {
					const auto& info = layout.info;
					const auto scrollRange = info.nMax - static_cast<int>(info.nPage) + 1 - info.nMin;
					const auto offset = GET_Y_LPARAM(lParam) - scrollBarThumbGrabOffset - layout.track.top;
					auto position = info.nMin + MulDiv(offset, scrollRange, slack);
					if (position < info.nMin) {
						position = info.nMin;
					}
					if (position > info.nMin + scrollRange) {
						position = info.nMin + scrollRange;
					}
					if (position != info.nPos) {
						sendScrollCommand(hWnd, SB_THUMBTRACK, static_cast<WORD>(position));
					}
				}
			}
			return 0;
		case WM_LBUTTONUP:
			if (GetCapture() == hWnd) {
				ReleaseCapture();
			}
			return 0;
		case WM_CAPTURECHANGED:
			if (scrollBarThumbGrabOffset >= 0) {
				scrollBarThumbGrabOffset = -1;
				sendScrollCommand(hWnd, SB_ENDSCROLL);
			}
			KillTimer(hWnd, SCROLLBAR_REPEAT_TIMER);
			break;
		case WM_TIMER:
			if (wParam == SCROLLBAR_REPEAT_TIMER) {
				if (GetCapture() == hWnd) {
					sendScrollCommand(hWnd, scrollBarRepeatCode);
					SetTimer(hWnd, SCROLLBAR_REPEAT_TIMER, 60, nullptr);
				}
				else {
					KillTimer(hWnd, SCROLLBAR_REPEAT_TIMER);
				}
				return 0;
			}
			break;
		case WM_NCDESTROY:
			KillTimer(hWnd, SCROLLBAR_REPEAT_TIMER);
			RemoveWindowSubclass(hWnd, scrollBarSubclassProc, SUBCLASS_ID);
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	void themePropertyGrid(HWND hWndGrid, HWND hWndHeader, HWND hWndScrollBar) {
		if (!active) {
			return;
		}

		if (hWndGrid) {
			allowDarkAndSetTheme(hWndGrid, L"DarkMode_Explorer");
			SetWindowPos(hWndGrid, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			RedrawWindow(hWndGrid, nullptr, nullptr, RDW_INVALIDATE | RDW_FRAME);
		}
		if (hWndHeader) {
			SetWindowSubclass(hWndHeader, propertyGridHeaderSubclassProc, SUBCLASS_ID, 0);
			InvalidateRect(hWndHeader, nullptr, TRUE);
		}
		if (hWndScrollBar) {
			SetWindowSubclass(hWndScrollBar, scrollBarSubclassProc, SUBCLASS_ID, 0);
			InvalidateRect(hWndScrollBar, nullptr, TRUE);
		}
	}

	void remapToolbarImages(HWND hWndToolbar, HINSTANCE hImageInstance, UINT bitmapId, int imageWidth, int imageHeight) {
		if (!active || hWndToolbar == nullptr) {
			return;
		}

		bool hasAlpha = false;
		auto bitmap = iconoverride::loadBitmapOverride(hImageInstance, bitmapId, hasAlpha);
		if (bitmap == nullptr) {
			bitmap = reinterpret_cast<HBITMAP>(LoadImageA(hImageInstance, MAKEINTRESOURCEA(bitmapId), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
		}
		if (bitmap == nullptr) {
			return;
		}

		BITMAP bitmapInfo = {};
		GetObjectA(bitmap, sizeof(bitmapInfo), &bitmapInfo);

		// Zero dimensions mean toolbar defaults: 16 wide, strip height tall.
		if (imageWidth <= 0) {
			imageWidth = 16;
		}
		if (imageHeight <= 0) {
			imageHeight = bitmapInfo.bmHeight;
		}

		const int imageCount = bitmapInfo.bmWidth / imageWidth;
		if (imageCount <= 0) {
			DeleteObject(bitmap);
			return;
		}

		const auto imageList = ImageList_Create(imageWidth, imageHeight, hasAlpha ? ILC_COLOR32 : (ILC_COLOR32 | ILC_MASK), imageCount, 0);
		if (hasAlpha) {
			ImageList_Add(imageList, bitmap, nullptr);
		}
		else {
			ImageList_AddMasked(imageList, bitmap, RGB(192, 192, 192));
		}
		DeleteObject(bitmap);

		const auto previous = reinterpret_cast<HIMAGELIST>(SendMessageA(hWndToolbar, TB_SETIMAGELIST, 0, reinterpret_cast<LPARAM>(imageList)));
		if (previous) {
			ImageList_Destroy(previous);
		}
	}

	//
	// Initialization.
	//

	void initialize() {
		if (!resolveWantsDarkMode()) {
			return;
		}

		if (isRunningUnderWine()) {
			log::stream << "Dark mode: disabled, requires native Windows 10 1809 or later." << std::endl;
			return;
		}

		const auto buildNumber = getWindowsBuildNumber();
		if (buildNumber < 17763) {
			log::stream << "Dark mode: disabled, requires Windows 10 1809 (build 17763) or later. Detected build: " << buildNumber << "." << std::endl;
			return;
		}

		if (!activateCommonControlsV6()) {
			log::stream << "Dark mode: disabled, could not activate comctl32 v6." << std::endl;
			return;
		}

		if (!loadAndEnableDarkModeAPIs(buildNumber)) {
			log::stream << "Dark mode: disabled, uxtheme dark mode exports unavailable." << std::endl;
			return;
		}

		if (!hookComctl32ScrollBarTheme()) {
			log::stream << "Dark mode: could not install non-client scrollbar theme hook." << std::endl;
		}

		createDrawingResources();

		cbtHook = SetWindowsHookExA(WH_CBT, cbtHookProc, nullptr, GetCurrentThreadId());
		if (cbtHook == nullptr) {
			log::stream << "Dark mode: disabled, could not install window creation hook." << std::endl;
			return;
		}

		active = true;

		// Repack highlight colors now that the dark palette applies.
		settings.color_theme.packColors();

		log::stream << "Dark mode: enabled." << std::endl;
	}
}
