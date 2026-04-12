#pragma once

namespace se::cs::theme {
	/// Returns true if UI theming is currently active (enabled in settings with a valid palette).
	bool isEnabled();

	/// Returns a cached HBRUSH for the given COLORREF. Brushes are lazily created
	/// and destroyed in bulk on theme change or shutdown.
	HBRUSH getCachedBrush(COLORREF color);

	/// Re-applies the current theme settings to every window on the current thread.
	void refresh();

	/// Re-applies the current theme settings to a specific window and its children.
	void refreshWindow(HWND hWnd);

	/// Loads a toolbar bitmap resource and creates a theme-aware variant when needed.
	HBITMAP loadThemedBitmap(HINSTANCE instance, UINT bitmapResourceId);

	/// Replaces a toolbar's image list with a theme-aware bitmap strip.
	void applyToolbarBitmap(HWND hWndToolbar, HINSTANCE instance, UINT bitmapResourceId, int bitmapWidth, int bitmapHeight, int bitmapCount);

	/// Install the global CBT hook on the current thread. Must be called early in
	/// CSSE::InstallPatches(), before any dialog windows are created.
	void install();

	/// Unhook the CBT hook and destroy all cached brushes. Called from CSSE::ExitInstance().
	void shutdown();
}
