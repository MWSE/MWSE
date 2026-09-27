#include "MiniDump.h"

#include "CrashLogger.h"
#include "MWSEConfig.h"
#include "Log.h"
#include "TES3DataHandler.h"
#include "TES3Game.h"


namespace mwse::minidump {
#ifndef _DEBUG
	// Release builds.
	constexpr auto INSTALL_MINIDUMP_HOOK = true;
#else
	// Debug builds.
	constexpr auto INSTALL_MINIDUMP_HOOK = false;
#endif

	bool isDataSectionNeeded(const WCHAR* pModuleName) {
		// Check parameters.
		if (pModuleName == 0) {
			return false;
		}

		// Extract the module name.
		WCHAR szFileName[_MAX_FNAME] = L"";
		_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);

		// Compare the name with the list of known names and decide.
		if (_wcsicmp(szFileName, L"Morrowind") == 0) {
			return true;
		}
		else if (_wcsicmp(szFileName, L"ntdll") == 0)
		{
			return true;
		}
		else if (_wcsicmp(szFileName, L"MWSE") == 0)
		{
			return true;
		}

		// Complete.
		return false;
	}

	static BOOL CALLBACK miniDumpCallback(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput) {
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

	const char* GetThreadName(DWORD threadId) {
		const auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			if (threadId == dataHandler->mainThreadID) {
				return "Main";
			}
			else if (threadId == dataHandler->backgroundThreadID) {
				return "Background";
			}
		}

		return "Unknown";
	}

	const char* GetThreadName() {
		return GetThreadName(GetCurrentThreadId());
	}

	void CreateMiniDump(EXCEPTION_POINTERS* pep) {
		// Open the file.
		auto hFile = CreateFile("MWSE_MiniDump.dmp", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE)) {
			// Create the minidump.
			MINIDUMP_EXCEPTION_INFORMATION mdei = {};

			mdei.ThreadId = GetCurrentThreadId();
			mdei.ExceptionPointers = pep;
			mdei.ClientPointers = FALSE;

			MINIDUMP_CALLBACK_INFORMATION mci = {};

			mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)miniDumpCallback;
			mci.CallbackParam = 0;

			auto mdt = MiniDumpWithDataSegs |
				MiniDumpWithHandleData |
				MiniDumpWithFullMemoryInfo |
				MiniDumpWithThreadInfo |
				MiniDumpWithUnloadedModules;

			if (Configuration::CreateFullMinidumps) {
				mdt |= MiniDumpWithFullMemory | MiniDumpWithIndirectlyReferencedMemory;
			}

			auto rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, static_cast<MINIDUMP_TYPE>(mdt), (pep != 0) ? &mdei : 0, 0, &mci);

			if (!rv) {
				log::getLog() << "MiniDump creation failed. Error: 0x" << std::hex << GetLastError() << std::endl;
			}
			else {
				log::getLog() << "MiniDump creation successful." << std::endl;
			}

			// Close the file
			CloseHandle(hFile);
		}
		else {
			log::getLog() << "MiniDump creation failed. Could not get file handle. Error: " << GetLastError() << std::endl;
		}
	}

	static void ResetGamma() {
		__try {
			auto game = TES3::Game::get();
			if (game) {
				game->setGamma(1.0f);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}

	LONG WINAPI MWSEUnhandledExceptionFilter(__in  struct _EXCEPTION_POINTERS* info) {
		log::getLog() << std::dec << std::endl;
		log::getLog() << "Morrowind has crashed! To help improve game stability, send mwse.log to the #mwse channel at the Morrowind Modding Community Discord: https://discord.me/mwmods" << std::endl;

		ResetGamma();

		CreateMiniDump(info);

		if constexpr (CrashLogger::DEBUG_LOGGER) {
			log::getLog() << "Attempting To Log Crash \n";
		}
		return CrashLogger::Filter(info);
	}

	bool installHook() {
		if constexpr (INSTALL_MINIDUMP_HOOK) {
			CrashLogger::Playtime::Init();
			CrashLogger::s_originalFilter = SetUnhandledExceptionFilter(MWSEUnhandledExceptionFilter);
		}
		return true;
	}
}
