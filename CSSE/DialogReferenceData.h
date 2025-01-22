#pragma once

#include "CSDefines.h"

namespace se::cs::dialog::reference_data {

	constexpr UINT DIALOG_ID = 141;

	// Default IDs.
	constexpr UINT CONTROL_ID_APPLY_TO_SELECTION_BUTTON = 1016;
	constexpr UINT CONTROL_ID_EXTRA_DATA_BUTTON = 1006;
	constexpr UINT CONTROL_ID_HEALTH_LEFT_EDIT = 1024;
	constexpr UINT CONTROL_ID_KEY_COMBO = 1064;
	constexpr UINT CONTROL_ID_LOAD_CELL_COMBO = 1066;
	constexpr UINT CONTROL_ID_LOCK_LEVEL_EDIT = 1063;
	constexpr UINT CONTROL_ID_LOCKED_BUTTON = 1009;
	constexpr UINT CONTROL_ID_OWNER_COMBO = 1021;
	constexpr UINT CONTROL_ID_OWNER_VARIABLE_RANK_COMBO = 1022;
	constexpr UINT CONTROL_ID_POSITION_X_EDIT = 1095;
	constexpr UINT CONTROL_ID_POSITION_X_SPIN = 1101;
	constexpr UINT CONTROL_ID_POSITION_X_SPIN_EDIT = 1098;
	constexpr UINT CONTROL_ID_POSITION_Y_EDIT = 1096;
	constexpr UINT CONTROL_ID_POSITION_Y_SPIN = 1102;
	constexpr UINT CONTROL_ID_POSITION_Y_SPIN_EDIT = 1098;
	constexpr UINT CONTROL_ID_POSITION_Z_EDIT = 1097;
	constexpr UINT CONTROL_ID_POSITION_Z_SPIN = 1103;
	constexpr UINT CONTROL_ID_POSITION_Z_SPIN_EDIT = 1099;
	constexpr UINT CONTROL_ID_REFERENCE_BLOCKED_BUTTON = 1007;
	constexpr UINT CONTROL_ID_ROTATION_X_EDIT = 1113;
	constexpr UINT CONTROL_ID_ROTATION_X_SPIN = 1114;
	constexpr UINT CONTROL_ID_ROTATION_X_SPIN_EDIT = 1119;
	constexpr UINT CONTROL_ID_ROTATION_Y_EDIT = 1115;
	constexpr UINT CONTROL_ID_ROTATION_Y_SPIN = 1116;
	constexpr UINT CONTROL_ID_ROTATION_Y_SPIN_EDIT = 1120;
	constexpr UINT CONTROL_ID_ROTATION_Z_EDIT = 1117;
	constexpr UINT CONTROL_ID_ROTATION_Z_SPIN = 1118;
	constexpr UINT CONTROL_ID_ROTATION_Z_SPIN_EDIT = 1121;
	constexpr UINT CONTROL_ID_SCALE_EDIT = 1025;
	constexpr UINT CONTROL_ID_SELECT_MARKER_BUTTON = 1487;
	constexpr UINT CONTROL_ID_SOUL_COMBO = 1197;
	constexpr UINT CONTROL_ID_TELEPORT_BUTTON = 1488;
	constexpr UINT CONTROL_ID_TRAP_COMBO = 1065;

	struct UserData {
		struct LParam {
			DWORD objectType; // 0x0
			Object* object; // 0x4
			Reference* reference; // 0x8
			void* referenceData; // 0xC
		};

		LParam* lParam; // 0x0
		ItemData* itemData; // 0x4
		float scale; // 0x8
		TravelDestination* loadDoorAttachment;
		SecurityAttachmentNode* securityAttachment;
		int unknown_0x14;
		int unknown_0x18;
		int unknown_0x1C;
		int unknown_0x20;
		int unknown_0x24;
		int unknown_0x28;
		BYTE unknown_0x2C;
		BYTE unknown_0x2D;
		BYTE unknown_0x2E;
		BYTE unknown_0x2F;
	};
	static_assert(sizeof(UserData) == 0x30, "ReferenceData dialog UserData failed size validation");
	static_assert(sizeof(UserData::LParam) == 0x10, "ReferenceData dialog UserData failed size validation");

	void installPatches();
}