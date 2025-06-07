#pragma once

#include "CSDefines.h"

namespace se::cs::dialog::use_report_window {
	constexpr UINT DIALOG_ID = 220;

	// Default IDs.
	constexpr UINT CONTROL_ID_USED_BY_OBJECTS_LIST_VIEW = 1637;
	constexpr UINT CONTROL_ID_USED_IN_CELLS_LIST_VIEW = 1638;

	struct UserData {
		BaseObject* reportingObject; // 0x0
		int unknown_0x4;
		int unknown_0x8;
		int unknown_0xC;
	};
	static_assert(sizeof(UserData) == 0x10, "UserData for the use report window failed size validation");

	void showUseReport(const BaseObject* object);

	void installPatches();
}
