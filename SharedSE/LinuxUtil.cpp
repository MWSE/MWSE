#include "LinuxUtil.h"

namespace se::cs::linux {
	bool isRunningWine() {
		return getWineVersion() != nullptr;
	}

	const char* getWineVersion() {
		HMODULE ntdll = GetModuleHandle("ntdll.dll");
		if (!ntdll) {
			return nullptr;
		}

		const auto wine_get_version = reinterpret_cast<const char* (__cdecl*)()>(GetProcAddress(ntdll, "wine_get_version"));
		if (!wine_get_version) {
			return nullptr;
		}

		return wine_get_version();
	}
}
