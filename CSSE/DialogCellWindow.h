#pragma once

#include "CSDefines.h"

namespace se::cs::dialog::cell_window {
	constexpr UINT DIALOG_ID = 175;

	// Default IDs.
	constexpr UINT CONTROL_ID_CELL_LIST_VIEW = 1155;
	constexpr UINT CONTROL_ID_REFS_LABEL = 1163;
	constexpr UINT CONTROL_ID_REFS_LIST_VIEW = 1156;

	// Custom IDs.
	constexpr UINT CONTROL_ID_FILTER_LABEL = 2000;
	constexpr UINT CONTROL_ID_FILTER_EDIT = 2001;
	constexpr UINT CONTROL_ID_SHOW_MODIFIED_ONLY_BUTTON = 2002;

	void installPatches();
}