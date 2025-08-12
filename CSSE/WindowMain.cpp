#include "WindowMain.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "WinUIUtil.h"

#include "Settings.h"

#include "CSArchive.h"
#include "CSBaseObject.h"
#include "CSDataHandler.h"
#include "CSGameFile.h"
#include "CSGameSetting.h"
#include "CSRecordHandler.h"
#include "CSReference.h"
#include "CSScript.h"

#include "NICamera.h"
#include "NIVector3.h"

#include "DialogRenderWindow.h"
#include "DialogObjectWindow.h"

#include "RenderWindowSceneGraphController.h"
#include "RenderWindowWidgets.h"

#include "DialogAboutCSSE.h"
#include "DialogCSSESettings.h"

#include "MathUtil.h"
#include "PathUtil.h"

#include "CSSE.h"
#include "resource.h"

#include "DialogProcContext.h"

namespace se::cs::window::main {

	struct ObjectEditLParam {
		ObjectType::ObjectType objectType; // 0x0
		BaseObject* object; // 0x4
		Reference* reference; // 0x8
		Reference::ReferenceData* referenceData; // 0xC
	};

	HWND showComboBasedEditWindow(const BaseObject* object, HWND hWnd, LPCSTR lpTemplateName, DLGPROC dlgProc, int comboBoxId) {
		// Create new window if needed.
		if (hWnd == NULL) {
			hWnd = CreateDialogParamA(hInstance::get(), lpTemplateName, ghWnd::get(), dlgProc, 0);
			if (hWnd == NULL) {
				return NULL;
			}
		}
		// Show existing window.
		else if (GetActiveWindow() != hWnd) {
			SetActiveWindow(hWnd);
		}

		// Select the searched for object.
		const auto hDlgCombo = GetDlgItem(hWnd, comboBoxId);
		const auto count = ComboBox_GetCount(hDlgCombo);
		for (auto i = 0; i < count; ++i) {
			const auto birthsign = (BaseObject*)ComboBox_GetItemData(hDlgCombo, i);
			if (birthsign == object) {
				winui::ComboBox_SetCurSelEx(hDlgCombo, i);
				break;
			}
		}

		return hWnd;
	}

	HWND showBirthsignEditWindow(BaseObject* object) {
		using ghWndDialogBirthsigns = memory::ExternalGlobal<HWND, 0x6CE9A0>;
		return showComboBasedEditWindow(object, ghWndDialogBirthsigns::get(), (LPCSTR)0xDF, (DLGPROC)0x402EA5, 1028);
	}

	HWND showClassEditWindow(BaseObject* object) {
		using ghWndDialogClasses = memory::ExternalGlobal<HWND, 0x6CE97C>;
		return showComboBasedEditWindow(object, ghWndDialogClasses::get(), (LPCSTR)0xA0, (DLGPROC)0x402036, 1158);
	}

	HWND showFactionEditWindow(BaseObject* object) {
		return showComboBasedEditWindow(object, NULL, (LPCSTR)0x9D, (DLGPROC)0x4034E5, 1168);
	}

	HWND showRaceEditWindow(BaseObject* object) {
		using ghWndDialogRaces = memory::ExternalGlobal<HWND, 0x6CE978>;
		return showComboBasedEditWindow(object, ghWndDialogRaces::get(), (LPCSTR)0x9F, (DLGPROC)0x403BFC, 1083);
	}

	void UpdateLoadedFilesInTestEnvironment() {
		const auto recordHandler = DataHandler::get()->recordHandler;
		settings.test_environment.game_files.clear();
		for (const auto& gameFile : *recordHandler->availableDataFiles) {
			if (gameFile->getToLoadFlag()) {
				settings.test_environment.game_files.push_back(gameFile->fileName);
			}
		}
	}

	void LaunchMorrowind() {
		// Update the environment data in settings.
		UpdateLoadedFilesInTestEnvironment();

		// Update the file on disk so Morrowind can see the update.
		settings.save();

		// Startup Morrowind.
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION pi = {};
		constexpr auto commandLine = "Morrowind.exe --fromCSSE";
		if (CreateProcessA("Morrowind.exe", (LPSTR)commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
			log::stream << "Running Morrowind with command line: " << commandLine << std::endl;
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else {
			const auto error = GetLastError();
			log::stream << "[ERROR] Failed to run Morrowind with command line: " << commandLine << std::endl;
			log::stream << "  Process path: " << (std::filesystem::current_path() / "Morrowind.exe").string() << std::endl;
			log::stream << "  Error code: 0x" << std::hex << error << std::dec << ": " << std::system_category().message(error) << std::endl;
		}
	}

	void SetupOpenMWUserFolder() {
		const auto realUserPath = path::openmw::getConfigPath();

		// Ensure user folder is created.
		const auto csseUserPath = path::openmw::getTemporaryConfigPath();
		std::filesystem::create_directories(csseUserPath);

		// Copy the settings.cfg file if it doesn't already exist.
		if (!std::filesystem::exists(csseUserPath / "settings.cfg") && std::filesystem::exists(realUserPath / "settings.cfg")) {
			std::filesystem::copy_file(realUserPath / "settings.cfg", csseUserPath / "settings.cfg");
		}
	}

	void UpdateOpenMWConfig() {
		std::ofstream cfg(path::openmw::getTemporaryConfigPath() / "openmw.cfg");
		if (!cfg.is_open()) {
			return;
		}

		const auto& environment = settings.test_environment;
		if (!environment.load_save_openmw.empty() || environment.start_new_game) {
			cfg << "skip-menu=1" << std::endl;
		}
		cfg << "encoding=win1252" << std::endl;
		cfg << "data=" << path::getDataFilesPath().string() << std::endl;

		// Build a stack of archives to display.
		std::stack<std::string> archives;
		auto archive = gBSALoader::get()->lastLoadedArchive;
		while (archive) {
			constexpr auto PREFIX_SIZE = sizeof("Data Files\\");
			archives.push(&archive->path[PREFIX_SIZE - 1]);
			archive = archive->nextArchive;
		}
		while (!archives.empty()) {
			cfg << "fallback-archive=" << archives.top() << std::endl;
			archives.pop();
		}

		for (const auto& file : settings.test_environment.game_files) {
			cfg << "content=" << file << std::endl;
		}

		cfg.close();
	}

	void UpdateOpenMWScriptFile() {
		std::ofstream mwscript(path::openmw::getTemporaryConfigPath() / "startup.txt");
		if (!mwscript.is_open()) {
			return;
		}

		const auto& environment = settings.test_environment;

		if (environment.start_new_game) {
			if (environment.starting_cell != "") {
				mwscript << "player->PositionCell "
					<< environment.position[0] << " "
					<< environment.position[1] << " "
					<< environment.position[2] << " "
					<< math::radiansToDegrees(environment.orientation[2]) << " "
					<< "\"" << environment.starting_cell << "\"" << std::endl;
			}
			else {
				mwscript << "player->Position "
					<< environment.position[0] << " "
					<< environment.position[1] << " "
					<< environment.position[2] << " "
					<< math::radiansToDegrees(environment.orientation[2]) << std::endl;
			}
		}

		for (const auto& [item, count] : environment.inventory) {
			mwscript << "player->AddItem \"" << item << "\" " << count << std::endl;
		}

		for (const auto& spell : environment.spells) {
			mwscript << "player->AddSpell \"" << spell << "\"" << std::endl;
		}

		for (const auto& [journal, stage] : environment.journal) {
			mwscript << "Journal \"" << journal << "\" " << stage << std::endl;
		}

		for (const auto& topic : environment.topics) {
			mwscript << "AddTopic \"" << topic << "\"" << std::endl;
		}

		for (const auto& [variable, value] : environment.globals) {
			mwscript << "set " << variable << " to " << value << std::endl;
		}

		mwscript.close();
	}

	void LaunchOpenMW() {
		// Update the environment data in settings.
		UpdateLoadedFilesInTestEnvironment();
		settings.save();

		// Update the script to have OpenMW run based on the environment.
		const auto tempPath = path::openmw::getTemporaryConfigPath();
		SetupOpenMWUserFolder();
		UpdateOpenMWConfig();
		UpdateOpenMWScriptFile();

		// Verify installation.
		if (settings.openmw.location.empty()) {
			MessageBoxA(NULL, "No OpenMW location found.", "Invalid Settings", MB_ICONERROR | MB_OK);
			return;
		}
		const auto path = settings.openmw.location + "\\openmw.exe";
		if (!std::filesystem::exists(path)) {
			MessageBoxA(NULL, "Invalid OpenMW location.", "Executable Not Found", MB_ICONERROR | MB_OK);
			return;
		}

		std::stringstream ss;
		ss << "openmw.exe";
		ss << " --replace=config --config=\"" << tempPath.string() << "\"";
		if (!settings.test_environment.load_save_openmw.empty()) {
			ss << " --load-savegame \"" << (path::openmw::getSavePath() / settings.test_environment.load_save_openmw).string() << "\"";
		}
		ss << " --script-run \"" << (tempPath / "startup.txt").string() << "\"";

		STARTUPINFO si = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION pi = {};
		const auto commandLine = ss.str();
		if (CreateProcessA(path.c_str(), (LPSTR)commandLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
			log::stream << "Running OpenMW with command line: " << commandLine << std::endl;
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else {
			const auto error = GetLastError();
			log::stream << "[ERROR] Failed to run OpenMW with command line: " << commandLine << std::endl;
			log::stream << "  Process path: " << path << std::endl;
			log::stream << "  Error code: 0x" << std::hex << error << std::dec << ": " << std::system_category().message(error) << std::endl;
		}
	}

	struct ScriptWindowUserData {
		int unknown_0x0;
		int unknown_0x4;
		int unknown_0x8;
		int unknown_0xC;
		int unknown_0x10;
		int unknown_0x14;
		int unknown_0x18;
		int unknown_0x1C;
		Script* script;
		Script* toOpenScript;
		bool unknown_0x28;
		bool unknown_0x29;
		HMENU hMenu; // 0x2C
		HWND hRichTextEdit; // 0x30
		HWND hStatusWindow; // 0x34
		HWND hToolbar; // 0x38
		HWND hCallingDialog; // 0x38
		int lineFromChar;
		int lineCount;
		bool blockProcEvents;
	};
	static_assert(sizeof(ScriptWindowUserData) == 0x4C, "ScriptWindowUserData failed size validation");

	HWND showScriptEditWindow(BaseObject* object) {
		auto hWnd = CreateDialogParamA(hInstance::get(), (LPCSTR)0xBC, ghWnd::get(), (DLGPROC)0x4015F5, 0);
		auto userData = (ScriptWindowUserData*)GetWindowLongA(hWnd, GWL_USERDATA);

		// Set the current script.
		auto script = static_cast<Script*>(object);
		userData->script = script;

		// Load the text.
		SetWindowTextA(userData->hRichTextEdit, script->text);

		// Set the window title.
		char buffer[64] = {};
		if (script->getModified()) {
			sprintf_s(buffer, "Script Edit - [%s *]", script->getObjectID());
		}
		else {
			sprintf_s(buffer, "Script Edit - [%s]", script->getObjectID());
		}
		SetWindowTextA(hWnd, buffer);

		if (script->getDeleted()) {
			ShowWindow(userData->hRichTextEdit, 5);
			EnableWindow(userData->hRichTextEdit, FALSE);
		}
		else {
			EnableWindow(userData->hRichTextEdit, TRUE);
		}

		SendMessageA(userData->hRichTextEdit, EM_SETMODIFY, 0, 0);
		userData->unknown_0x29 = false;

		return hWnd;
	}

	HWND showDefaultObjectEditWindow(BaseObject* object) {
		Reference* reference = nullptr;
		if (object->objectType == ObjectType::Reference) {
			reference = static_cast<Reference*>(object);
			object = reference->baseObject;
		}

		ObjectEditLParam lParam = {};
		lParam.objectType = object->objectType;
		lParam.object = object;
		lParam.reference = reference;
		if (reference) {
			lParam.referenceData = &reference->referenceData;
		}

		DLGPROC proc = (DLGPROC)0x402F9A;
		switch (object->objectType) {
		case ObjectType::Alchemy:
			proc = (DLGPROC)0x4035BC;
			break;
		case ObjectType::Creature:
			proc = (DLGPROC)0x40132F;
			break;
		case ObjectType::Enchantment:
			proc = (DLGPROC)0x404912;
			break;
		case ObjectType::LeveledCreature:
			proc = (DLGPROC)0x402C3E;
			break;
		case ObjectType::LeveledItem:
			proc = (DLGPROC)0x401D66;
			break;
		case ObjectType::NPC:
			proc = (DLGPROC)0x40313E;
			break;
		case ObjectType::Spell:
			proc = (DLGPROC)0x401299;
			break;
		}

		auto tab = dialog::object_window::getTabForObjectType(object->objectType);
		if (tab == -1) {
			return NULL;
		}

		const auto objectWinTypeForTab = (LPCSTR*)0x69460C;

		return CreateDialogParamA(hInstance::get(), objectWinTypeForTab[tab], ghWnd::get(), proc, LPARAM(&lParam));
	}

	HWND showObjectEditWindow(BaseObject* object) {
		switch (object->objectType) {
		case ObjectType::Birthsign:
			return showBirthsignEditWindow(object);
		case ObjectType::Class:
			return showClassEditWindow(object);
		case ObjectType::Faction:
			return showFactionEditWindow(object);
		case ObjectType::GameSetting:
			return NULL;
		case ObjectType::Race:
			return showRaceEditWindow(object);
		case ObjectType::Script:
			return showScriptEditWindow(object);
		default:
			return showDefaultObjectEditWindow(object);
		}
	}

	const auto TES3CS_CreateAutosave = reinterpret_cast<void(__stdcall*)()>(0x401ED3);
	void createAutosave() {
		TES3CS_CreateAutosave();
	}

	//
	// Patch: Throttle UI status updates.
	//

	static auto last2ndClassUpdateTime = std::chrono::milliseconds::zero();
	const auto TES3CS_UpdateStatusMessage = reinterpret_cast<void(__cdecl*)(WPARAM, LPARAM)>(0x46E680);
	void __cdecl PatchThrottleMessageUpdate(WPARAM type, LPARAM lParam) {
		if (type == 2) {
			const auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
			const auto msSinceLastUpdate = now - last2ndClassUpdateTime;
			if (msSinceLastUpdate.count() < 20) {
				return;
			}
			last2ndClassUpdateTime = now;
		}
		else {
			last2ndClassUpdateTime = std::chrono::milliseconds::zero();
		}
		TES3CS_UpdateStatusMessage(type, lParam);
	}

	//
	// Patch: Enable QuickStart cell loading.
	//

	bool isQuickStarting = false;

	bool __cdecl PatchEnableQuickStartCellLoading(bool a1) {
		const auto existingFunction = reinterpret_cast<bool(__cdecl*)(bool)>(0x4033FF);
		auto result = existingFunction(a1);

		if (!isQuickStarting) {
			return result;
		}

		auto dataHandler = DataHandler::get();
		auto renderController = dialog::render_window::RenderController::get();

		// Check to see if we're loading a cell.
		if (settings.quickstart.load_cell) {
			// Load position from settings. We need to shift down by 16,384 units because of the weird offset in the function.
			const auto& qsPos = settings.quickstart.position;
			NI::Vector3 position(qsPos[0], qsPos[1], qsPos[2]);
			position.z -= 16384.0f;

			// Setup a specific interior cell if needed.
			const auto& cellID = settings.quickstart.cell;
			if (!cellID.empty()) {
				auto cell = dataHandler->recordHandler->getCellByID(cellID.c_str());
				if (cell == nullptr) {
					return result;
				}

				const auto setToLoadCell = reinterpret_cast<bool(__thiscall*)(DataHandler*, Cell*, NI::Vector3*)>(0x4A1370);
				setToLoadCell(dataHandler, cell, &position);
			}

			// Actually do our load.
			const auto loadCell = reinterpret_cast<bool(__cdecl*)(NI::Vector3*, Reference*)>(0x469B40);
			loadCell(&position, nullptr);

			// Setup camera.
			const auto& qsRot = settings.quickstart.orientation;
			NI::Matrix33 rotationMatrix;
			rotationMatrix.fromEulerXYZ(qsRot[0], qsRot[1], qsRot[2]);
			renderController->node->setLocalRotationMatrix(&rotationMatrix);
			renderController->node->update();
		}

		isQuickStarting = false;

		return result;
	}

	//
	// Patch: Use CSSE.dll's toolbar bitmap.
	//

	HWND __stdcall PatchReplaceToolbarBitmap(HWND hWnd, DWORD ws, UINT wID, int nBitmaps, HINSTANCE hBMInst, UINT_PTR wBMID, LPCTBBUTTON lpButtons, int iNumButtons, int dxButton, int dyButton, int dxBitmap, int dyBitmap, UINT uStructSize) {
		hBMInst = application.m_hInstance;
		wBMID = IDB_MAIN_TOOLBAR;

		return CreateToolbarEx(hWnd, ws, wID, nBitmaps, hBMInst, wBMID, lpButtons, iNumButtons, dxButton, dyButton, dxBitmap, dyBitmap, uStructSize);
	}

	//
	// Patch: Extend window messages.
	//

	void setupQuickStart() {
		char* commandLineFile = (char*)0x6CE6CC;

		// Skip any initialization if the preview window is active.
		auto renderController = dialog::render_window::RenderController::get();
		if (renderController->node == nullptr) {
			commandLineFile[0] = '\0';
			return;
		}

		// Skip any QuickStart usage if we were given a file to load.
		if (*commandLineFile != '\0') {
			return;
		}

		// We don't have to do anything else if we have it disabled.
		if (!settings.quickstart.enabled || settings.quickstart.data_files.empty()) {
			return;
		}

		auto dataHandler = DataHandler::get();
		auto recordHandler = dataHandler->recordHandler;

		std::unordered_set<std::string> toLoadSet;
		for (const auto& master : settings.quickstart.data_files) {
			toLoadSet.insert(master);
		}

		// Flag any game files as marked to load.
		for (auto itt = recordHandler->availableDataFiles->head; itt; itt = itt->next) {
			auto gameFile = itt->data;
			if (toLoadSet.find(gameFile->fileName) != toLoadSet.end()) {
				gameFile->setToLoadFlag(true);
			}
		}

		std::string mainFileToLoad = settings.quickstart.active_file;
		if (mainFileToLoad.empty()) {
			mainFileToLoad = settings.quickstart.data_files.back();
		}
		strncpy_s(commandLineFile, 0x224, mainFileToLoad.c_str(), mainFileToLoad.length());

		isQuickStarting = true;
	}

	HMENU createExtenderMenu() {
		auto menu = CreateMenu();

		AppendMenuA(menu, MF_STRING, MENU_ID_CSSE_SETTINGS, "&Settings");
		//AppendMenuA(menu, MF_STRING, MENU_ID_CSSE_TEST_ENVIRONMENT, "&Test Environment");
		AppendMenuA(menu, MF_SEPARATOR, NULL, NULL);
		AppendMenuA(menu, MF_STRING, MENU_ID_CSSE_ABOUT, "&About");

		return menu;
	}

	void PatchDialogProc_BeforeFinishInitialization(DialogProcContext& context) {
		// Handle quickstart.
		setupQuickStart();
	}

	void showSettingsDialog(HWND hParent) {
		DialogCSSESettings dialog;
		dialog.DoModal();
	}

	void showTestEnvironmentDialog(HWND hParent) {

	}

	void showAboutDialog(HWND hParent) {
		DialogAboutCSSE dialog;
		dialog.DoModal();
	}

	BOOL __stdcall PatchSaveESPGetSaveFileName(OPENFILENAMEA* ofn) {
		// WINE compatibility shim for the save plugin file dialog.
		// Provide a dummy file path buffer so that WINE will fill in the OFN struct without exiting early.
		char tmp[MAX_PATH];
		memset(tmp, 0, sizeof(tmp));

		ofn->lpstrFile = tmp;
		ofn->nMaxFile = sizeof(tmp);
		auto result = GetSaveFileNameA(ofn);
		ofn->lpstrFile = nullptr;
		ofn->nMaxFile = 0;
		return result;
	}

	void PatchDialogProc_BeforeCommand(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		switch (context.getCommandControlIdentifier()) {
		case MENU_ID_CSSE_SETTINGS:
			showSettingsDialog(hWnd);
			break;
		case MENU_ID_CSSE_TEST_ENVIRONMENT:
			showTestEnvironmentDialog(hWnd);
			break;
		case MENU_ID_CSSE_ABOUT:
			showAboutDialog(hWnd);
			break;
		}
	}

	void PatchDialogProc_BeforeNotify_TooltipRightClick_TestInMorrowind_NewGame(DialogProcContext& context) {
		settings.test_environment.start_new_game = true;
		settings.test_environment.load_save_morrowind = "";

		const auto hWnd = context.getWindowHandle();
		SendMessageA(hWnd, WM_COMMAND, WM_COMMAND_TEST_IN_GAME_MORROWIND, NULL);
	}

	void PatchDialogProc_BeforeNotify_TooltipRightClick_TestInMorrowind_SelectSave(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		OPENFILENAME ofn = {};
		TCHAR szFile[260] = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = _T("Elder Scroll Saves (*.ess)\0*.ess\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = LPSTR("Select save file");
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (winui::GetOpenFileNameWithoutDirChangeA(&ofn) != TRUE) {
			return;
		}

		settings.test_environment.start_new_game = false;
		settings.test_environment.load_save_morrowind = std::filesystem::path(ofn.lpstrFile).filename().string();

		SendMessageA(hWnd, WM_COMMAND, WM_COMMAND_TEST_IN_GAME_MORROWIND, NULL);
	}

	void PatchDialogProc_BeforeNotify_TooltipRightClick_TestInMorrowind_MainMenu(DialogProcContext& context) {
		settings.test_environment.start_new_game = false;
		settings.test_environment.load_save_morrowind = "";
		
		const auto hWnd = context.getWindowHandle();
		SendMessageA(hWnd, WM_COMMAND, WM_COMMAND_TEST_IN_GAME_MORROWIND, NULL);
	}

	void PatchDialogProc_BeforeNotify_TooltipRightClick_TestInMorrowind(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();

		auto menu = CreatePopupMenu();
		if (menu == NULL) {
			return;
		}

		enum ContextMenuId {
			RESERVED_ERROR,
			RESERVED_NO_CALLBACK,
			TEST_NEW_GAME,
			TEST_LOAD_GAME,
			TEST_TO_MAIN_MENU,
		};

		MENUITEMINFO menuItem = {};
		menuItem.cbSize = sizeof(MENUITEMINFO);
		unsigned int index = 0;

		menuItem.wID = TEST_NEW_GAME;
		menuItem.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID;
		menuItem.fType = MFT_STRING;
		menuItem.dwTypeData = (LPSTR)"Test &new game at chosen position";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		menuItem.wID = TEST_LOAD_GAME;
		menuItem.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID;
		menuItem.fType = MFT_STRING;
		menuItem.dwTypeData = (LPSTR)"Test &saved game";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		menuItem.wID = TEST_TO_MAIN_MENU;
		menuItem.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID;
		menuItem.fType = MFT_STRING;
		menuItem.dwTypeData = (LPSTR)"Test normally from &main menu";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		POINT p;
		GetCursorPos(&p);

		auto result = TrackPopupMenuEx(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_NOANIMATION | TPM_VERTICAL, p.x, p.y, hWnd, NULL);
		switch (result) {
		case RESERVED_ERROR:
			break;
		case TEST_NEW_GAME:
			PatchDialogProc_BeforeNotify_TooltipRightClick_TestInMorrowind_NewGame(context);
			break;
		case TEST_LOAD_GAME:
			PatchDialogProc_BeforeNotify_TooltipRightClick_TestInMorrowind_SelectSave(context);
			break;
		case TEST_TO_MAIN_MENU:
			PatchDialogProc_BeforeNotify_TooltipRightClick_TestInMorrowind_MainMenu(context);
			break;
		}

		// Cleanup our menus.
		DestroyMenu(menu);

		context.setResult(TRUE);
	}

	void PatchDialogProc_BeforeNotify_TooltipRightClick_TestInOpenMW_NewGame(DialogProcContext& context) {
		settings.test_environment.start_new_game = true;
		settings.test_environment.load_save_openmw = "";

		const auto hWnd = context.getWindowHandle();
		SendMessageA(hWnd, WM_COMMAND, WM_COMMAND_TEST_IN_GAME_OPENMW, NULL);
	}

	void PatchDialogProc_BeforeNotify_TooltipRightClick_TestInOpenMW_SelectSave(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		OPENFILENAME ofn = {};
		TCHAR szFile[260] = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = _T("OpenMW saves (*.omwsave)\0*.omwsave\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = LPSTR("Select save file");
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		std::filesystem::current_path(path::openmw::getSavePath());
		const auto currentDir = std::filesystem::current_path();
		const auto exists = std::filesystem::exists(currentDir);
		if (GetOpenFileNameA(&ofn) != TRUE) {
			std::filesystem::current_path(path::getInstallPath());
			return;
		}
		std::filesystem::current_path(path::getInstallPath());

		settings.test_environment.start_new_game = false;

		const auto selectedFile = std::filesystem::canonical(ofn.lpstrFile);
		settings.test_environment.load_save_openmw = (selectedFile.parent_path().filename() / selectedFile.filename()).string();

		SendMessageA(hWnd, WM_COMMAND, WM_COMMAND_TEST_IN_GAME_OPENMW, NULL);
	}

	void PatchDialogProc_BeforeNotify_TooltipRightClick_TestInOpenMW_MainMenu(DialogProcContext& context) {
		settings.test_environment.start_new_game = false;
		settings.test_environment.load_save_openmw = "";

		const auto hWnd = context.getWindowHandle();
		SendMessageA(hWnd, WM_COMMAND, WM_COMMAND_TEST_IN_GAME_OPENMW, NULL);
	}

	void PatchDialogProc_BeforeNotify_TooltipRightClick_TestInOpenMW(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		auto menu = CreatePopupMenu();
		if (menu == NULL) {
			return;
		}

		enum ContextMenuId {
			RESERVED_ERROR,
			RESERVED_NO_CALLBACK,
			TEST_NEW_GAME,
			TEST_LOAD_GAME,
			TEST_TO_MAIN_MENU,
		};

		MENUITEMINFO menuItem = {};
		menuItem.cbSize = sizeof(MENUITEMINFO);
		unsigned int index = 0;

		menuItem.wID = TEST_NEW_GAME;
		menuItem.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID;
		menuItem.fType = MFT_STRING;
		menuItem.dwTypeData = (LPSTR)"Test &new game at chosen position";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		menuItem.wID = TEST_LOAD_GAME;
		menuItem.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID;
		menuItem.fType = MFT_STRING;
		menuItem.dwTypeData = (LPSTR)"Test &saved game";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		menuItem.wID = TEST_TO_MAIN_MENU;
		menuItem.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID;
		menuItem.fType = MFT_STRING;
		menuItem.dwTypeData = (LPSTR)"Test normally from &main menu";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		POINT p;
		GetCursorPos(&p);

		auto result = TrackPopupMenuEx(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_NOANIMATION | TPM_VERTICAL, p.x, p.y, hWnd, NULL);
		switch (result) {
		case RESERVED_ERROR:
			break;
		case TEST_NEW_GAME:
			PatchDialogProc_BeforeNotify_TooltipRightClick_TestInOpenMW_NewGame(context);
			break;
		case TEST_LOAD_GAME:
			PatchDialogProc_BeforeNotify_TooltipRightClick_TestInOpenMW_SelectSave(context);
			break;
		case TEST_TO_MAIN_MENU:
			PatchDialogProc_BeforeNotify_TooltipRightClick_TestInOpenMW_MainMenu(context);
			break;
		}

		// Cleanup our menus.
		DestroyMenu(menu);

		context.setResult(TRUE);
	}

	void PatchDialogProc_BeforeNotify_TooltipRightClick(DialogProcContext& context) {
		const auto lParam = context.getNotificationItemActivateData();
		switch (lParam->iItem) {
		case WM_COMMAND_TEST_IN_GAME_MORROWIND:
			PatchDialogProc_BeforeNotify_TooltipRightClick_TestInMorrowind(context);
			break;
		case WM_COMMAND_TEST_IN_GAME_OPENMW:
			PatchDialogProc_BeforeNotify_TooltipRightClick_TestInOpenMW(context);
			break;
		}
	}

	void PatchDialogProc_BeforeNotify_TooltipGetDisplayInfo_TestInMorrowind(DialogProcContext& context) {
		const auto lParam = context.getNotificationTooltipGetDisplayInfo();
		const auto& environment = settings.test_environment;
		if (!environment.load_save_morrowind.empty()) {
			constexpr auto allowance = sizeof(lParam->szText) - sizeof("Test in Morrowind (%s)") - 2u;
			static_assert(allowance < sizeof(lParam->szText) && allowance > 8, "String too long. Need a larger buffer or a smaller tooltip string.");
			if (environment.load_save_morrowind.length() <= allowance) {
				sprintf_s(lParam->szText, "Test in Morrowind (%s)", environment.load_save_morrowind.c_str());
			}
			else {
				std::string clipped = environment.load_save_morrowind;
				clipped.resize(allowance - 3);
				clipped += "...";
				sprintf_s(lParam->szText, "Test in Morrowind (%s)", clipped.c_str());
			}
			context.setResult(TRUE);
		}
		else if (environment.start_new_game) {
			strcpy_s(lParam->szText, "Test in Morrowind (New Game)");
			context.setResult(TRUE);
		}
		else {
			strcpy_s(lParam->szText, "Test in Morrowind (Normal Start)");
			context.setResult(TRUE);
		}
	}

	void PatchDialogProc_BeforeNotify_TooltipGetDisplayInfo_TestInOpenMW(DialogProcContext& context) {
		const auto lParam = context.getNotificationTooltipGetDisplayInfo();
		const auto& environment = settings.test_environment;

		if (!environment.load_save_openmw.empty()) {
			constexpr auto allowance = sizeof(lParam->szText) - sizeof("Test in OpenMW (%s)") - 2u;
			static_assert(allowance < sizeof(lParam->szText) && allowance > 8, "String too long. Need a larger buffer or a smaller tooltip string.");

			// Just get the character name + save name.
			const std::filesystem::path asPath = environment.load_save_openmw;
			std::string clipped = asPath.parent_path().filename().string() + "\\" + asPath.filename().string();
			if (clipped.length() > allowance) {
				clipped.resize(allowance - 3);
				clipped += "...";
			}

			sprintf_s(lParam->szText, "Test in OpenMW (%s)", clipped.c_str());
			context.setResult(TRUE);
		}
		else if (environment.start_new_game) {
			strcpy_s(lParam->szText, "Test in OpenMW (New Game)");
			context.setResult(TRUE);
		}
		else {
			strcpy_s(lParam->szText, "Test in OpenMW (Normal Start)");
			context.setResult(TRUE);
		}
	}

	// Catch tooltip info requests to add custom tooltips to new toolbar buttons.
	void PatchDialogProc_BeforeNotify_TooltipGetDisplayInfo(DialogProcContext& context) {
		const auto lParam = context.getNotificationTooltipGetDisplayInfo();
		switch (lParam->hdr.idFrom) {
		case WM_COMMAND_TEST_IN_GAME_MORROWIND:
			PatchDialogProc_BeforeNotify_TooltipGetDisplayInfo_TestInMorrowind(context);
			break;
		case WM_COMMAND_TEST_IN_GAME_OPENMW:
			PatchDialogProc_BeforeNotify_TooltipGetDisplayInfo_TestInOpenMW(context);
			break;
		}
	}

	void PatchDialogProc_BeforeNotify(DialogProcContext& context) {
		const auto lParam = context.getNotificationData();
		switch (lParam->code) {
		case NM_RCLICK:
			PatchDialogProc_BeforeNotify_TooltipRightClick(context);
			break;
		case TTN_GETDISPINFO:
			PatchDialogProc_BeforeNotify_TooltipGetDisplayInfo(context);
			break;
		}
	}

	void PatchDialogProc_BeforeClose(DialogProcContext& context) {
		auto sgController = dialog::render_window::SceneGraphController::get();
		sgController->clearWidgets();
	}

	void PatchDialogProc_AfterCreate_CreateNewCSSEMenu(DialogProcContext& context) {
		// Add MWSE to the menu.
		const auto hWnd = context.getWindowHandle();
		auto menu = GetMenu(hWnd);
		if (!menu) {
			return;
		}

		// Add the MWSE menu to the system.
		MENUITEMINFO newExtenderMenu = {};
		newExtenderMenu.cbSize = sizeof(MENUITEMINFO);
		newExtenderMenu.fMask = MIIM_STRING | MIIM_SUBMENU;
		newExtenderMenu.hSubMenu = createExtenderMenu();
		newExtenderMenu.dwTypeData = (char*)"C&SSE";
		InsertMenuItemA(menu, 6, TRUE, &newExtenderMenu);
	}

	void PatchDialogProc_AfterCreate_DoBaseToolbarExtensions(DialogProcContext& context) {
		using winui::Toolbar_AddButton;
		using winui::Toolbar_AddSeparator;

		const auto hWndToolbar = ghToolbar::get();

		// Always-there buttons.
		Toolbar_AddSeparator(hWndToolbar);
		Toolbar_AddButton(hWndToolbar, WM_COMMAND_TEST_IN_GAME_MORROWIND, 14);

		// Add OpenMW button we have a valid installation.
		if (!settings.openmw.location.empty()) {
			const auto path = settings.openmw.location + "\\openmw.exe";
			if (std::filesystem::exists(path)) {
				Toolbar_AddButton(hWndToolbar, WM_COMMAND_TEST_IN_GAME_OPENMW, 15);
			}
		}
	}

	void PatchDialogProc_AfterCreate(DialogProcContext& context) {
		PatchDialogProc_AfterCreate_CreateNewCSSEMenu(context);
		PatchDialogProc_AfterCreate_DoBaseToolbarExtensions(context);
	}

	void PatchDialogProc_AfterCommand_ToggleLandscapeEditing(DialogProcContext& context) {
		// Restore focus to the render window.
		SetFocus(memory::ExternalGlobal<HWND, 0x6CE93C>::get());
	}

	static std::optional<UINT> testAfterSave = {};

	void PatchDialogProc_AfterCommand_TestInMorrowind(DialogProcContext& context) {
		if (gActiveFileModified::get()) {
			testAfterSave = WM_COMMAND_TEST_IN_GAME_MORROWIND;
			SendMessageA(context.getWindowHandle(), WM_COMMAND, WM_COMMAND_SAVE, NULL);
		}
		else {
			LaunchMorrowind();
		}
	}

	void PatchDialogProc_AfterCommand_TestInOpenMW(DialogProcContext& context) {
		if (gActiveFileModified::get()) {
			testAfterSave = WM_COMMAND_TEST_IN_GAME_OPENMW;
			SendMessageA(context.getWindowHandle(), WM_COMMAND, WM_COMMAND_SAVE, NULL);
		}
		else {
			LaunchOpenMW();
		}
	}

	void PatchDialogProc_AfterCommand(DialogProcContext& context) {
		switch (context.getCommandControlIdentifier()) {
		case WM_COMMAND_TOGGLE_LANDSCAPE_EDITING:
			PatchDialogProc_AfterCommand_ToggleLandscapeEditing(context);
			break;
		case WM_COMMAND_TEST_IN_GAME_MORROWIND:
			PatchDialogProc_AfterCommand_TestInMorrowind(context);
			break;
		case WM_COMMAND_TEST_IN_GAME_OPENMW:
			PatchDialogProc_AfterCommand_TestInOpenMW(context);
			break;
		}
	}

	void PatchDialogProc_AfterSave(DialogProcContext& context) {
		if (testAfterSave) {
			switch (testAfterSave.value()) {
			case WM_COMMAND_TEST_IN_GAME_MORROWIND:
				LaunchMorrowind();
				break;
			case WM_COMMAND_TEST_IN_GAME_OPENMW:
				LaunchOpenMW();
				break;
			}
			testAfterSave.reset();
		}
	}

	void PatchDialogProc_AfterInitialize(DialogProcContext& context) {
		const auto hWnd = context.getWindowHandle();
		auto statusWindow = FindWindowEx(hWnd, NULL, "msctls_statusbar32", NULL);

		if (!statusWindow) {
			return;
		}

		int partsRightEdgePositions[4] = { 220, 330, 800, -1 };
		SendMessage(statusWindow, SB_SETPARTS, (WPARAM)4, (LPARAM)partsRightEdgePositions);
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		DialogProcContext context(hWnd, msg, wParam, lParam, 0x444590);

		// Handle pre-patches.
		switch (msg) {
		case WM_COMMAND:
			PatchDialogProc_BeforeCommand(context);
			break;
		case WM_NOTIFY:
			PatchDialogProc_BeforeNotify(context);
			break;
		case WM_CLOSE:
			PatchDialogProc_BeforeClose(context);
			break;
		case WM_FINISH_INITIALIZATION:
			PatchDialogProc_BeforeFinishInitialization(context);
			break;
		}

		// Call original function, or return early if we already have a result.
		if (context.hasResult()) {
			return context.getResult();
		}
		else {
			context.callOriginalFunction();
		}

		switch (msg) {
		case WM_CREATE:
			PatchDialogProc_AfterCreate(context);
			break;
		case WM_COMMAND:
			PatchDialogProc_AfterCommand(context);
			break;
		case WM_SAVE:
			PatchDialogProc_AfterSave(context);
			break;
		case WM_FINISH_INITIALIZATION:
			PatchDialogProc_AfterInitialize(context);
			break;
		}

		return context.getResult();
	}

	void installPatches() {
		using memory::genJumpEnforced;
		using memory::genCallEnforced;
		using memory::genCallUnprotected;

		// Patch: Throttle UI status updates.
		genJumpEnforced(0x404881, 0x46E680, reinterpret_cast<DWORD>(PatchThrottleMessageUpdate));

		// Patch: Enable QuickStart cell loading.
		genCallEnforced(0x447B78, 0x4033FF, reinterpret_cast<DWORD>(PatchEnableQuickStartCellLoading));

		// Patch: Use CSSE.dll's toolbar bitmap.
		genCallUnprotected(0x46F97B, reinterpret_cast<DWORD>(PatchReplaceToolbarBitmap), 0x6);

		// Patch: WINE compatibility shim for the save plugin file dialog.
		genCallEnforced(0x4153FD, 0x573296, reinterpret_cast<DWORD>(PatchSaveESPGetSaveFileName));

		// Patch: Extend window messages.
		genJumpEnforced(0x401EF1, 0x444590, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}