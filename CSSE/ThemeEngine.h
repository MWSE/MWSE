#pragma once

namespace se::cs::theme {
	/// Returns true if UI theming is currently active (enabled in settings with a valid palette).
	bool isEnabled();

	/// Returns a cached HBRUSH for the given COLORREF. Brushes are lazily created
	/// and destroyed in bulk on theme change or shutdown.
	HBRUSH getCachedBrush(COLORREF color);

	/// Install the global CBT hook on the current thread. Must be called early in
	/// CSSE::InstallPatches(), before any dialog windows are created.
	void install();

	/// Unhook the CBT hook and destroy all cached brushes. Called from CSSE::ExitInstance().
	void shutdown();
}
