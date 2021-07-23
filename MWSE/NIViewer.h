#pragma once

namespace NI {
	struct Viewer {
		static constexpr auto createViewerSectionText = reinterpret_cast<char* (__cdecl*)(const char*)>(0x6FCFD0);

		static constexpr auto createViewerFloat = reinterpret_cast<char* (__cdecl*)(const char*, float)>(0x6FD1B0);
		static constexpr auto createViewerString = reinterpret_cast<char* (__cdecl*)(const char*, const char*)>(0x6FD230);
	};
}
