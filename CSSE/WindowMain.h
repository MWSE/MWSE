#pragma once

#include "CSDefines.h"

#include "MemoryUtil.h"

namespace se::cs::window::main {
	// Vanilla messages
	constexpr UINT WM_SAVE = 0x40D;
	constexpr UINT WM_FINISH_INITIALIZATION = 0x414;

	// Vanilla commands
	constexpr UINT WM_COMMAND_OPEN = 0x9CD1;
	constexpr UINT WM_COMMAND_SAVE = 0x9CD2;
	constexpr UINT WM_COMMAND_SETTINGS = 0x9CCA;
	constexpr UINT WM_COMMAND_UNDO = 0x9CCF;
	constexpr UINT WM_COMMAND_REDO = 0x9CE4;
	constexpr UINT WM_COMMAND_TOGGLE_GRID_SNAP = 0x9CC6;
	constexpr UINT WM_COMMAND_TOGGLE_ANGLE_SNAP = 0x9CC7;
	constexpr UINT WM_COMMAND_TOGGLE_LANDSCAPE_EDITING = 0x9CFE;
	constexpr UINT WM_COMMAND_TOGGLE_PATHGRID = 0x9D03;
	constexpr UINT WM_COMMAND_TOGGLE_BRIGHTEN_LIGHTS = 0x9D10;
	constexpr UINT WM_COMMAND_TOGGLE_FOG = 0x9D24;
	constexpr UINT WM_COMMAND_OPEN_DIALOGUE_MENU = 0x9D34;
	constexpr UINT WM_COMMAND_OPEN_SEARCH_AND_REPLACE = 0x9CF3;
	constexpr UINT WM_COMMAND_OPEN_SCRIPTS_MENU = 0x9CE3;
	constexpr UINT WM_COMMAND_OPEN_SOUND_MENU = 0x9D35;

	// Extended commands
	constexpr UINT WM_COMMAND_TEST_IN_GAME_MORROWIND = 0xA000;
	constexpr UINT WM_COMMAND_TEST_IN_GAME_OPENMW = 0xA001;

	// CSSE added menu
	constexpr UINT MENU_ID_CSSE_SETTINGS = 0x4000;
	constexpr UINT MENU_ID_CSSE_TEST_ENVIRONMENT = 0x4001;
	constexpr UINT MENU_ID_CSSE_ABOUT = 0x4002;
	constexpr UINT MENU_ID_VIEW_LAYERS_WINDOW = 0x4003;

	// Global variables.
	using hInstance = memory::ExternalGlobal<HINSTANCE, 0x6CE930>;
	using ghWnd = memory::ExternalGlobal<HWND, 0x6CE934>;
	using ghToolbar = memory::ExternalGlobal<HWND, 0x6CE970>;

	using gActiveFileModified = memory::ExternalGlobal<bool, 0x6CEA34>;

	HWND showObjectEditWindow(BaseObject* object);

	void createAutosave();

	void installPatches();
}