#include "CrashLoggerShared.h"
#include "StringUtil.h"
#include "WindowsUtil.h"

namespace se::crash {
	constexpr UINT32 stackScanCount = 0x100;
	constexpr UINT32 dereferenceDepth = 5;

	Settings currentSettings;
	std::chrono::time_point<std::chrono::system_clock> startTime;
	LPTOP_LEVEL_EXCEPTION_FILTER originalFilter = nullptr;
	bool caughtCrash = false;

	std::ostream& getLog() {
		if (currentSettings.logProvider) {
			return currentSettings.logProvider();
		}

		return std::cerr;
	}

	std::string sanitize(std::string value) {
		if (value.length() >= MAX_PATH) {
			value.resize(MAX_PATH);
		}

		const auto firstNonPrintable = std::remove_if(value.begin(), value.end(), [](char c) {
			return !std::isprint(static_cast<unsigned char>(c));
		});
		value.erase(firstNonPrintable, value.end());

		std::replace_if(value.begin(), value.end(), [](char c) {
			return std::isspace(static_cast<unsigned char>(c));
		}, ' ');

		return value;
	}

	float toMiB(UINT64 size) {
		return static_cast<float>(size) / 1024.0f / 1024.0f;
	}

	std::string formatSize(UINT64 size) {
		std::ostringstream stream;
		stream << std::right << std::fixed << std::setprecision(2);
		if (size < 1024) {
			stream << std::setw(6) << size << " B";
		}
		else if (size < 1024ull * 1024ull) {
			stream << std::setw(6) << (static_cast<float>(size) / 1024.0f) << " KiB";
		}
		else if (size < 1024ull * 1024ull * 1024ull) {
			stream << std::setw(6) << toMiB(size) << " MiB";
		}
		else {
			stream << std::setw(6) << (toMiB(size) / 1024.0f) << " GiB";
		}
		return stream.str();
	}

	std::string formatMemoryUsage(UINT64 used, UINT64 total) {
		const auto usedPercent = total == 0 ? 0.0f : static_cast<float>(used) / static_cast<float>(total) * 100.0f;

		std::ostringstream stream;
		stream << formatSize(used) << " / " << formatSize(total) << " (" << std::fixed << std::setprecision(2) << usedPercent << "%)";
		return stream.str();
	}

	std::string getErrorAsString(DWORD error) {
		if (error == 0) {
			return "";
		}

		LPSTR messageBuffer = nullptr;
		const size_t size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&messageBuffer),
			0,
			nullptr);

		std::string message(messageBuffer, size);
		LocalFree(messageBuffer);
		return sanitize(message);
	}

	const char* getExceptionAsString(DWORD exceptionCode) {
		switch (exceptionCode) {
		case EXCEPTION_ACCESS_VIOLATION: return "EXCEPTION_ACCESS_VIOLATION";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		case EXCEPTION_BREAKPOINT: return "EXCEPTION_BREAKPOINT";
		case EXCEPTION_DATATYPE_MISALIGNMENT: return "EXCEPTION_DATATYPE_MISALIGNMENT";
		case EXCEPTION_FLT_DENORMAL_OPERAND: return "EXCEPTION_FLT_DENORMAL_OPERAND";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
		case EXCEPTION_FLT_INEXACT_RESULT: return "EXCEPTION_FLT_INEXACT_RESULT";
		case EXCEPTION_FLT_INVALID_OPERATION: return "EXCEPTION_FLT_INVALID_OPERATION";
		case EXCEPTION_FLT_OVERFLOW: return "EXCEPTION_FLT_OVERFLOW";
		case EXCEPTION_FLT_STACK_CHECK: return "EXCEPTION_FLT_STACK_CHECK";
		case EXCEPTION_FLT_UNDERFLOW: return "EXCEPTION_FLT_UNDERFLOW";
		case EXCEPTION_ILLEGAL_INSTRUCTION: return "EXCEPTION_ILLEGAL_INSTRUCTION";
		case EXCEPTION_IN_PAGE_ERROR: return "EXCEPTION_IN_PAGE_ERROR";
		case EXCEPTION_INT_DIVIDE_BY_ZERO: return "EXCEPTION_INT_DIVIDE_BY_ZERO";
		case EXCEPTION_INT_OVERFLOW: return "EXCEPTION_INT_OVERFLOW";
		case EXCEPTION_INVALID_DISPOSITION: return "EXCEPTION_INVALID_DISPOSITION";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
		case EXCEPTION_PRIV_INSTRUCTION: return "EXCEPTION_PRIV_INSTRUCTION";
		case EXCEPTION_SINGLE_STEP: return "EXCEPTION_SINGLE_STEP";
		case EXCEPTION_STACK_OVERFLOW: return "EXCEPTION_STACK_OVERFLOW";
		default: return "UNKNOWN_EXCEPTION";
		}
	}

	std::string getRegistryString(HKEY key, const char* name) {
		char buffer[MAX_PATH] = {};
		DWORD size = sizeof(buffer);
		if (RegQueryValueExA(key, name, nullptr, nullptr, reinterpret_cast<BYTE*>(buffer), &size) == ERROR_SUCCESS) {
			return buffer;
		}

		return "Unknown";
	}

	std::string getCpuName() {
		HKEY key = nullptr;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key) != ERROR_SUCCESS) {
			return "<unknown>";
		}

		auto cpu = getRegistryString(key, "ProcessorNameString");
		RegCloseKey(key);

		cpu.erase(std::find_if(cpu.rbegin(), cpu.rend(), [](int ch) { return !std::isspace(ch); }).base(), cpu.end());
		return cpu;
	}

	std::string getWindowsVersion() {
		HKEY key = nullptr;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &key) != ERROR_SUCCESS) {
			return "<unknown>";
		}

		auto release = getRegistryString(key, "DisplayVersion");
		auto buildNumber = getRegistryString(key, "CurrentBuild");
		auto version = getRegistryString(key, "ProductName");
		RegCloseKey(key);

		try {
			const auto buildNumberInt = std::stoul(buildNumber);
			if (buildNumberInt >= 22000 && version.starts_with("Windows 10")) {
				version.replace(8, 2, "11");
			}
		}
		catch (...) {}

		return version + " - " + buildNumber + " (" + release + ")";
	}

	std::string getInstallPath() {
		char path[MAX_PATH] = {};
		if (GetModuleFileNameA(nullptr, path, MAX_PATH) <= 0) {
			return {};
		}

		return std::filesystem::canonical(path).parent_path().string();
	}

	std::string getModuleName(UINT32 eip, HANDLE process) {
		IMAGEHLP_MODULE module = {};
		module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
		if (!SymGetModuleInfo(process, eip, &module)) {
			return "";
		}

		return module.ModuleName;
	}

	UINT32 getModuleBase(UINT32 eip, HANDLE process) {
		IMAGEHLP_MODULE module = {};
		module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
		if (!SymGetModuleInfo(process, eip, &module)) {
			return 0;
		}

		return module.BaseOfImage;
	}

	std::string getSymbol(UINT32 eip, HANDLE process) {
		char symbolBuffer[sizeof(SYMBOL_INFO) + 255] = {};
		const auto symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);

		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = 254;

		DWORD64 offset = 0;
		if (!SymFromAddr(process, eip, &offset, symbol)) {
			return "";
		}

		std::ostringstream stream;
		stream << symbol->Name << "+0x" << std::hex << std::uppercase << offset;
		return stream.str();
	}

	std::string getLine(UINT32 eip, HANDLE process) {
		IMAGEHLP_LINE line = {};
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

		DWORD offset = 0;
		if (!SymGetLineFromAddr(process, eip, &offset, &line)) {
			return "";
		}

		std::ostringstream stream;
		stream << line.FileName << ":" << std::dec << line.LineNumber;
		return stream.str();
	}

	bool isReadableAddress(const void* pointer, size_t size) {
		MEMORY_BASIC_INFORMATION mbi = {};
		if (!VirtualQuery(pointer, &mbi, sizeof(mbi))) {
			return false;
		}

		constexpr DWORD readableMask = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
		if (mbi.State != MEM_COMMIT || (mbi.Protect & readableMask) == 0 || (mbi.Protect & PAGE_GUARD) || (mbi.Protect & PAGE_NOACCESS)) {
			return false;
		}

		return mbi.RegionSize >= size;
	}

	bool tryReadUInt32(const void* pointer, UINT32& value) {
		__try {
			if (!isReadableAddress(pointer, sizeof(UINT32))) {
				return false;
			}

			value = *reinterpret_cast<const volatile UINT32*>(pointer);
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return false;
		}
	}

	bool tryReadChar(const char* pointer, char& value) {
		__try {
			if (!isReadableAddress(pointer, sizeof(char))) {
				return false;
			}

			value = *pointer;
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return false;
		}
	}

	bool getStringForPointer(const void* pointer, std::string& output) {
		if (!isReadableAddress(pointer, 4)) {
			return false;
		}

		const auto cstr = static_cast<const char*>(pointer);
		std::string value;
		value.reserve(32);

		for (size_t length = 0; length < MAX_PATH; ++length) {
			char c = 0;
			if (!tryReadChar(cstr + length, c)) {
				return false;
			}

			if (c == '\0') {
				if (value.length() < 3) {
					return false;
				}

				output = "String: \"" + sanitize(value) + "\"";
				return true;
			}

			if (!se::string::is_printable(c)) {
				return false;
			}

			value.push_back(c);
		}

		return false;
	}

	bool getClassNameForPointer(const void* pointer, std::string& output) {
		UINT32 vtable = 0;
		if (!tryReadUInt32(pointer, vtable)) {
			return false;
		}

		auto symbol = getSymbol(vtable, GetCurrentProcess());
		if (symbol.empty()) {
			return false;
		}

		se::string::strip_start(symbol, "vtbl_");
		se::string::strip_start(symbol, "sg_");
		se::string::strip_end(symbol, "+0x0");
		se::string::strip_end(symbol, "+0x0");

		std::ostringstream stream;
		stream << "RTTI/PDB: " << symbol;
		output = stream.str();
		return true;
	}

	std::string describePointer(void* pointer) {
		if (!pointer) {
			return "";
		}

		std::string output;
		if (getStringForPointer(pointer, output) || getClassNameForPointer(pointer, output)) {
			return output;
		}

		return "";
	}

	std::string describeDereferenceChain(void* pointer) {
		std::ostringstream stream;
		auto current = pointer;
		for (UINT32 depth = 0; current && depth < dereferenceDepth; ++depth) {
			if (const auto description = describePointer(current); !description.empty()) {
				stream << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << reinterpret_cast<UINT32>(current) << " ==> " << description;
				return stream.str();
			}

			UINT32 next = 0;
			if (!tryReadUInt32(current, next)) {
				return "";
			}

			stream << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << next << " ==> ";
			current = reinterpret_cast<void*>(next);
		}

		return "";
	}

	bool isDataSectionNeeded(const WCHAR* modulePath) {
		if (!modulePath) {
			return false;
		}

		WCHAR fileName[_MAX_FNAME] = {};
		_wsplitpath_s(modulePath, nullptr, 0, nullptr, 0, fileName, _MAX_FNAME, nullptr, 0);

		for (const auto& module : currentSettings.dataSectionModules) {
			if (_wcsicmp(fileName, module.c_str()) == 0) {
				return true;
			}
		}

		return false;
	}

	BOOL CALLBACK miniDumpCallback(PVOID, const PMINIDUMP_CALLBACK_INPUT input, PMINIDUMP_CALLBACK_OUTPUT output) {
		if (!input || !output) {
			return FALSE;
		}

		switch (input->CallbackType) {
		case IncludeModuleCallback:
		case IncludeThreadCallback:
		case ThreadCallback:
		case ThreadExCallback:
			return TRUE;
		case MemoryCallback:
			return FALSE;
		case ModuleCallback:
			if ((output->ModuleWriteFlags & ModuleWriteDataSeg) && !isDataSectionNeeded(input->Module.FullPath)) {
				output->ModuleWriteFlags &= ~ModuleWriteDataSeg;
			}
			return TRUE;
		default:
			return FALSE;
		}
	}

	void writeHeader(std::ostream& log, const char* title) {
		log << "=== " << title << ": ";
		for (auto i = std::strlen(title); i < 116; ++i) {
			log << '=';
		}
		log << '\n';
	}

	void writeBasicInformation(EXCEPTION_POINTERS* info, std::ostream& log) {
		writeHeader(log, "BASIC INFORMATION");

#ifdef APPVEYOR_BUILD_NUMBER
		log << "Appveyor Build: " << APPVEYOR_BUILD_NUMBER << '\n';
#else
		log << "Appveyor Build: <unavailable>\n";
#endif
		if (!currentSettings.buildDate.empty()) {
			log << "Build Date: " << currentSettings.buildDate << '\n';
		}

		try {
			log << "Install Path: " << getInstallPath() << '\n';
		}
		catch (...) {
			log << "Install Path: <unavailable>\n";
		}

		const auto elapsed = std::chrono::system_clock::now() - startTime;
		const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
		log << "Runtime: " << (elapsedSeconds / 3600) << ':' << std::setw(2) << std::setfill('0') << ((elapsedSeconds / 60) % 60) << ':' << std::setw(2) << (elapsedSeconds % 60) << std::setfill(' ') << '\n';

		log << "Exception: " << getExceptionAsString(info->ExceptionRecord->ExceptionCode) << " (0x" << std::hex << std::uppercase << info->ExceptionRecord->ExceptionCode << std::dec << ")\n";

		const auto lastError = GetLastError();
		if (lastError) {
			log << "Last Error: " << getErrorAsString(lastError) << " (0x" << std::hex << std::uppercase << lastError << std::dec << ")\n";
		}

		auto threadName = se::windows::GetThreadDescription(GetCurrentThread());
		if (!threadName.has_value()) {
			threadName = L"<unsupported>";
		}
		else if (threadName.value().empty()) {
			threadName = L"<unknown>";
		}
		log << "Thread: " << se::string::from_wstring(threadName.value()) << '\n';

		PROCESS_MEMORY_COUNTERS_EX pmc = {};
		pmc.cb = sizeof(pmc);
		MEMORYSTATUSEX memoryStatus = {};
		memoryStatus.dwLength = sizeof(memoryStatus);
		GlobalMemoryStatusEx(&memoryStatus);
		if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
			const auto virtualUsage = memoryStatus.ullTotalVirtual - memoryStatus.ullAvailVirtual;
			log << "Physical Memory Usage: " << formatMemoryUsage(pmc.PrivateUsage, memoryStatus.ullTotalPhys) << '\n';
			log << "Virtual  Memory Usage: " << formatMemoryUsage(virtualUsage, memoryStatus.ullTotalVirtual) << '\n';
			if (pmc.PrivateUsage > 3650722201ull) {
				log << "Memory Warning: memory usage is high. Crash is likely due to running out of memory.\n";
			}
		}

		log << "OS:  " << getWindowsVersion() << '\n';
		log << "CPU: " << getCpuName() << '\n';
		if (currentSettings.gpuProvider) {
			log << "GPU: " << currentSettings.gpuProvider() << '\n';
		}
		else {
			log << "GPU: <unknown>\n";
		}

		ULONGLONG installedMemoryKb = 0;
		if (GetPhysicallyInstalledSystemMemory(&installedMemoryKb)) {
			log << "RAM: " << std::fixed << std::setprecision(2) << (installedMemoryKb / 1024.0f / 1024.0f) << " GB\n";
		}
	}

	struct StackEntry {
		UINT32 ebp = 0;
		std::string address;
		std::string name;
		std::string source;
	};

	void writeCallStack(EXCEPTION_POINTERS* info, std::ostream& log) {
		writeHeader(log, "CALL STACK");

		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();
		CONTEXT context = {};
		memcpy(&context, info->ContextRecord, sizeof(CONTEXT));

		STACKFRAME frame = {};
		frame.AddrPC.Offset = info->ContextRecord->Eip;
		frame.AddrPC.Mode = AddrModeFlat;
		frame.AddrFrame.Offset = info->ContextRecord->Ebp;
		frame.AddrFrame.Mode = AddrModeFlat;
		frame.AddrStack.Offset = info->ContextRecord->Esp;
		frame.AddrStack.Mode = AddrModeFlat;

		std::vector<StackEntry> entries;
		DWORD previousEip = 0;
		while (StackWalk(IMAGE_FILE_MACHINE_I386, process, thread, &frame, &context, nullptr, SymFunctionTableAccess, SymGetModuleBase, nullptr)) {
			if (frame.AddrPC.Offset == previousEip) {
				break;
			}
			previousEip = frame.AddrPC.Offset;

			StackEntry entry;
			entry.ebp = frame.AddrFrame.Offset;

			const auto moduleBase = getModuleBase(frame.AddrPC.Offset, process);
			const auto moduleOffset = moduleBase != 0x00400000 ? frame.AddrPC.Offset - moduleBase + 0x10000000 : frame.AddrPC.Offset;

			std::ostringstream address;
			if (const auto module = getModuleName(frame.AddrPC.Offset, process); module.empty()) {
				address << "??? (0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << moduleOffset << ")";
				entry.name = "(Corrupt stack or heap?)";
			}
			else {
				address << module << " (0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << moduleOffset << ")";
				entry.name = getSymbol(frame.AddrPC.Offset, process);
			}

			entry.address = address.str();
			entry.source = getLine(frame.AddrPC.Offset, process);
			entries.push_back(entry);
		}

		size_t addressLength = std::strlen("Function Address");
		size_t nameLength = std::strlen("Function Name");
		for (const auto& entry : entries) {
			addressLength = std::max(addressLength, entry.address.length());
			nameLength = std::max(nameLength, entry.name.length());
		}

		log << std::setw(10) << "EBP" << " | " << std::setw(static_cast<int>(addressLength)) << "Function Address" << " | " << std::left << std::setw(static_cast<int>(nameLength)) << "Function Name" << std::right << " | Source\n";
		for (const auto& entry : entries) {
			log << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << entry.ebp << std::setfill(' ') << " | "
				<< std::setw(static_cast<int>(addressLength)) << entry.address << " | "
				<< std::left << std::setw(static_cast<int>(nameLength)) << entry.name << std::right << " | "
				<< entry.source << '\n';
		}
	}

	void writeRegisters(EXCEPTION_POINTERS* info, std::ostream& log) {
		writeHeader(log, "REGISTERS");

		const std::pair<const char*, UINT32> registers[] = {
			{ "eax", info->ContextRecord->Eax },
			{ "ebx", info->ContextRecord->Ebx },
			{ "ecx", info->ContextRecord->Ecx },
			{ "edx", info->ContextRecord->Edx },
			{ "edi", info->ContextRecord->Edi },
			{ "esi", info->ContextRecord->Esi },
			{ "ebp", info->ContextRecord->Ebp },
			{ "esp", info->ContextRecord->Esp },
			{ "eip", info->ContextRecord->Eip },
		};

		log << "REG |    Value   | DEREFERENCE INFO\n";
		for (const auto& [name, value] : registers) {
			log << name << " | 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value << std::setfill(' ') << " | " << describeDereferenceChain(reinterpret_cast<void*>(value)) << '\n';
		}
	}

	void writeStack(EXCEPTION_POINTERS* info, std::ostream& log) {
		writeHeader(log, "STACK");

		std::map<UINT32, UINT32> seenValues;
		const auto esp = reinterpret_cast<UINT32*>(info->ContextRecord->Esp);
		log << "  # |    Value   | DEREFERENCE INFO\n";
		for (UINT32 i = 0; i < stackScanCount; ++i) {
			UINT32 value = 0;
			if (!tryReadUInt32(&esp[i], value)) {
				continue;
			}

			const auto description = describeDereferenceChain(reinterpret_cast<void*>(value));
			const auto seen = seenValues.find(value);
			if (i <= 0x8 || (!description.empty() && seen == seenValues.end())) {
				log << ' ' << std::hex << std::uppercase << std::setw(2) << i << " | 0x" << std::setw(8) << std::setfill('0') << value << std::setfill(' ') << " | ";
				if (seen == seenValues.end()) {
					log << description;
					seenValues.emplace(value, i);
				}
				else {
					log << "Identical to " << std::hex << std::uppercase << seen->second;
				}
				log << '\n';
			}
		}
	}

	void writeWarnings(std::ostream& log) {
		if (currentSettings.warningsPath.empty() || !std::filesystem::exists(currentSettings.warningsPath)) {
			return;
		}

		std::ifstream warnings(currentSettings.warningsPath);
		if (!warnings.is_open()) {
			return;
		}

		writeHeader(log, "WARNINGS");

		std::unordered_set<std::string> seenLines;
		std::string line;
		while (std::getline(warnings, line)) {
			if (line.empty()) {
				continue;
			}
			if (!seenLines.contains(line)) {
				log << line << '\n';
				seenLines.insert(line);
			}
		}
	}

	LONG WINAPI unhandledExceptionFilter(EXCEPTION_POINTERS* info) {
		const auto result = handleException(info);
		if (originalFilter) {
			return originalFilter(info);
		}
		return result;
	}

	void configure(Settings settings) {
		currentSettings = std::move(settings);
	}

	void initialize(Settings settings) {
		configure(std::move(settings));
		noteStartTime();
	}

	void noteStartTime() {
		startTime = std::chrono::system_clock::now();
	}

	void createMiniDump(EXCEPTION_POINTERS* info) {
		auto& log = getLog();
		const auto path = currentSettings.miniDumpPath.empty() ? "MiniDump.dmp" : currentSettings.miniDumpPath.c_str();
		const auto file = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (file == nullptr || file == INVALID_HANDLE_VALUE) {
			log << "MiniDump creation failed. Could not get file handle. Error: " << GetLastError() << std::endl;
			return;
		}

		MINIDUMP_EXCEPTION_INFORMATION exceptionInfo = {};
		exceptionInfo.ThreadId = GetCurrentThreadId();
		exceptionInfo.ExceptionPointers = info;
		exceptionInfo.ClientPointers = FALSE;

		MINIDUMP_CALLBACK_INFORMATION callbackInfo = {};
		callbackInfo.CallbackRoutine = reinterpret_cast<MINIDUMP_CALLBACK_ROUTINE>(miniDumpCallback);

		auto dumpType = MiniDumpWithDataSegs
			| MiniDumpWithHandleData
			| MiniDumpWithFullMemoryInfo
			| MiniDumpWithThreadInfo
			| MiniDumpWithUnloadedModules;

		if (currentSettings.createFullMiniDump) {
			dumpType |= MiniDumpWithFullMemory | MiniDumpWithIndirectlyReferencedMemory;
		}

		const auto result = MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			file,
			static_cast<MINIDUMP_TYPE>(dumpType),
			info ? &exceptionInfo : nullptr,
			nullptr,
			&callbackInfo);

		if (!result) {
			log << "MiniDump creation failed. Error: 0x" << std::hex << std::uppercase << GetLastError() << std::dec << std::endl;
		}
		else {
			log << "MiniDump creation successful." << std::endl;
		}

		CloseHandle(file);
	}

	void writeCrashLog(EXCEPTION_POINTERS* info) {
		auto& log = getLog();

		SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_ALLOW_ABSOLUTE_SYMBOLS);

		char workingDirectory[MAX_PATH] = {};
		char symbolPath[MAX_PATH] = {};
		char alternateSymbolPath[MAX_PATH] = {};
		GetCurrentDirectoryA(MAX_PATH, workingDirectory);
		GetEnvironmentVariableA("_NT_SYMBOL_PATH", symbolPath, MAX_PATH);
		GetEnvironmentVariableA("_NT_ALTERNATE_SYMBOL_PATH", alternateSymbolPath, MAX_PATH);

		const std::string lookupPath = std::string(workingDirectory) + ";" + symbolPath + ";" + alternateSymbolPath;
		const auto symbolsInitialized = SymInitialize(GetCurrentProcess(), lookupPath.c_str(), TRUE);
		if (!symbolsInitialized) {
			log << "Error initializing symbol store: " << GetLastError() << '\n';
		}

		try {
			writeBasicInformation(info, log);
		}
		catch (...) {
			log << "Failed to log basic information.\n";
		}

		try {
			writeCallStack(info, log);
		}
		catch (...) {
			log << "Failed to log call stack.\n";
		}

		try {
			writeRegisters(info, log);
		}
		catch (...) {
			log << "Failed to log registers.\n";
		}

		try {
			writeStack(info, log);
		}
		catch (...) {
			log << "Failed to log stack.\n";
		}

		if (currentSettings.extraLog) {
			try {
				currentSettings.extraLog(info, log);
			}
			catch (...) {
				log << "Failed to log extra crash information.\n";
			}
		}

		try {
			writeWarnings(log);
		}
		catch (...) {
			log << "Failed to log warnings.\n";
		}

		if (symbolsInitialized) {
			SymCleanup(GetCurrentProcess());
		}

		log.flush();
	}

	LONG handleException(EXCEPTION_POINTERS* info) {
		if (caughtCrash) {
			return EXCEPTION_EXECUTE_HANDLER;
		}

		caughtCrash = true;

		auto& log = getLog();
		log << std::dec << std::endl;
		if (!currentSettings.crashMessage.empty()) {
			log << currentSettings.crashMessage << std::endl;
		}

		if (currentSettings.beforeCrashLog) {
			currentSettings.beforeCrashLog(info);
		}

		createMiniDump(info);
		writeCrashLog(info);
		return EXCEPTION_CONTINUE_SEARCH;
	}

	bool installUnhandledExceptionFilter(Settings settings) {
		initialize(std::move(settings));
		originalFilter = SetUnhandledExceptionFilter(unhandledExceptionFilter);
		return true;
	}
}
