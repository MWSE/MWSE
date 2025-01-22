#pragma once

#include "CSDefines.h"

namespace se::cs::dialog::edit_npc_object_window {

	constexpr UINT DIALOG_ID = 148;

	// Default IDs.
	constexpr UINT CONTROL_ID_ADD_ANIMATION_FILE_BUTTON = 1206;
	constexpr UINT CONTROL_ID_AGI_EDIT = 1603;
	constexpr UINT CONTROL_ID_AI_BUTTON = 1205;
	constexpr UINT CONTROL_ID_ANIMATION_BUTTON = 1043;
	constexpr UINT CONTROL_ID_AUTOCALC_BUTTON = 1215;
	constexpr UINT CONTROL_ID_BLOCKED_BUTTON = 1684;
	constexpr UINT CONTROL_ID_BLOOD_COMBO = 1170;
	constexpr UINT CONTROL_ID_CANCELBUTTON = 2;
	constexpr UINT CONTROL_ID_CLASS_COMBO = 1158;
	constexpr UINT CONTROL_ID_CORPSES_PERSIST_BUTTON = 1683;
	constexpr UINT CONTROL_ID_DETAILS_LIST = 1018;
	constexpr UINT CONTROL_ID_DETAILS_TABS = 1111;
	constexpr UINT CONTROL_ID_DIALOGUE_BUTTON = 1207;
	constexpr UINT CONTROL_ID_DISPOSITION_EDIT = 1005;
	constexpr UINT CONTROL_ID_ENCUMBRANCE_STATIC = 1182;
	constexpr UINT CONTROL_ID_END_EDIT = 1605;
	constexpr UINT CONTROL_ID_ESSENTIAL_BUTTON = 1180;
	constexpr UINT CONTROL_ID_FACTION_COMBO = 1168;
	constexpr UINT CONTROL_ID_FACTION_RANK_COMBO = 1169;
	constexpr UINT CONTROL_ID_FATIGUE_EDIT = 1179;
	constexpr UINT CONTROL_ID_FEMALE_BUTTON = 1086;
	constexpr UINT CONTROL_ID_HAIR_LIST = 1187;
	constexpr UINT CONTROL_ID_HEAD_LIST = 1186;
	constexpr UINT CONTROL_ID_HEALTH_EDIT = 1024;
	constexpr UINT CONTROL_ID_ID_EDIT = 1028;
	constexpr UINT CONTROL_ID_INT_EDIT = 1601;
	constexpr UINT CONTROL_ID_LEVEL_EDIT = 1063;
	constexpr UINT CONTROL_ID_LUC_EDIT = 1607;
	constexpr UINT CONTROL_ID_MAGICKA_EDIT = 1178;
	constexpr UINT CONTROL_ID_NAME_EDIT = 1060;
	constexpr UINT CONTROL_ID_OK_BUTTON = 1;
	constexpr UINT CONTROL_ID_PER_EDIT = 1606;
	constexpr UINT CONTROL_ID_RACE_COMBO = 1083;
	constexpr UINT CONTROL_ID_REPUTATION_EDIT = 1006;
	constexpr UINT CONTROL_ID_RESPAWN_BUTTON = 1216;
	constexpr UINT CONTROL_ID_SCRIPT_COMBO = 1226;
	constexpr UINT CONTROL_ID_SCRIPT_EDIT_BUTTON = 1062;
	constexpr UINT CONTROL_ID_SKILLS_LIST = 1087;
	constexpr UINT CONTROL_ID_SPD_EDIT = 1604;
	constexpr UINT CONTROL_ID_STR_EDIT = 1600;
	constexpr UINT CONTROL_ID_WIL_EDIT = 1602;

	struct UserData {
		NPC* newNPC; // 0x0
		NPC* existingNPC; // 0x4
		int unknown_0x8;
		RecordHandler* recordHandler; // 0xC
		HIMAGELIST unknown_0x10;
		int unknown_0x14;
		int unknown_0x18;
		int unknown_0x1C;
		NPC* unknown_0x20;
		int unknown_0x24;
		int unknown_0x28;
		bool unknown_0x2C;
	};
	static_assert(sizeof(UserData) == 0x30, "UserData for the NPC edit window failed size validation");

	void installPatches();
}
