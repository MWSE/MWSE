#include "CrashLogger.hpp"

#include "NIDX8Renderer.h"

namespace CrashLogger::Device {
	std::stringstream output;

	std::string GetRegistryString(HKEY key, const char* name) {
		char buffer[MAX_PATH] = {};
		DWORD size = sizeof(buffer);
		if (RegQueryValueExA(key, name, nullptr, nullptr, (BYTE*)buffer, &size) == ERROR_SUCCESS)
			return buffer;
		return "Unknown";
	}

	static std::string getGPU() {
		const auto game = TES3::Game::get();
		if (!game) return "<unknown>";
		if (!game->renderer) return "<unknown>";

		const auto adapter = game->renderer->getCurrentAdapter();
		if (!adapter) return "<unknown>";

		return adapter->identifier.Description;
	}

	extern void Process(EXCEPTION_POINTERS* info) {
		try {
			std::string cpu = "<unknown>";
			{
				HKEY key;
				if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key) == ERROR_SUCCESS)
				{
					cpu = GetRegistryString(key, "ProcessorNameString");
					RegCloseKey(key);
				}
			}

			std::string gpu = getGPU();

			std::string version;
			std::string buildNumber;
			std::string release;
			{
				HKEY key;
				if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &key) == ERROR_SUCCESS) {
					release = GetRegistryString(key, "DisplayVersion");
					buildNumber = GetRegistryString(key, "CurrentBuild");
					version = GetRegistryString(key, "ProductName");

					UINT32 buildNumberInt = std::stoul(buildNumber);
					if (buildNumberInt >= 22000) {
						// Aaahhh i'm a schizo
						version.replace(9, 1, "1");
					}

					RegCloseKey(key);
				}
			}

			ULONGLONG memAmount;
			GetPhysicallyInstalledSystemMemory(&memAmount);

			// Trim the empty space at the end of the CPU string
			cpu.erase(std::find_if(cpu.rbegin(), cpu.rend(), [](int ch) { return !std::isspace(ch); }).base(), cpu.end());

			output << fmt::format("OS:  {} - {} ({})", version, buildNumber, release) << '\n';
			output << fmt::format("CPU: {}", cpu) << '\n';
			output << fmt::format("GPU: {}", gpu) << '\n';
			output << fmt::format("RAM: {:>5.2f} GB", memAmount / 1024.f / 1024.f) << '\n';

		}
		catch (...) {
			output << "Failed to process device info." << '\n';
		}
	}

	extern std::stringstream& Get() {
		output.flush();
		return output;
	}
}