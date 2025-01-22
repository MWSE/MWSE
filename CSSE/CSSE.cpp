#include "CSSE.h"

#include "LogUtil.h"

#include "CSDialogue.h"
#include "CSDialogueInfo.h"
#include "CSGameFile.h"
#include "CSGameSetting.h"
#include "CSPhysicalObject.h"
#include "CSRecordHandler.h"

#include "NIAVObject.h"
#include "NICamera.h"
#include "NILinesData.h"
#include "NISortAdjustNode.h"

#include "WindowMain.h"

#include "DialogActorAIWindow.h"
#include "DialogCellWindow.h"
#include "DialogDialogueWindow.h"
#include "DialogEditObjectWindow.h"
#include "DialogLandscapeEditSettingsWindow.h"
#include "DialogObjectWindow.h"
#include "DialogPathGridWindow.h"
#include "DialogPreviewWindow.h"
#include "DialogReferenceData.h"
#include "DialogRenderWindow.h"
#include "DialogScriptEditorWindow.h"
#include "DialogScriptListWindow.h"
#include "DialogSearchAndReplaceWindow.h"
#include "DialogTextSearchWindow.h"
#include "DialogUseReportWindow.h"

#include "TextureRenderer.h"

#include "MemoryUtil.h"
#include "PathUtil.h"
#include "StringUtil.h"
#include "WindowsUtil.h"
#include "MetadataUtil.h"

#include "BuildDate.h"
#include "Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace se::cs {
	constexpr auto LOG_SUPPRESSED_WARNINGS = false;
	constexpr auto LOG_NI_MESSAGES = false;

	GameFile* master_Morrowind = nullptr;
	GameFile* master_Tribunal = nullptr;
	GameFile* master_Bloodmoon = nullptr;

	bool isVanillaMaster(GameFile* master) {
		return (master_Morrowind && master == master_Morrowind)
			|| (master_Tribunal && master == master_Tribunal)
			|| (master_Bloodmoon && master == master_Bloodmoon);
	}

	namespace patch {
		const auto TES3_RecordHandler_whoCares = reinterpret_cast<void(__thiscall*)(RecordHandler*)>(0x4041C4);
		void __fastcall findVanillaMasters(RecordHandler* recordHandler) {
			for (auto i = 0; i < recordHandler->activeModCount; ++i) {
				const auto gameFile = recordHandler->activeGameFiles[i];
				if (master_Morrowind == nullptr && string::iequal(gameFile->fileName, "Morrowind.esm")) {
					master_Morrowind = gameFile;
				}
				else if (master_Tribunal == nullptr && string::iequal(gameFile->fileName, "Tribunal.esm")) {
					master_Tribunal = gameFile;
				}
				else if (master_Bloodmoon == nullptr && string::iequal(gameFile->fileName, "Bloodmoon.esm")) {
					master_Bloodmoon = gameFile;
				}
			}

			TES3_RecordHandler_whoCares(recordHandler);
		}

		const auto TES3_GameSetting_SaveGameSetting = reinterpret_cast<bool(__thiscall*)(GameSetting*, GameFile*)>(0x4F9BE0);
		bool __fastcall preventGMSTPollution(GameSetting* gameSetting, DWORD _EDX_, GameFile* file) {
			if (!gameSetting->getModified()) {
				return false;
			}

			return TES3_GameSetting_SaveGameSetting(gameSetting, file);
		}

		void __cdecl suppressDialogueInfoResolveIssues(const char* format, const char* topicId, const char* infoId, const char* text) {
			if constexpr (LOG_SUPPRESSED_WARNINGS) {
				char buffer[1024];
				sprintf_s(buffer, format, topicId, infoId, text);
				log::stream << "Suppressing warning: " << buffer << std::endl;
			}
		}

		void __fastcall suppressDeletedDefaultDialogueTypeChangeWarning(Dialogue* self, DWORD _EDX_, Dialogue* from) {
			// Ignore type changes entirely for deleted data.
			if (self->id == nullptr && from->id == nullptr && self->type != from->type && self->getDeleted() && from->getDeleted()) {
				return;
			}

			// Call overwritten code.
			const auto TES3_Dialogue_LoadOver = reinterpret_cast<void(__thiscall*)(Dialogue*, Dialogue*)>(0x4F3360);
			TES3_Dialogue_LoadOver(self, from);
		}

		const auto ShowDuplicateReferenceWarning = reinterpret_cast<bool(__cdecl*)(const char*, int)>(0x40123A);
		bool __cdecl suppressDuplicateReferenceRemovedWarningForVanillaMasters(const char* message, int referenceCount) {
			if (referenceCount == 1 && master_Tribunal && master_Bloodmoon) {
				const auto cachedValue = se::memory::MemAccess<bool>::Get(0x6D0B6E);
				se::memory::MemAccess<bool>::Set(0x6D0B6E, true);
				auto result = ShowDuplicateReferenceWarning(message, referenceCount);
				se::memory::MemAccess<bool>::Set(0x6D0B6E, cachedValue);
				return result;
			}
			else {
				return ShowDuplicateReferenceWarning(message, referenceCount);
			}
		}

		void __cdecl restoreNiLogMessage(const char* fmt, ...) {
			// Get the calling address.
			byte** asmEBP;
			__asm { mov asmEBP, ebp };
			DWORD callingAddress = DWORD(asmEBP[1] - 0x5);

			va_list args;
			va_start(args, fmt);

			char buffer[2048] = {};
			vsprintf_s(buffer, fmt, args);

			va_end(args);

			log::stream << "[NiLog:0x" << std::hex << callingAddress << std::dec << "] " << buffer;
		}

		void __cdecl restoreNiLogMessageWithNewline(const char* fmt, ...) {
			// Get the calling address.
			byte** asmEBP;
			__asm { mov asmEBP, ebp };
			DWORD callingAddress = DWORD(asmEBP[1] - 0x5);

			va_list args;
			va_start(args, fmt);

			char buffer[2048] = {};
			vsprintf_s(buffer, fmt, args);

			va_end(args);

			log::stream << "[NiLog:0x" << std::hex << callingAddress << std::dec << "] " << buffer << std::endl;
		}

		void __fastcall forceLoadFlagsOnActiveMods(RecordHandler* recordHandler) {
			for (auto i = 0; i < recordHandler->activeModCount; ++i) {
				auto file = recordHandler->activeGameFiles[i];
				if (file->masters == nullptr) {
					const auto cs_GameFile_CreateMasterArray = reinterpret_cast<bool(__thiscall*)(GameFile*, StlList<GameFile*>*, bool)>(0x401D7F);
					cs_GameFile_CreateMasterArray(file, recordHandler->availableDataFiles, true);
				}
			}

			const auto overwrittenFunction = reinterpret_cast<void(__thiscall*)(RecordHandler*)>(0x4016F4);
			overwrittenFunction(recordHandler);
		}

		//
		// Patch: Add deterministic subtree ordering mode to NiSortAdjustNode. Fix crash when cloning with no accumulator.
		//

		const auto NI_SortAdjustNode_Display = reinterpret_cast<void(__thiscall*)(NI::SortAdjustNode*, NI::Camera*)>(0x5CF270);
		const auto NI_ClusterAccumulator_RegisterObject = reinterpret_cast<void(__thiscall*)(NI::Accumulator*, NI::AVObject*)>(0x5BE250);

		void __fastcall NISortAdjustNodeDisplay(NI::SortAdjustNode* node, DWORD unused, NI::Camera* camera) {
			// Add extra sort adjust mode for accumulating a node instead of geom.
			auto accumulator = camera->renderer->accumulator.get();
			if (node->sortingMode == NI::SortAdjustMode::SORTING_ORDERED_SUBTREE_MWSE
				&& accumulator != nullptr
				&& accumulator->isInstanceOfType(NI::RTTIStaticPtr::NiAlphaAccumulator)) {
				NI_ClusterAccumulator_RegisterObject(accumulator, node);
			}
			else {
				NI_SortAdjustNode_Display(node, camera);
			}
		}

		NI::Object* __fastcall NISortAdjustNodeCloneAccumulator(NI::Accumulator* accumulator) {
			// Only call createClone if accumulator exists.
			return accumulator ? accumulator->vTable.asObject->createClone(accumulator) : nullptr;
		}

		//
		// Patch: Prevent rogue files from popping up.
		//

		HANDLE __stdcall CreateRootDirectoryFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
			const auto forcedPath = (path::getInstallPath() / lpFileName).string();
			return CreateFileA(forcedPath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		}

		BOOL __stdcall DeleteRootDirectoryFile(LPCSTR lpFileName) {
			const auto forcedPath = (path::getInstallPath() / lpFileName).string();
			return DeleteFileA(forcedPath.c_str());
		}

		//
		// Patch: Remember last chosen model directory.
		//

		std::optional<std::string> lastModelDirectory = {};

		BOOL __stdcall GetOpenFileNameForModel(LPOPENFILENAMEA openData) {
			// Use the last saved dir.
			if (lastModelDirectory) {
				openData->lpstrInitialDir = lastModelDirectory.value().c_str();
			}

			// Don't dereference symlinks.
			openData->Flags |= OFN_NODEREFERENCELINKS;

			if (!GetOpenFileNameA(openData)) {
				return FALSE;
			}

			lastModelDirectory = std::filesystem::path(openData->lpstrFile).remove_filename().string();

			return TRUE;
		}

		//
		// Patch: Remember last chosen icon directory.
		//

		std::optional<std::string> lastIconDirectory = {};

		BOOL __stdcall GetOpenFileNameForIcon(LPOPENFILENAMEA openData) {
			// Use the last saved dir.
			if (lastIconDirectory) {
				openData->lpstrInitialDir = lastIconDirectory.value().c_str();
			}

			// Don't dereference symlinks.
			openData->Flags |= OFN_NODEREFERENCELINKS;

			if (!GetOpenFileNameA(openData)) {
				return FALSE;
			}

			lastIconDirectory = std::filesystem::path(openData->lpstrFile).remove_filename().string();

			return TRUE;
		}

		//
		// Patch: Add default option for showing all icon types.
		//

		static const auto iconTypes = "Morrowind Icons (*.TGA, *.DDS)\0*.tga;*.dds\0Morrowind TGA Icons (*.TGA)\0*.tga\0Morrowind DDS Icons (*.DDS)\0*.dds\0\0";

		const auto CS_RequestIconFilename = reinterpret_cast<int(__cdecl*)(HWND, const char*, const char*, const char*)>(0x414E10);
		int __cdecl RequestIconFilename(HWND hWnd, const char* currentFilename, const char* successMessage, const char* filter) {
			if (filter == nullptr) {
				filter = iconTypes;
			}
			return CS_RequestIconFilename(hWnd, currentFilename, successMessage, filter);
		}

		//
		// Create minidumps.
		//

		bool isDataSectionNeeded(const WCHAR* pModuleName) {
			// Check parameters.
			if (pModuleName == 0) {
				return false;
			}

			// Extract the module name.
			WCHAR szFileName[_MAX_FNAME] = L"";
			_wsplitpath_s(pModuleName, NULL, NULL, NULL, NULL, szFileName, _MAX_FNAME, NULL, NULL);

			// Compare the name with the list of known names and decide.
			if (_wcsicmp(szFileName, L"Morrowind") == 0) {
				return true;
			}
			else if (_wcsicmp(szFileName, L"ntdll") == 0)
			{
				return true;
			}
			else if (_wcsicmp(szFileName, L"CSSE") == 0)
			{
				return true;
			}

			// Complete.
			return false;
		}

		BOOL CALLBACK miniDumpCallback(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput) {
			BOOL bRet = FALSE;

			// Check parameters 
			if (pInput == 0) {
				return FALSE;
			}
			if (pOutput == 0) {
				return FALSE;
			}

			// Process the callbacks 
			switch (pInput->CallbackType) {
			case IncludeModuleCallback:
			case IncludeThreadCallback:
			case ThreadCallback:
			case ThreadExCallback:
			{
				// Include the thread into the dump 
				bRet = TRUE;
			}
			break;

			case MemoryCallback:
			{
				// We do not include any information here -> return FALSE 
				bRet = FALSE;
			}
			break;

			case ModuleCallback:
			{
				// Does the module have ModuleReferencedByMemory flag set? 
				if (pOutput->ModuleWriteFlags & ModuleWriteDataSeg) {
					if (!isDataSectionNeeded(pInput->Module.FullPath)) {
						pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
					}
				}

				bRet = TRUE;
			}
			break;
			}

			return bRet;
		}

		void CreateMiniDump(EXCEPTION_POINTERS* pep) {
			log::stream << std::endl;
			log::stream << "The Construction Set has crashed! To help improve game stability, post the CSSE_Minidump.dmp and csse.log files to the Morrowind Modding Community #mwse channel on Discord." << std::endl;
			log::stream << "The Morrowind Modding Community Discord can be found at https://discord.me/mwmods" << std::endl;

#ifdef APPVEYOR_BUILD_NUMBER
			log::stream << "Appveyor build: " << APPVEYOR_BUILD_NUMBER << std::endl;
#endif
			log::stream << "Build date: " << CSSE_BUILD_DATE << std::endl;

			// Display the memory usage in the log.
			PROCESS_MEMORY_COUNTERS_EX memCounter = {};
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memCounter, sizeof(memCounter));
			log::stream << "Memory usage: " << std::dec << memCounter.PrivateUsage << " bytes." << std::endl;
			if (memCounter.PrivateUsage > 3650722201) {
				log::stream << "  Memory usage is high. Crash is likely due to running out of memory." << std::endl;
			}

			// Open the file.
			auto hFile = CreateFile("CSSE_MiniDump.dmp", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE)) {
				// Create the minidump.
				MINIDUMP_EXCEPTION_INFORMATION mdei = {};

				mdei.ThreadId = GetCurrentThreadId();
				mdei.ExceptionPointers = pep;
				mdei.ClientPointers = FALSE;

				MINIDUMP_CALLBACK_INFORMATION mci = {};

				mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)miniDumpCallback;
				mci.CallbackParam = 0;

				auto mdt = (MINIDUMP_TYPE)(MiniDumpWithDataSegs |
					MiniDumpWithHandleData |
					MiniDumpWithFullMemoryInfo |
					MiniDumpWithThreadInfo |
					MiniDumpWithUnloadedModules);

				auto rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci);

				if (!rv) {
					log::stream << "MiniDump creation failed. Error: 0x" << std::hex << GetLastError() << std::endl;
				}
				else {
					log::stream << "MiniDump creation successful." << std::endl;
				}

				// Close the file
				CloseHandle(hFile);
			}
			else {
				log::stream << "MiniDump creation failed. Could not get file handle. Error: " << GetLastError() << std::endl;
			}
		}

		int __stdcall onWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
			__try {
				return reinterpret_cast<int(__stdcall*)(HINSTANCE, HINSTANCE, LPSTR, int)>(0x4049B7)(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
			}
			__except (CreateMiniDump(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}

		//
		// Patch: Respect symbolic links.
		// 
		// Unlike most of the Win32 API, FindFirstFileA and FindNextFileA don't respect symbolic links and
		// instead return information about the link itself instead of its target.
		// 
		// This patch makes it return the file size of the target file, rather than the symlink itself (0).
		//

		namespace sizeForSymbolicLinks {
			static std::unordered_map<HANDLE, std::string> findFilePaths;
			static std::recursive_mutex findFileMutex;

			std::optional<std::string> getFindFilePath(HANDLE hFindFile) {
				findFileMutex.lock();
				const auto itt = findFilePaths.find(hFindFile);
				if (itt == findFilePaths.end()) {
					findFileMutex.unlock();
					return {};
				}

				findFileMutex.unlock();
				return itt->second;
			}

			void fixSymlinkData(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData) {
				const auto path = getFindFilePath(hFindFile);
				if (!path) {
					return;
				}

				const auto fullPath = std::filesystem::path(path.value()) / lpFindFileData->cFileName;
				if (!std::filesystem::exists(fullPath)) {
					return;
				}

				const auto fileSize = std::filesystem::file_size(fullPath);
				lpFindFileData->nFileSizeHigh = unsigned int(fileSize / std::numeric_limits<unsigned int>::max());
				lpFindFileData->nFileSizeLow = unsigned int(fileSize);
			}

			HANDLE __stdcall findFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) {
				auto result = FindFirstFileA(lpFileName, lpFindFileData);
				if (result == INVALID_HANDLE_VALUE) {
					return result;
				}

				findFileMutex.lock();
				findFilePaths[result] = std::filesystem::path(lpFileName).parent_path().string();
				findFileMutex.unlock();

				// Check to see if it resolved to a symbolic link.
				if (lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT && lpFindFileData->dwReserved0 == IO_REPARSE_TAG_SYMLINK) {
					fixSymlinkData(result, lpFindFileData);
				}

				return result;
			}

			BOOL __stdcall findNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData) {
				auto result = FindNextFileA(hFindFile, lpFindFileData);
				if (!result) {
					return result;
				}

				// Check to see if it resolved to a symbolic link.
				if (lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT && lpFindFileData->dwReserved0 == IO_REPARSE_TAG_SYMLINK) {
					fixSymlinkData(hFindFile, lpFindFileData);
				}

				return result;
			}

			BOOL __stdcall findClose(HANDLE hFindFile) {
				findFileMutex.lock();
				const auto itt = findFilePaths.find(hFindFile);
				if (itt != findFilePaths.end()) {
					findFilePaths.erase(itt);
				}
				findFileMutex.unlock();

				return FindClose(hFindFile);
			}
		}

		//
		// Patch: Load metadata when mods load.
		//

		const auto CS_RecordHandler_LoadFiles = reinterpret_cast<void(__thiscall*)(RecordHandler*)>(0x501500);
		void __fastcall PatchOnLoadFiles(RecordHandler* recordHandler) {
			CS_RecordHandler_LoadFiles(recordHandler);
			metadata::reloadModMetadata();
		}

		//
		// Patch: Redirect from help file to a conversion website
		//

		BOOL __stdcall OverrideWinHelpA(HWND hWndMain, LPCSTR lpszHelp, UINT uCommand, ULONG_PTR dwData) {
			if (!string::equal(lpszHelp, "TES Construction Set.HLP")) {
				return WinHelpA(hWndMain, lpszHelp, uCommand, dwData);
			}

			// The CS only ever calls with two commands: FINDER and CONTEXT. The FINDER isn't even aware to the window it is on.
			switch (uCommand) {
			case HELP_FINDER:
				// Displays the Help Topics dialog box.
				ShellExecuteA(0, 0, "https://tes3cs.pages.dev/", 0, 0, SW_SHOW);
				return TRUE;
			case HELP_CONTEXT:
				// Displays the topic identified by the specified context identifier defined in the [MAP] section of the .hpj file.
				// Only two of these are ever called: 0x5DC and 0x3E8
				switch (dwData) {
				case 0x5DCu: // Functions
					ShellExecuteA(0, 0, "https://tes3cs.pages.dev/gameplay/scripting/functions/", 0, 0, SW_SHOW);
					return TRUE;
				case 0x3E8u: // Commands
					ShellExecuteA(0, 0, "https://tes3cs.pages.dev/gameplay/scripting/commands/", 0, 0, SW_SHOW);
					return TRUE;
				}
				break;
			}

			// Fall back to default behavior.
			return WinHelpA(hWndMain, lpszHelp, uCommand, dwData);
		}

	}

	CSSE application;

	CSSE::CSSE() {

	}

	BOOL CSSE::InitInstance() {
		CWinApp::InitInstance();

		// Open our log file.
		log::stream.open(path::getInstallPath() / "csse.log");
#ifdef APPVEYOR_BUILD_NUMBER
		log::stream << "Construction Set Extender build " << APPVEYOR_BUILD_NUMBER << " (built " << __DATE__ << ") hooked." << std::endl;
#else
		log::stream << "Construction Set Extender (built " << __DATE__ << ") hooked." << std::endl;
#endif

		// Always force the current path to the root directory.
		UpdateCurrentDirectory();

		// Load settings. Immediately save after so we can see new options if needed.
		try {
			settings.load();
		}
		catch (std::exception&) {
			const char* message = "Could not parse the settings file csse.toml. Read csse.log for details of the error.\r\n\r\n" \
				"Press Yes to reset settings to default.\r\n" \
				"Press No to keep the settings file. You will need to immediately close the CS and correct the problem in csse.toml.";

			auto result = MessageBoxA(NULL, message, "Settings Error", MB_YESNO);
			if (result == IDYES) {
				// Reset validity and use default constructed values.
				settings.valid = true;
			}
		}
		settings.save();

		// We can stop now if we're (mostly) disabled.
		if (!settings.enabled) {
			log::stream << "CSSE is disabled." << std::endl;
			return FALSE;
		}

		// Install TES Construction Set executable patches.
		InstallPatches();

		// Report successful initialization.
		log::stream << "CSSE initialization complete." << std::endl;

		return TRUE;
	}

	void CSSE::InstallPatches() const {
		using memory::genCallEnforced;
		using memory::genCallUnprotected;
		using memory::genNOPUnprotected;
		using memory::genJumpEnforced;
		using memory::genJumpUnprotected;
		using memory::writeDoubleWordUnprotected;
		using memory::writeValueEnforced;
		using memory::overrideVirtualTableEnforced;

		// Patch: Collect crash dumps.
#ifndef _DEBUG
		genCallEnforced(0x620DF9, 0x4049B7, reinterpret_cast<DWORD>(patch::onWinMain));
#endif

		// Get the vanilla masters so we suppress errors from them.
		genCallEnforced(0x50194E, 0x4041C4, reinterpret_cast<DWORD>(patch::findVanillaMasters));

		// Patch: Prevent GMST pollution.
		genJumpEnforced(0x4042B4, 0x4F9BE0, reinterpret_cast<DWORD>(patch::preventGMSTPollution));

		// Patch: Suppress "[Following/Previous] string is different for topic" warnings. They're pointless.
		genCallEnforced(0x4F3186, 0x40123A, reinterpret_cast<DWORD>(patch::suppressDialogueInfoResolveIssues));
		genCallEnforced(0x4F31AA, 0x40123A, reinterpret_cast<DWORD>(patch::suppressDialogueInfoResolveIssues));
		genCallEnforced(0x4F3236, 0x40123A, reinterpret_cast<DWORD>(patch::suppressDialogueInfoResolveIssues));
		genCallEnforced(0x4F325A, 0x40123A, reinterpret_cast<DWORD>(patch::suppressDialogueInfoResolveIssues));

		// Patch: suppressDeletedDefaultDialogueTypeChangeWarning
		genJumpEnforced(0x402211, 0x4F3360, reinterpret_cast<DWORD>(patch::suppressDeletedDefaultDialogueTypeChangeWarning));

		// Patch: Suppress "1 duplicate references were removed" warning popups for vanilla masters.
		genCallEnforced(0x50A9ED, 0x40123A, reinterpret_cast<DWORD>(patch::suppressDuplicateReferenceRemovedWarningForVanillaMasters));

		// Restore debug logs.
		if constexpr (LOG_NI_MESSAGES) {
			genJumpUnprotected(0x593110, reinterpret_cast<DWORD>(patch::restoreNiLogMessage));
			genJumpUnprotected(0x593120, reinterpret_cast<DWORD>(patch::restoreNiLogMessage));
			genJumpUnprotected(0x593130, reinterpret_cast<DWORD>(patch::restoreNiLogMessageWithNewline));
		}

		// Patch: Prevent directory changing when passing a file to the CS.
		genNOPUnprotected(0x443E25, 0x443E30 - 0x443E25);

		// Patch: Speed up MO2 load times.
		writeDoubleWordUnprotected(0x6D9C20, reinterpret_cast<DWORD>(&_stat32));

		// Patch: Ensure master array is initialized for loading plugins.
		genCallEnforced(0x501884, 0x4016F4, reinterpret_cast<DWORD>(patch::forceLoadFlagsOnActiveMods));

		// Patch: Fix bound calculation.
		genJumpEnforced(0x404467, 0x548280, reinterpret_cast<DWORD>(NI::CalculateBounds));

		// Patch: Fix NiLinesData binary loading.
		auto NiLinesData_loadBinary = &NI::LinesData::loadBinary;
		overrideVirtualTableEnforced(0x67A220, 0xC, 0x5CAEF0, *reinterpret_cast<DWORD*>(&NiLinesData_loadBinary));

		// Patch: Always clone scene graph nodes.
		writeValueEnforced(0x548973, BYTE(0x02), BYTE(0x00));

		// Patch: Always copy all NiExtraData on clone, instead of only the first NiStringExtraData.
		genJumpUnprotected(0x53FEE8, 0x53FF0E);
		genJumpUnprotected(0x53FF17, 0x53FF21);

		// Patch: Add deterministic subtree ordering mode to NiSortAdjustNode. Fix crash when cloning with no accumulator.
		overrideVirtualTableEnforced(0x67A5C8, 0x78, 0x5CF270, reinterpret_cast<DWORD>(patch::NISortAdjustNodeDisplay));
		genCallUnprotected(0x5CF45B, reinterpret_cast<DWORD>(patch::NISortAdjustNodeCloneAccumulator));

		// Patch: Prevent rogue files from popping up.
		genCallUnprotected(0x4852C8, reinterpret_cast<DWORD>(patch::CreateRootDirectoryFile), 0x6); // Warnings.txt
		genCallUnprotected(0x4852F2, reinterpret_cast<DWORD>(patch::DeleteRootDirectoryFile), 0x6); // Warnings.txt
		genCallUnprotected(0x485359, reinterpret_cast<DWORD>(patch::CreateRootDirectoryFile), 0x6); // Warnings.txt
		genCallUnprotected(0x485494, reinterpret_cast<DWORD>(patch::CreateRootDirectoryFile), 0x6); // Warnings.txt
		genCallUnprotected(0x4857A6, reinterpret_cast<DWORD>(patch::CreateRootDirectoryFile), 0x6); // ProgramFlow.txt

		// Patch: Remember last used model/icon directories.
		genCallEnforced(0x414EB4, 0x573290, reinterpret_cast<DWORD>(patch::GetOpenFileNameForIcon));
		genCallEnforced(0x414C5E, 0x573290, reinterpret_cast<DWORD>(patch::GetOpenFileNameForModel));

		// Patch: Allow selecting both tga and dds icon files.
		genJumpEnforced(0x402FF4, 0x414E10, reinterpret_cast<DWORD>(patch::RequestIconFilename));

		// Patch: Respect targets when searching for symlinks.
		writeDoubleWordUnprotected(0x6D99E8, reinterpret_cast<DWORD>(&patch::sizeForSymbolicLinks::findFirstFileA));
		writeDoubleWordUnprotected(0x6D99EC, reinterpret_cast<DWORD>(&patch::sizeForSymbolicLinks::findNextFileA));
		writeDoubleWordUnprotected(0x6D9A00, reinterpret_cast<DWORD>(&patch::sizeForSymbolicLinks::findClose));

		// Patch: Load mod metadata with mod data.
		genJumpEnforced(0x40178A, 0x501500, reinterpret_cast<DWORD>(patch::PatchOnLoadFiles));

		// Patch: Redirect away from the help file, which is no longer supported by Windows.
		writeDoubleWordUnprotected(0x6D9ECC, reinterpret_cast<DWORD>(&patch::OverrideWinHelpA));

		// Install all our sectioned patches.
		window::main::installPatches();
		dialog::actor_ai_window::installPatches();
		dialog::cell_window::installPatches();
		dialog::dialogue_window::installPatches();
		dialog::edit_object_window::installPatches();
		dialog::landscape_edit_settings_window::installPatches();
		dialog::object_window::installPatches();
		dialog::path_grid_window::installPatches();
		dialog::preview_window::installPatches();
		dialog::reference_data::installPatches();
		dialog::render_window::installPatches();
		dialog::script_editor_window::installPatches();
		dialog::script_list_window::installPatches();
		dialog::search_and_replace_window::installPatches();
		dialog::text_search_window::installPatches();
		dialog::use_report_window::installPatches();
		TextureRenderer::installPatches();
	}

	void CSSE::UpdateCurrentDirectory() const {
		using namespace std::filesystem;
		using namespace windows;

		// Get the path of CSSE.dll.
		auto csseDllPath = getModulePath(m_hInstance);
		if (csseDllPath.empty()) {
			log::stream << "WARNING: Could not resolve CSSE DLL directory." << std::endl;
			return;
		}

		// See if we even need to change paths.
		auto oldPath = current_path();
		auto installPath = csseDllPath.parent_path();
		if (oldPath == installPath) {
			return;
		}

		// Update and log path change.
		current_path(installPath);
		log::stream << "Changed working directory from " << oldPath << " to " << installPath << std::endl;
	}

	int CSSE::ExitInstance() {
		settings.save();

		return CWinApp::ExitInstance();
	}
}

BEGIN_MESSAGE_MAP(se::cs::CSSE, CWinApp)
END_MESSAGE_MAP()