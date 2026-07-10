#include "CrashLogger.h"

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A000010

namespace CrashLogger::Memory {
	static std::stringstream output;

	void Process(EXCEPTION_POINTERS* info) {
		try {
			const auto process = GetCurrentProcess();

			PROCESS_MEMORY_COUNTERS_EX2 counters = {};
			counters.cb = sizeof(counters);

			MEMORYSTATUSEX memoryStatus = {};
			memoryStatus.dwLength = sizeof(memoryStatus);
			GlobalMemoryStatusEx(&memoryStatus);
			if (GetProcessMemoryInfo(process, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&counters), sizeof(counters))) {
				const auto virtualUsage = memoryStatus.ullTotalVirtual - memoryStatus.ullAvailVirtual;
				const auto physicalUsage = counters.PrivateUsage;
				output << fmt::format("Physical Memory Usage: {}\n", GetMemoryUsageString(physicalUsage, memoryStatus.ullTotalPhys));
				output << fmt::format("Virtual  Memory Usage: {}\n", GetMemoryUsageString(virtualUsage, memoryStatus.ullTotalVirtual));
			}
		}
		catch (...) {
			output << "Failed to log memory.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}
