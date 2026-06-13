#pragma once

namespace se::cs::iconoverride {
	// Loads a loose-file replacement for a bitmap resource as a 32bpp DIB
	// section, or nullptr when none exists. outHasAlpha reports a PNG source
	// (premultiplied alpha); BMP sources keep their mask-color convention.
	HBITMAP loadBitmapOverride(HINSTANCE hInstance, UINT resourceId, bool& outHasAlpha);

	// True when any Data Files check mark icon has an authored override, which
	// disables the black-stroke recolor hack in DarkMode.
	bool hasDataFilesCheckOverrides();

	// Redirects the CS import table so icons and bitmaps can be overridden by
	// loose files under Data Files\MWSE\csse\icons\dark. Must run after
	// darkmode::initialize() and before WinMain; no-op when dark mode is
	// inactive, keeping light mode byte-identical to vanilla.
	void initialize();
}
