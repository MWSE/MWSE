#include "CrashLogger.h"

namespace CrashLogger::Device {
	static std::stringstream output;

	static std::string GetRegistryString(HKEY key, const char* name) {
		char buffer[MAX_PATH] = {};
		DWORD size = sizeof(buffer);
		if (RegQueryValueExA(key, name, nullptr, nullptr, reinterpret_cast<BYTE*>(buffer), &size) == ERROR_SUCCESS) {
			return buffer;
		}
		return "Unknown";
	}

	void Process(EXCEPTION_POINTERS* info) {
		try {
			std::string cpu = "<unknown>";
			HKEY cpuKey = nullptr;
			if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &cpuKey) == ERROR_SUCCESS) {
				cpu = GetRegistryString(cpuKey, "ProcessorNameString");
				RegCloseKey(cpuKey);
			}

			std::string version;
			std::string buildNumber;
			std::string release;
			HKEY windowsKey = nullptr;
			if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &windowsKey) == ERROR_SUCCESS) {
				release = GetRegistryString(windowsKey, "DisplayVersion");
				buildNumber = GetRegistryString(windowsKey, "CurrentBuild");
				version = GetRegistryString(windowsKey, "ProductName");

				const auto buildNumberValue = std::stoul(buildNumber);
				if (buildNumberValue >= 22000 && version.size() > 9) {
					version.replace(9, 1, "1");
				}
				RegCloseKey(windowsKey);
			}

			ULONGLONG memoryAmount = 0;
			GetPhysicallyInstalledSystemMemory(&memoryAmount);
			cpu.erase(std::find_if(cpu.rbegin(), cpu.rend(), [](int character) {
				return !std::isspace(character);
			}).base(), cpu.end());

			output << fmt::format("OS:  {} - {} ({})", version, buildNumber, release) << '\n';
			output << fmt::format("CPU: {}", cpu) << '\n';
			output << fmt::format("GPU: {}", Client::GetGPU()) << '\n';
			output << fmt::format("RAM: {:>5.2f} GB", memoryAmount / 1024.0f / 1024.0f) << '\n';
		}
		catch (...) {
			output << "Failed to process device info.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}
