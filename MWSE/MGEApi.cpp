#include "MGEApi.h"

namespace mge {
	static mge::VersionStruct loadMGEGUIVersion() {
		DWORD dwSize = GetFileVersionInfoSize("MGEXEgui.exe", NULL);
		if (dwSize == 0) {
			return {};
		}

		BYTE* pbVersionInfo = new BYTE[dwSize];
		if (!GetFileVersionInfo("MGEXEgui.exe", 0, dwSize, pbVersionInfo)) {
			delete[] pbVersionInfo;
			return {};
		}

		VS_FIXEDFILEINFO* pFileInfo = NULL;
		UINT puLenFileInfo = 0;
		if (!VerQueryValue(pbVersionInfo, TEXT("\\"), (LPVOID*)&pFileInfo, &puLenFileInfo)) {
			delete[] pbVersionInfo;
			return {};
		}

		mge::VersionStruct version = {};
		version.major = BYTE(HIWORD(pFileInfo->dwProductVersionMS));
		version.minor = BYTE(LOWORD(pFileInfo->dwProductVersionMS));
		version.patch = BYTE(LOWORD(pFileInfo->dwProductVersionLS >> 16));
		version.build = BYTE(HIWORD(pFileInfo->dwProductVersionLS >> 16));
		delete[] pbVersionInfo;

		return version;
	}

	VersionStruct guiVersion = loadMGEGUIVersion();
}
