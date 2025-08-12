#include <CrashLogger.h>

#pragma comment (lib, "version.lib")

namespace CrashLogger::Install
{
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info)
		try {
		output << "Install: " << SanitizeString(path::getInstallPath()) << '\n';
	}
	catch (...) { output << "Failed to print install path." << '\n'; }

	extern std::stringstream& Get() { output.flush(); return output; }
}

namespace CrashLogger::Modules
{
	std::stringstream output;

	struct UserContext {
		UINT32 eip;
		UINT32 moduleBase;
		CHAR* name;
	};

	struct Module
	{
		UINT32 moduleBase;
		UINT32 moduleEnd;
		std::filesystem::path path;

		bool operator<(const Module& other) const {
			if (path < other.path) return true;
			return false;
		};
	};

	std::set<Module> enumeratedModules;

	BOOL CALLBACK EumerateModulesCallback(PCSTR name, ULONG moduleBase, ULONG moduleSize, PVOID context) {
		UserContext* info = (UserContext*)context;
		if (info->eip >= (UINT32)moduleBase && info->eip <= (UINT32)moduleBase + (UINT32)moduleSize) {
			info->moduleBase = moduleBase;
			strcpy_s(info->name, 100, name);
		}
		enumeratedModules.emplace((UINT32)moduleBase, (UINT32)moduleBase + (UINT32)moduleSize, std::string(name));

		return TRUE;
	}

	static std::unordered_map<std::string, std::string> pluginNames = {
		{"hot_reload_editor", "hot_reload"},
		{"ilsfix", "ILS Fix"},
		{"improved_console", "Improved Console"},
		{"ImprovedLightingShaders", "Improved Lighting Shaders for FNV"},
		{"jip_nvse", "JIP LN NVSE"},
		{"johnnyguitar", "JohnnyGuitarNVSE"},
		{"MCM", "MCM Extensions"},
		{"mod_limit_fix", "Mod Limit Fix"},
		{"nvse_console_clipboard", "Console Clipboard"},
		{"nvse_stewie_tweaks", "lStewieAl's Tweaks"},
		{"ShowOffNVSE", "ShowOffNVSE Plugin"},
		{"supNVSE", "SUP NVSE Plugin"},
		{"ui_organizer", "UI Organizer Plugin"},
		{"EngineOptimizations", "Engine Optimizations"},
		{"DynamicReflections", "Dynamic Reflections"},
		{"Alpha Fixes", "Fallout Alpha Rendering Tweaks"}
	};

	std::string GetPluginNameForFileName(std::string name)
	{
		if (pluginNames.contains(name))
			return pluginNames[name];
		return name;
	}

	std::string GetFileVersion(std::string path) {
		std::string version = "";
		DWORD infoSize = GetFileVersionInfoSizeA(path.c_str(), 0);
		LPSTR verData = new char[infoSize];
		if (GetFileVersionInfo(path.c_str(), 0, infoSize, verData)) {
			LPBYTE lpBuffer = nullptr;
			UINT size = 0;
			if (VerQueryValue(verData, "\\", (VOID FAR * FAR*) & lpBuffer, &size) && size) {
				VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
				if (verInfo->dwSignature == 0xfeef04bd) {
					version = fmt::format("{}.{}.{}.{}",
						(verInfo->dwFileVersionMS >> 16) & 0xffff,
						(verInfo->dwFileVersionMS >> 0) & 0xffff,
						(verInfo->dwFileVersionLS >> 16) & 0xffff,
						(verInfo->dwFileVersionLS >> 0) & 0xffff
					);
				}
			}
		}
		delete[] verData;
		return version;
	}

	extern void Process(EXCEPTION_POINTERS* info)
		try {
		HANDLE process = GetCurrentProcess();

		const UINT32 eip = info->ContextRecord->Eip;

		UserContext infoUser = { eip,  0, (char*)calloc(sizeof(char), 200) };

		EnumerateLoadedModules(process, EumerateModulesCallback, &infoUser);

		size_t memoryAllocated = 0;

		//output << "Module bases:" << '\n' << fmt::format(" {:^23} | {:>40} ", "Address", "Module") <<
		//	'\n';;
		_MESSAGE("Module bases: \n %s \n", fmt::format(" {:^23} | {:>40} | {:>20} | Filepath", "Address", "Module", "Version").c_str());
		for (const auto& [moduleBase, moduleEnd, path] : enumeratedModules)
		{
			std::string version;

			if (const auto info = g_pluginManager.GetInfoByName(GetPluginNameForFileName(path.stem().generic_string()).c_str()))
				version = fmt::format("{:d}", info->version);

			if (version.empty()) {
				std::string dll_version = GetFileVersion(path.generic_string());
				if (dll_version.empty())
					dll_version = "Unknown";

				version = dll_version;
			}

			memoryAllocated += moduleEnd - moduleBase;

			//output << fmt::format(" 0x{:08X} - 0x{:08X} | {:>40} ", moduleBase, moduleEnd, path.stem().generic_string()) <<
			//	'\n';
			_MESSAGE("%s \n", fmt::format(" 0x{:08X} - 0x{:08X} | {:>40} | {:>20} | {}", moduleBase, moduleEnd, path.stem().generic_string(), version, SanitizeString(path.generic_string())).c_str());
		}

		//output << "\nTotal memory allocated to modules: " << FormatSize(memoryAllocated) << '\n';
		_MESSAGE("\nTotal memory allocated to modules: %s \n", FormatSize(memoryAllocated).c_str());
		output << '\n';

		if (infoUser.moduleBase) {
			/*
			output << fmt::format("GAME CRASHED AT INSTRUCTION Base+0x{:08X} IN MODULE: {}", (infoUser.eip - infoUser.moduleBase), infoUser.name) <<
			'\n'
			<< "Please note that this does not automatically mean that that module is responsible. It may have been supplied bad data or" <<
			'\n'
			<< "program state as the result of an issue in the base game or a different DLL." << '\n'; */
			_MESSAGE("%s \n", fmt::format("GAME CRASHED AT INSTRUCTION Base+0x{:08X} IN MODULE: {}", (infoUser.eip - infoUser.moduleBase), infoUser.name).c_str());
			_MESSAGE("Please note that this does not automatically mean that that module is responsible. It may have been supplied bad data or \n");
			_MESSAGE("program state as the result of an issue in the base game or a different DLL.\n");
		}
		else {
			/*
			output << "UNABLE TO IDENTIFY MODULE CONTAINING THE CRASH ADDRESS." << '\n'
			<< "This can occur if the crashing instruction is located in the vanilla address space, but it can also occur if there are too many" << '\n'
			<< "DLLs for us to list, and if the crash occurred in one of their address spaces. Please note that even if the crash occurred" << '\n'
			<< "in vanilla code, that does not necessarily mean that it is a vanilla problem. The vanilla code may have been supplied bad data" << '\n'
			<< "or program state as the result of an issue in a loaded DLL." << '\n'; */

			_MESSAGE("UNABLE TO IDENTIFY MODULE CONTAINING THE CRASH ADDRESS.");
			_MESSAGE("This can occur if the crashing instruction is located in the vanilla address space, but it can also occur if there are too many");
			_MESSAGE("DLLs for us to list, and if the crash occurred in one of their address spaces. Please note that even if the crash occurred");
			_MESSAGE("in vanilla code, that does not necessarily mean that it is a vanilla problem. The vanilla code may have been supplied bad data");
			_MESSAGE("or program state as the result of an issue in a loaded DLL. \n");
		}
	}
	catch (...) { output << "Failed to print out modules." << '\n'; }

	extern std::stringstream& Get() { output.flush(); return output; }
}