#pragma once

namespace NI {
	struct Viewer {
#if (defined(SE_IS_MWSE) && SE_IS_MWSE == 1) || defined(SE_IS_MGE) && SE_IS_MGE == 1
		static constexpr auto createViewerSectionText = reinterpret_cast<char*(__cdecl*)(const char*)>(0x6FCFD0);
		static constexpr auto createViewerFloat = reinterpret_cast<char*(__cdecl*)(const char*, float)>(0x6FD1B0);
		static constexpr auto createViewerString = reinterpret_cast<char*(__cdecl*)(const char*, const char*)>(0x6FD230);
#endif
	};
}
