#include "CrashLogger.h"

#include "StringUtil.h"

namespace CrashLogger::PDB {
	std::string GetModule(UINT32 eip, HANDLE process) {
		IMAGEHLP_MODULE module = {};
		module.SizeOfStruct = sizeof(module);
		if (!SymGetModuleInfo(process, eip, &module)) return "";

		return module.ModuleName;
	}

	UINT32 GetModuleBase(UINT32 eip, HANDLE process) {
		IMAGEHLP_MODULE module = {};
		module.SizeOfStruct = sizeof(module);
		if (!SymGetModuleInfo(process, eip, &module)) return 0;

		return module.BaseOfImage;
	}

	std::string GetSymbol(UINT32 eip, HANDLE process) {
		char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME] = {};
		const auto symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);

		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MAX_SYM_NAME - 1;
		DWORD64 offset = 0;

		if (!SymFromAddr(process, eip, &offset, symbol)) return "";

		return fmt::format("{}+0x{:0X}", symbol->Name, offset);
	}

	std::string GetLine(UINT32 eip, HANDLE process) {
		IMAGEHLP_LINE line = {};
		line.SizeOfStruct = sizeof(line);
		DWORD offset = 0;

		if (!SymGetLineFromAddr(process, eip, &offset, &line)) return "";

		return fmt::format("{}:{:d}", line.FileName, line.LineNumber);
	}

	static void GetClassNameFromPDBCpp(void* object, std::string& name) {
		try {
			name = GetSymbol(*static_cast<UINT32*>(object), GetCurrentProcess());
		}
		catch (...) {
		}
	}

	static void GetClassNameFromPDBSEH(void* object, std::string& name) {
		__try {
			GetClassNameFromPDBCpp(object, name);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
		}
	}

	static std::string GetClassNameFromPDB(void* object) {
		std::string name;
		GetClassNameFromPDBSEH(object, name);
		se::string::strip_start(name, "vtbl_");
		se::string::strip_start(name, "sg_");
		se::string::strip_end(name, "+0x0");
		return name;
	}

	std::string GetClassNameFromRTTIorPDB(void* object) {
		if (auto name = Client::GetKnownClassName(object); !name.empty()) {
			return name;
		}
		return GetClassNameFromPDB(object);
	}
}

namespace CrashLogger {
	static void LogSection(const char* failureMessage, void (*process)(EXCEPTION_POINTERS*), EXCEPTION_POINTERS* info) {
		__try {
			process(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			Client::GetLog() << failureMessage << '\n';
		}
	}

	void Log(EXCEPTION_POINTERS* info) {
		auto& log = Client::GetLog();
		const auto begin = std::chrono::system_clock::now();

		LogSection("Failed to log playtime.", Playtime::Process, info);
		LogSection("Failed to log version.", Version::Process, info);
		LogSection("Failed to log exception.", Exception::Process, info);
		LogSection("Failed to log thread.", Thread::Process, info);
		LogSection("Failed to log memory.", Memory::Process, info);
		LogSection("Failed to log device.", Device::Process, info);
		LogSection("Failed to log calltrace.", Calltrace::Process, info);
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		LogSection("Failed to log lua traceback.", LuaTraceback::Process, info);
#endif
		LogSection("Failed to log registry.", Registry::Process, info);
		LogSection("Failed to log stack.", Stack::Process, info);
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		LogSection("Failed to log mwscript state.", MorrowindScript::Process, info);
#endif
		LogSection("Failed to log mods.", Mods::Process, info);
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		LogSection("Failed to log lua mods.", LuaMods::Process, info);
#endif
		LogSection("Failed to log install.", Install::Process, info);
		LogSection("Failed to log warnings.", Warnings::Process, info);

		const auto processing = std::chrono::system_clock::now();

		log << "=== BASIC INFORMATION: =================================================================================================\n";
		log << Version::Get().str();
		log << Memory::Get().str();
		log << Playtime::Get().str();
		log << Thread::Get().str();
		log << Exception::Get().str();
		log << Install::Get().str();
		log << Device::Get().str();
		log << "=== CALL STACK: ========================================================================================================\n";
		log << Calltrace::Get().str();
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		log << "=== LUA STACK: =========================================================================================================\n";
		log << LuaTraceback::Get().str();
#endif
		log << "=== REGISTRY: ==========================================================================================================\n";
		log << Registry::Get().str();
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		log << "=== MWSCRIPT STATE: ====================================================================================================\n";
		log << MorrowindScript::Get().str();
#endif
		log << "=== STACK: =============================================================================================================\n";
		log << Stack::Get().str();
		log << "==== MODS: =============================================================================================================\n";
		log << Mods::Get().str();
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		log << "==== LUA MODS: =========================================================================================================\n";
		log << LuaMods::Get().str();
#endif
		log << "=== WARNINGS: ==========================================================================================================\n";
		log << Warnings::Get().str();

		if constexpr (DEBUG_LOGGER) {
			const auto printing = std::chrono::system_clock::now();
			const auto timeProcessing = std::chrono::duration_cast<std::chrono::milliseconds>(processing - begin);
			const auto timePrinting = std::chrono::duration_cast<std::chrono::milliseconds>(printing - processing);
			log << "=== LOGGING INFORMATION: ===============================================================================================\n";
			log << fmt::format("Processed in {:d} ms, printed in {:d} ms", static_cast<long>(timeProcessing.count()), static_cast<long>(timePrinting.count()));
		}

		log.flush();
		SymCleanup(GetCurrentProcess());
	}

	void AttemptLog(EXCEPTION_POINTERS* info) {
		__try {
			Log(info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			Client::GetLog() << "Failed to log exception info";
		}
	}

	LPTOP_LEVEL_EXCEPTION_FILTER s_originalFilter = nullptr;

	LONG WINAPI Filter(EXCEPTION_POINTERS* info) {
		static bool caught = false;
		const bool ignored = caught;
		if (!caught) {
			caught = true;
			AttemptLog(info);
		}
		if (s_originalFilter) s_originalFilter(info);
		return !ignored ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
	}
}
