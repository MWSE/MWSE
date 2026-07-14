#include "CrashLogger.h"

#include "StringUtil.h"
#include "WindowsUtil.h"

namespace CrashLogger::Playtime {
	static std::stringstream output;
	static std::chrono::time_point<std::chrono::system_clock> applicationStart;

	void Init() {
		applicationStart = std::chrono::system_clock::now();
	}

	void Process(EXCEPTION_POINTERS* info) {
		try {
			const auto applicationEnd = std::chrono::system_clock::now();
			output << fmt::format("Playtime: {:%H:%M:%S}\n", applicationEnd - applicationStart);
		}
		catch (...) {
			output << "Failed to process playtime.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::Exception {
	static std::stringstream output;

	void Process(EXCEPTION_POINTERS* info) {
		try {
			const auto exceptionAsString = GetExceptionAsString(info->ExceptionRecord->ExceptionCode);
			output << fmt::format("Exception: {} ({:08X})\n", exceptionAsString, info->ExceptionRecord->ExceptionCode);

			const auto lastError = GetLastError();
			if (lastError) {
				const auto asString = SanitizeString(GetErrorAsString(lastError));
				output << fmt::format("Last Error: {} ({:08X})\n", asString, lastError);
			}
		}
		catch (...) {
			output << "Failed to log exception.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::Thread {
	static std::stringstream output;

	static std::string GetThreadName() {
		auto name = se::windows::GetThreadDescription(GetCurrentThread());
		if (!name.has_value()) {
			name = L"<unsupported>";
		}
		else if (name.value().empty()) {
			name = L"<unknown>";
		}

		return se::string::from_wstring(name.value());
	}

	void Process(EXCEPTION_POINTERS* info) {
		try {
			output << "Thread: " << GetThreadName() << '\n';
		}
		catch (...) {
			output << "Failed to log thread name.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::Install {
	static std::stringstream output;

	static std::filesystem::path GetInstallPath() {
		CHAR path[MAX_PATH] = {};
		if (GetModuleFileNameA(nullptr, path, MAX_PATH) <= 0) {
			return {};
		}
		return std::filesystem::canonical(path).parent_path();
	}

	void Process(EXCEPTION_POINTERS* info) {
		try {
			output << fmt::format("Install Path: {}\n", GetInstallPath().string());
		}
		catch (...) {
			output << "Failed to log version.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::Calltrace {
	static std::stringstream output;

	struct StackEntry {
		UINT32 ebp;
		std::string address;
		std::string name;
		std::string source;

		StackEntry(UINT32 eip, UINT32 ebp, HANDLE process) : ebp(ebp) {
			const auto moduleBase = PDB::GetModuleBase(eip, process);
			const auto moduleOffset = (moduleBase != 0x00400000) ? eip - moduleBase + 0x10000000 : eip;

			if (const auto module = PDB::GetModule(eip, process); module.empty()) {
				address = fmt::format("??? (0x{:08X})", moduleOffset);
				name = "(Corrupt stack or heap?)";
			}
			else if (const auto symbol = PDB::GetSymbol(eip, process); symbol.empty()) {
				address = fmt::format("{} (0x{:08X})", module, moduleOffset);
			}
			else {
				address = fmt::format("{} (0x{:08X})", module, moduleOffset);
				name = symbol;
			}

			if (const auto line = PDB::GetLine(eip, process); !line.empty()) {
				source = line;
			}
		}
	};

	void Process(EXCEPTION_POINTERS* info) {
		try {
			const auto process = GetCurrentProcess();
			const auto thread = GetCurrentThread();
			constexpr DWORD machine = IMAGE_FILE_MACHINE_I386;
			CONTEXT context = {};
			memcpy(&context, info->ContextRecord, sizeof(context));

			SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_ALLOW_ABSOLUTE_SYMBOLS);

			char workingDirectory[MAX_PATH] = {};
			char symbolPath[MAX_PATH] = {};
			char alternateSymbolPath[MAX_PATH] = {};
			GetCurrentDirectoryA(MAX_PATH, workingDirectory);
			GetEnvironmentVariableA("_NT_SYMBOL_PATH", symbolPath, MAX_PATH);
			GetEnvironmentVariableA("_NT_ALTERNATE_SYMBOL_PATH", alternateSymbolPath, MAX_PATH);
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
			const auto searchPath = fmt::format("{};{}\\Data\\OBSE\\plugins;{};{}", workingDirectory, workingDirectory, symbolPath, alternateSymbolPath);
#else
			const auto searchPath = fmt::format("{};{};{}", workingDirectory, symbolPath, alternateSymbolPath);
#endif

			if (!SymInitialize(process, searchPath.c_str(), true)) {
				output << "Error initializing symbol store\n";
			}

			STACKFRAME frame = {};
			frame.AddrPC.Offset = info->ContextRecord->Eip;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = info->ContextRecord->Ebp;
			frame.AddrFrame.Mode = AddrModeFlat;
			frame.AddrStack.Offset = info->ContextRecord->Esp;
			frame.AddrStack.Mode = AddrModeFlat;

			DWORD previousEip = 0;
			std::vector<StackEntry> entries;
			while (StackWalk(machine, process, thread, &frame, &context, nullptr, SymFunctionTableAccess, SymGetModuleBase, nullptr)) {
				if (frame.AddrPC.Offset == previousEip) break;
				previousEip = frame.AddrPC.Offset;
				entries.emplace_back(frame.AddrPC.Offset, frame.AddrFrame.Offset, process);
			}

			if (entries.empty()) {
				output << "No call stack frames were available.\n";
				return;
			}

			const auto addressLength = std::max_element(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
				return a.address.length() < b.address.length();
			})->address.length();
			const auto nameLength = std::max_element(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
				return a.name.length() < b.name.length();
			})->name.length();

			output << fmt::format("{:^10} | {:>{}} | {:<{}} | {}", "EBP", "Function Address", addressLength, "Function Name", nameLength, "Source") << '\n';
			for (const auto& entry : entries) {
				output << fmt::format("0x{:08X} | {:>{}} | {:<{}} | {}", entry.ebp, entry.address, addressLength, entry.name, nameLength, entry.source) << '\n';
			}
		}
		catch (...) {
			output << "Failed to log callstack.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::Warnings {
	static std::stringstream output;

	void Process(EXCEPTION_POINTERS* info) {
		try {
			if (!std::filesystem::exists("Warnings.txt")) return;

			std::ifstream warnings("Warnings.txt");
			if (!warnings.is_open()) return;

			std::unordered_set<std::string> seenLines;
			std::string line;
			while (std::getline(warnings, line)) {
				if (!line.empty() && seenLines.emplace(line).second) {
					output << line << '\n';
				}
			}
		}
		catch (...) {
			output << "Failed to log warnings.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}
