#include "mwAdapter.h"
#include "Log.h"
#include "MgeTes3Machine.h"
#include "MGEApi.h"

#include "TES3Util.h"
#include "CodePatchUtil.h"
#include "PatchUtil.h"
#include "MWSEDefs.h"
#include "BuildDate.h"

#include "NICopyTransformController.h"
#include "NIStream.h"

#include "LuaManager.h"
#include "TES3Game.h"

TES3MACHINE* mge_virtual_machine = NULL;

const auto TES3_Game_ctor = reinterpret_cast<TES3::Game * (__thiscall*)(TES3::Game*)>(0x417280);
TES3::Game* __fastcall OnGameStructCreated(TES3::Game* game) {
	// Install necessary patches.
	mwse::patch::installPatches();

	// Install NetImmerse extensions.
	auto registered = *reinterpret_cast<TES3::HashMap<int, int>**>(0x7DDE5C);
	NI::Stream::registerLoader(NI::CopyTransformController::CopyTransformController_Name, &NI::CopyTransformController::loader);

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
	auto initialized = game->initialize();

	// Install any patches that need to run after most core structures are initialized.
	mwse::patch::installPostInitializationPatches();

	return initialized;
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
		if (!mge::guiVersion.valid()) {
			mwse::log::getLog() << "Error: Could not determine MGE XE version." << std::endl;
			MessageBox(NULL, "MGE XE does not seem to be installed. Please install MGE XE v0.10.0.0 or later.", "MGE XE Check Failed", MB_ICONERROR | MB_OK);
			exit(0);
		}
		else if (mge::guiVersion.major == 0 && mge::guiVersion.minor < 10) {
			mwse::log::getLog() << "Invalid MGE XE version: " << (int)mge::guiVersion.major << "." << (int)mge::guiVersion.minor << "." << (int)mge::guiVersion.patch << "." << (int)mge::guiVersion.build << std::endl;

			std::stringstream ss;
			ss << "Invalid MGE XE version found. Minimum version is 0.10.0.0." << std::endl;
			ss << "Found MGE XE v" << (int)mge::guiVersion.major << "." << (int)mge::guiVersion.minor << "." << (int)mge::guiVersion.patch << "." << (int)mge::guiVersion.build;
			MessageBox(NULL, ss.str().c_str(), "MGE XE Check Failed", MB_ICONERROR | MB_OK);
			exit(0);
		}
		else {
			mwse::log::getLog() << "Found MGE XE. Version: " << (int)mge::guiVersion.major << "." << (int)mge::guiVersion.minor << "." << (int)mge::guiVersion.patch << "." << (int)mge::guiVersion.build << std::endl;
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
			for (auto i = 0U; i < enabledFeatures.size(); ++i) {
				if (i != 0) log << ", ";
				log << enabledFeatures[i];
			}
			log << std::endl;

			// Make sure our necessary features are installed.
			if (!mwse::mcp::getFeatureEnabled(mwse::mcp::feature::SavegameCorruptionFix)) {
				mwse::log::getLog() << "The Morrowind Script Extender requires the Morrowind Code Patch \"Savegame Corruption Fix\" bugfix to be. Ensure that you have installed it." << std::endl;
				MessageBox(NULL, "The Morrowind Script Extender requires the Morrowind Code Patch \"Savegame Corruption Fix\" bugfix to be. Ensure that you have installed it.", "Morrowind Code Patch installation insufficient!", MB_OK | MB_APPLMODAL | MB_ICONERROR);
				return FALSE;
			}
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
	__declspec(dllexport) bool MGEInterface(mge::MGEAPI*);
	__declspec(dllexport) TES3MACHINE* MWSEGetVM();
	__declspec(dllexport) bool MWSEAddInstruction(OPCODE op, INSTRUCTION* ins);
}

bool MGEInterface(mge::MGEAPI* api) {
	int version = api->getAPIVersion();
	if (version >= 1) {
		mge::api = static_cast<mge::MGEAPIv1*>(api);
		mge::apiVersion = version;
		mge::macros = mge::api->macroFunctions();
		return true;
	}
	return false;
}

TES3MACHINE* MWSEGetVM()
{
	return mge_virtual_machine;
}

bool MWSEAddInstruction(OPCODE op, INSTRUCTION* ins)
{
	return mge_virtual_machine->AddInstruction(op, ins);
}
