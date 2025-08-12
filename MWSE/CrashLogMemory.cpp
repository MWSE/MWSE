#include "CrashLogger.hpp"

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A000010

namespace CrashLogger::Memory {
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info) {
		try {
			const auto hProcess = GetCurrentProcess();

			PROCESS_MEMORY_COUNTERS_EX2 pmc = {};
			pmc.cb = sizeof(pmc);

			// Get physical memory size
			MEMORYSTATUSEX memoryStatus;
			memoryStatus.dwLength = sizeof(memoryStatus);
			GlobalMemoryStatusEx(&memoryStatus);
			if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
				DWORDLONG virtUsage = memoryStatus.ullTotalVirtual - memoryStatus.ullAvailVirtual;
				DWORDLONG physUsage = pmc.PrivateUsage;
				output << fmt::format("Physical Memory Usage: {}\n", GetMemoryUsageString(physUsage, memoryStatus.ullTotalPhys));
				output << fmt::format("Virtual  Memory Usage: {}\n", GetMemoryUsageString(virtUsage, memoryStatus.ullTotalVirtual));
			}
		}
		catch (...) {
			output << "Failed to log memory." << '\n';
		}
	}

	extern std::stringstream& Get() {
		output.flush();
		return output;
	}
}