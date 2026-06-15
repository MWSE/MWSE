#pragma once

namespace se::cs::darkmode {
	// The fixed dark palette. Highlight colors for modified/deleted rows remain
	// configurable through Settings_t::ColorTheme.
	namespace palette {
		constexpr COLORREF background = RGB(0x2B, 0x2B, 0x2B);
		constexpr COLORREF workspace = RGB(0x14, 0x14, 0x14);
		constexpr COLORREF surface = RGB(0x17, 0x17, 0x17);
		constexpr COLORREF control = RGB(0x3C, 0x3C, 0x3C);
		constexpr COLORREF controlHot = RGB(0x55, 0x55, 0x55);
		constexpr COLORREF text = RGB(0xD0, 0xD0, 0xD0);
		constexpr COLORREF textDisabled = RGB(0x83, 0x83, 0x83);
		constexpr COLORREF border = RGB(0x64, 0x64, 0x64);
		constexpr COLORREF clientEdge = RGB(0x48, 0x48, 0x48);
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

	// Builds a comctl32 v6 image list from a single-row strip bitmap (imageCount
	// cells of imageWidth). When hasAlpha the bitmap is added as a premultiplied
	// 32bpp image with per-pixel alpha; otherwise maskColor is keyed out. Built
	// through v6 so alpha is honored — CSSE's static comctl32 imports bind to
	// v5.82, whose image lists composite alpha images opaquely (transparent
	// pixels go black). Returns nullptr on failure.
	HIMAGELIST buildImageList(HBITMAP bitmap, int imageWidth, int imageHeight, int imageCount, int grow, bool hasAlpha, COLORREF maskColor);

	// Applies dark painting to an MFC property grid's header and child scrollbar.
	// No-op when dark mode is inactive.
	void themePropertyGrid(HWND hWndGrid, HWND hWndHeader, HWND hWndScrollBar);
}
