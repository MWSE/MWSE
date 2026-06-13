#pragma once

namespace se::cs::darkmode {
	// The fixed dark palette. Highlight colors for modified/deleted rows remain
	// configurable through Settings_t::ColorTheme.
	namespace palette {
		constexpr COLORREF background = RGB(0x2B, 0x2B, 0x2B);
		constexpr COLORREF workspace = RGB(0x14, 0x14, 0x14);
		constexpr COLORREF surface = RGB(0x19, 0x19, 0x19);
		constexpr COLORREF control = RGB(0x3C, 0x3C, 0x3C);
		constexpr COLORREF controlHot = RGB(0x55, 0x55, 0x55);
		constexpr COLORREF text = RGB(0xC0, 0xC0, 0xC0);
		constexpr COLORREF textDisabled = RGB(0x83, 0x83, 0x83);
		constexpr COLORREF border = RGB(0x64, 0x64, 0x64);
		constexpr COLORREF selectionHotBorder = RGB(0xD8, 0xD8, 0xD8);
	}

	// True when dark mode was successfully activated for this session.
	bool isActive();

	// Resolves the configured theme mode and, if dark, activates comctl32 v6 and
	// dark window theming. Must be called on the main thread before any CS window
	// is created.
	void initialize();

	// Replaces a toolbar's image list with one whose classic silver background is
	// masked out, so icons composite correctly over the dark toolbar fill. No-op
	// when dark mode is inactive.
	void remapToolbarImages(HWND hWndToolbar, HINSTANCE hImageInstance, UINT bitmapId, int imageWidth, int imageHeight);

	// Applies dark painting to an MFC property grid's header and child scrollbar.
	// No-op when dark mode is inactive.
	void themePropertyGrid(HWND hWndGrid, HWND hWndHeader, HWND hWndScrollBar);
}
