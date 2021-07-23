/************************************************************************
	
	main.cpp - Copyright (c) 2008 The MWSE Project
	https://github.com/MWSE/MWSE/

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#include "mwAdapter.h"
#include "Log.h"
#include "MgeTes3Machine.h"

#include "TES3Util.h"
#include "CodePatchUtil.h"
#include "PatchUtil.h"
#include "MWSEDefs.h"
#include "BuildDate.h"

#include "LuaManager.h"
#include "TES3Game.h"

TES3MACHINE* mge_virtual_machine = NULL;

struct VersionStruct {
	BYTE major;
	BYTE minor;
	BYTE patch;
	BYTE build;
};

VersionStruct GetMGEVersion() {
	DWORD dwSize = GetFileVersionInfoSize("MGEXEgui.exe", NULL);
	if (dwSize == 0) {
		return VersionStruct{};
	}

	BYTE * pbVersionInfo = new BYTE[dwSize];
	if (!GetFileVersionInfo("MGEXEgui.exe", 0, dwSize, pbVersionInfo)) {
		delete[] pbVersionInfo;
		return VersionStruct{};
	}

	VS_FIXEDFILEINFO * pFileInfo = NULL;
	UINT puLenFileInfo = 0;
	if (!VerQueryValue(pbVersionInfo, TEXT("\\"), (LPVOID*)&pFileInfo, &puLenFileInfo)) {
		delete[] pbVersionInfo;
		return VersionStruct{};
	}

	VersionStruct version;
	version.major = BYTE(HIWORD(pFileInfo->dwProductVersionMS));
	version.minor = BYTE(LOWORD(pFileInfo->dwProductVersionMS));
	version.patch = BYTE(LOWORD(pFileInfo->dwProductVersionLS >> 16));
	version.build = BYTE(HIWORD(pFileInfo->dwProductVersionLS >> 16));
	delete[] pbVersionInfo;

	return version;
}

const auto TES3_Game_ctor = reinterpret_cast<TES3::Game*(__thiscall*)(TES3::Game*)>(0x417280);
TES3::Game* __fastcall OnGameStructCreated(TES3::Game * game) {
	// Install necessary patches.
	mwse::patch::installPatches();

	// Call overloaded function.
	return TES3_Game_ctor(game);
}

// This is technically a this-call function, but it doesn't make any use of member data.
const auto TES3_LoadLanguageFromINI = reinterpret_cast<void(__stdcall*)()>(0x467850);

bool __fastcall OnGameStructInitialized(TES3::Game* game) {
	// Force language recognition early so pre-initialization mods can make use of it.
	// We want this to run before mwse_init.lua but after MO2 has had a chance to do its thing.
	TES3_LoadLanguageFromINI();

	// Setup our lua interface before initializing.
	mwse::lua::LuaManager::getInstance().hook();

	// Install our later patches.
	mwse::patch::installPostLuaPatches();

	// Call overloaded function.
	return game->initialize();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	HANDLE process = NULL;
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
	{
		// Initialize once for each new process. Return FALSE to fail DLL load.

		// Initialize log file.
		mwse::log::OpenLog("MWSE.log");
#ifdef APPVEYOR_BUILD_NUMBER
		mwse::log::getLog() << "Morrowind Script Extender v" << MWSE_VERSION_MAJOR << "." << MWSE_VERSION_MINOR << "." << MWSE_VERSION_PATCH << "-" << APPVEYOR_BUILD_NUMBER << " (built " << __DATE__ << ") hooked." << std::endl;
#else
		mwse::log::getLog() << "Morrowind Script Extender v" << MWSE_VERSION_MAJOR << "." << MWSE_VERSION_MINOR << "." << MWSE_VERSION_PATCH << " (built " << __DATE__ << ") hooked." << std::endl;
#endif

		// Before we do anything else, ensure that we can make minidumps.
		if (!mwse::patch::installMiniDumpHook()) {
			mwse::log::getLog() << "Warning: Unable to hook minidump! Crash dumps will be unavailable." << std::endl;
		}

		// Make sure we have the right version of MGE XE installed.
		VersionStruct mgeVersion = GetMGEVersion();
		if (mgeVersion.major == 0 && mgeVersion.minor == 0) {
			mwse::log::getLog() << "Error: Could not determine MGE XE version." << std::endl;
			MessageBox(NULL, "MGE XE does not seem to be installed. Please install MGE XE v0.10.0.0 or later.", "MGE XE Check Failed", MB_ICONERROR | MB_OK);
			exit(0);
		}
		else if (mgeVersion.major == 0 && mgeVersion.minor < 10) {
			mwse::log::getLog() << "Invalid MGE XE version: " << (int)mgeVersion.major << "." << (int)mgeVersion.minor << "." << (int)mgeVersion.patch << "." << (int)mgeVersion.build << std::endl;

			std::stringstream ss;
			ss << "Invalid MGE XE version found. Minimum version is 0.10.0.0.\nFound version: ";
			ss << "Found MGE XE v" << (int)mgeVersion.major << "." << (int)mgeVersion.minor << "." << (int)mgeVersion.patch << "." << (int)mgeVersion.build;
			MessageBox(NULL, ss.str().c_str(), "MGE XE Check Failed", MB_ICONERROR | MB_OK);
			exit(0);
		}
		else {
			mwse::log::getLog() << "Found MGE XE. Version: " << (int)mgeVersion.major << "." << (int)mgeVersion.minor << "." << (int)mgeVersion.patch << "." << (int)mgeVersion.build << std::endl;
		}

		// Legacy support for old updater exe swap method.
		if (std::filesystem::exists("MWSE-Update.tmp")) {
			if (std::filesystem::exists("MWSE-Update.exe")) {
				std::filesystem::remove("MWSE-Update.exe");
			}
			std::filesystem::rename("MWSE-Update.tmp", "MWSE-Update.exe");
		}

		// List of temporary files that the updater couldn't update, and so need to be swapped out.
		std::vector<std::string> updaterTempFiles;
		updaterTempFiles.push_back("MWSE-Update.exe");
		updaterTempFiles.push_back("Newtonsoft.Json.dll");
		
		// Look to see if an update to the MWSE Updater was downloaded. If so, swap the temp files.
		for (const std::string& destFile : updaterTempFiles) {
			const std::string tempFile = destFile + ".tmp";
			if (std::filesystem::exists(tempFile)) {
				if (std::filesystem::exists(destFile)) {
					std::filesystem::remove(destFile);
				}
				std::filesystem::rename(tempFile, destFile);
			}
		}

		// Delete any old crash dumps.
		if (std::filesystem::exists("MWSE_MiniDump.dmp")) {
			std::filesystem::remove("MWSE_MiniDump.dmp");
		}

		// Initialize our main mwscript hook.
		mwse::mwAdapter::Hook();

		// Create MGE VM interface.
		mge_virtual_machine = new TES3MACHINE();

		// Parse and load the features installed by the Morrowind Code Patch.
		if (mwse::mcp::loadFeatureList()) {
			auto& log = mwse::log::getLog();
			log << "Morrowind Code Patch installed features: ";

			// Get a sorted list of enabled features.
			std::vector<long> enabledFeatures;
			for (const auto& itt : mwse::mcp::featureStore) {
				if (itt.second) {
					enabledFeatures.push_back(itt.first);
				}
			}
			std::sort(enabledFeatures.begin(), enabledFeatures.end());

			// Print them to the log.
			log << std::dec;
			for (auto i = 0U; i < enabledFeatures.size(); i++) {
				if (i != 0) log << ", ";
				log << enabledFeatures[i];
			}
			log << std::endl;
		}
		else {
			mwse::log::getLog() << "The Morrowind Script Extender requires the Morrowind Code Patch to be installed. Ensure that you have installed it, and have not deleted the mcpatch folder." << std::endl;

			auto result = MessageBox(NULL, "The Morrowind Script Extender requires the Morrowind Code Patch to be installed. Ensure that you have installed it, and have not deleted the mcpatch folder.\n\nWould you like to open the browser to visit the MCP download page?", "Morrowind Code Patch not installed!", MB_YESNO | MB_APPLMODAL | MB_ICONERROR);
			if (result == IDYES) {
				system("start https://www.nexusmods.com/morrowind/mods/19510?");
			}

			return FALSE;
		}

		// Install patches.
		mwse::genCallEnforced(0x417169, 0x417280, reinterpret_cast<DWORD>(OnGameStructCreated));

		// Delay our lua hook until later, to ensure that Mod Organizer's VFS is hooked up.
		if (!mwse::genCallEnforced(0x417195, 0x417880, reinterpret_cast<DWORD>(OnGameStructInitialized))) {
			mwse::log::getLog() << "Could not hook MWSE-Lua initialization point!" << std::endl;
			exit(1);
		}
	}
	break;
	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;
	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;
	case DLL_PROCESS_DETACH:
		// Unhook Lua interface.
		mwse::lua::LuaManager::getInstance().cleanup();
		break;
	}

	return TRUE; // Successful DLL_PROCESS_ATTACH.
}

// MGE XE exports
extern "C"
{
	__declspec(dllexport) TES3MACHINE* MWSEGetVM();
	__declspec(dllexport) bool MWSEAddInstruction(OPCODE op, INSTRUCTION *ins);
}

TES3MACHINE* MWSEGetVM()
{
	return mge_virtual_machine;
}

bool MWSEAddInstruction(OPCODE op, INSTRUCTION *ins)
{
	return mge_virtual_machine->AddInstruction(op, ins);
}
