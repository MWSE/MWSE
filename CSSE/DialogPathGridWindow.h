#pragma once

namespace se::cs::dialog::path_grid_window {
	constexpr UINT DIALOG_ID = 208;

	// Default IDs.
	constexpr UINT CONTROL_ID_CANCEL_BUTTON = 2;
	constexpr UINT CONTROL_ID_GENERATE_DEFAULT_GRID_BUTTON = 1018;
	constexpr UINT CONTROL_ID_GRANULARITY_COMBO = 1020;
	constexpr UINT CONTROL_ID_SAVE_BUTTON = 1;
	constexpr UINT CONTROL_ID_SAVE_USER_POINTS_CHECK_BUTTON = 1006;

	void installPatches();
}
