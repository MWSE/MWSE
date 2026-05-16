#pragma once

namespace se::cs::dialog::edit_object_window {

	// Dialog IDs.
	constexpr UINT DIALOG_ID_ACTIVATOR = 147;
	constexpr UINT DIALOG_ID_APPARATUS = 143;
	constexpr UINT DIALOG_ID_ARMOR = 136;
	constexpr UINT DIALOG_ID_BODY_PART = 153;
	constexpr UINT DIALOG_ID_BOOK = 144;
	constexpr UINT DIALOG_ID_CLOTHING = 137;
	constexpr UINT DIALOG_ID_CONTAINER = 145;
	constexpr UINT DIALOG_ID_DOOR = 196;
	constexpr UINT DIALOG_ID_INGREDIENT = 139;
	constexpr UINT DIALOG_ID_LIGHT = 156;
	constexpr UINT DIALOG_ID_LOCKPICK = 142;
	constexpr UINT DIALOG_ID_MISC = 138;
	constexpr UINT DIALOG_ID_PROBE = 146;
	constexpr UINT DIALOG_ID_REPAIR_ITEM = 167;
	constexpr UINT DIALOG_ID_STATIC = 140;
	constexpr UINT DIALOG_ID_WEAPON = 135;

	// Default IDs.
	constexpr UINT CONTROL_ID_ICON_FRAME = 1120;
	constexpr UINT CONTROL_ID_SCRIPT_COMBO = 1226;

	void installPatches();
}
