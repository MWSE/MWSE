#include "PatchIO.h"

#include "Log.h"
#include "MemoryUtil.h"

#include "TES3Cell.h"
#include "TES3UIMenuController.h"
#include "TES3VFXManager.h"
#include "TES3WorldController.h"

/*
raiseStdioFileLimit
OverrideDontThreadLoad

INI-related patches
ESM/ESP serialization if you choose to classify that as I/O
*/

namespace mwse::patch::io {
	//
	// Patch: Respect symbolic links.
	// 
	// Unlike most of the Win32 API, FindFirstFileA and FindNextFileA don't respect symbolic links and
	// instead return information about the link itself instead of its target.
	// 
	// This patch makes it return the file size of the target file, rather than the symlink itself (0).
	//

	static std::unordered_map<HANDLE, std::string> findFilePaths;
	static std::recursive_mutex findFileMutex;

	std::optional<std::string> getFindFilePath(HANDLE hFindFile) {
		findFileMutex.lock();
		const auto itt = findFilePaths.find(hFindFile);
		if (itt == findFilePaths.end()) {
			findFileMutex.unlock();
			return {};
		}

		findFileMutex.unlock();
		return itt->second;
	}

	void PatchFixSymlinkData(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData) {
		const auto path = getFindFilePath(hFindFile);
		if (!path) {
			return;
		}

		const auto fullPath = std::filesystem::path(path.value()) / lpFindFileData->cFileName;
		if (!std::filesystem::exists(fullPath)) {
			return;
		}

		const auto fileSize = std::filesystem::file_size(fullPath);
		lpFindFileData->nFileSizeHigh = unsigned int(fileSize / std::numeric_limits<unsigned int>::max());
		lpFindFileData->nFileSizeLow = unsigned int(fileSize);
	}

	HANDLE __stdcall PatchFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) {
		auto result = FindFirstFileA(lpFileName, lpFindFileData);
		if (result == INVALID_HANDLE_VALUE) {
			return result;
		}

		findFileMutex.lock();
		findFilePaths[result] = std::filesystem::path(lpFileName).parent_path().string();
		findFileMutex.unlock();

		// Check to see if it resolved to a symbolic link.
		if (lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT && lpFindFileData->dwReserved0 == IO_REPARSE_TAG_SYMLINK) {
			PatchFixSymlinkData(result, lpFindFileData);
		}

		return result;
	}

	BOOL __stdcall PatchFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData) {
		auto result = FindNextFileA(hFindFile, lpFindFileData);
		if (!result) {
			return result;
		}

		// Check to see if it resolved to a symbolic link.
		if (lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT && lpFindFileData->dwReserved0 == IO_REPARSE_TAG_SYMLINK) {
			PatchFixSymlinkData(hFindFile, lpFindFileData);
		}

		return result;
	}

	BOOL __stdcall PatchFindClose(HANDLE hFindFile) {
		findFileMutex.lock();
		const auto itt = findFilePaths.find(hFindFile);
		if (itt != findFilePaths.end()) {
			findFilePaths.erase(itt);
		}
		findFileMutex.unlock();

		return FindClose(hFindFile);
	}

	//
	// Patch: Fix issue when serializing no effects.
	//

	const auto TES3_SaveVisualEffects = reinterpret_cast<void(__thiscall*)(TES3::VFXManager*, TES3::GameFile*)>(0x469CC0);
	void __fastcall PatchSaveVisualEffects(TES3::VFXManager* vfxManager, DWORD _EDX_, TES3::GameFile* file) {
		// Ensure that we have serializable VFXs.
		for (const auto& vfx : vfxManager->vfxNodes) {
			if (vfx->effectObject && !vfx->createdFromNode && !vfx->expired) {
				// Call original code.
				TES3_SaveVisualEffects(vfxManager, file);
				return;
			}
		}
	}

	//
	// Patch: Do not load VFX with maxAge <= 0.001f from save games.
	//

	const auto TES3_VFXManager_createFromSaveData = reinterpret_cast<TES3::VFX * (__thiscall*)(TES3::VFXManager*, TES3::PhysicalObject*, TES3::Reference*, TES3::VFXSerialized*, float)>(0x468620);

	TES3::VFX* __fastcall PatchVFXManagerCreateFromSaveData(TES3::VFXManager* vfxManager, DWORD unused, TES3::PhysicalObject* effect, TES3::Reference* reference, TES3::VFXSerialized* serializedVFX, float verticalOffset) {
		// Do not load VFX with maxAge <= 0.001f, as they are persistent and may have accumulated in saves from before these fixes.
		if (serializedVFX->maxAge <= 0.001f) {
			return nullptr;
		}

		return TES3_VFXManager_createFromSaveData(vfxManager, effect, reference, serializedVFX, verticalOffset);
	}

	// Patch: Land textures loading/unloading flag array overflow bug. Increase array from 500 to 4096 elements and fix bounds checks.

	const unsigned short Land_LTEX_isLoaded_size = 4096;
	bool Land_LTEX_isLoaded[Land_LTEX_isLoaded_size];

	__declspec(naked) void PatchLandUnloadTexturesBoundsCheck() {
		__asm {
			// Replace index >= 500 and index != -1 with a single unsigned comparison against the new size.
			cmp ax, 4096 // immediate arg = Land_LTEX_isLoaded_size
			jnb $ + 0xAB
			nop
			nop
			nop
			nop
			nop
			nop
		}
	}
	const size_t PatchLandUnloadTexturesBoundsCheck_size = 0x10;

	__declspec(naked) void PatchLandLoadTexturesBoundsCheck() {
		__asm {
			// Replace index >= 500 and index != -1 with a single unsigned comparison against the new size.
			cmp cx, 4096 // immediate arg = Land_LTEX_isLoaded_size
			jnb $ + 0xF5
			nop
			nop
			nop
			nop
			nop
			nop
		}
	}
	const size_t PatchLandLoadTexturesBoundsCheck_size = 0x11;

	//
	// Patch: Resolve node count mismatch when loading pathgrid records with missing subrecords.
	// 

	__declspec(naked) void PatchPathGridLoader() {
		__asm {
			pop esi
			pop ebx
			mov ecx, ebp
			nop			// Replace with call
			nop
			nop
			nop
			nop
			jmp $ + 0x15
		}
	}
	const size_t PatchPathGridLoader_size = 0xE;

	void __fastcall PatchPathGridLoaderCheckNodeData(TES3::PathGrid* pathGrid) {
		// Check node count from record matches node data. Reset node count on mismatch.
		if (pathGrid->nodeCount != pathGrid->nodes.count) {
			log::getLog() << "[MWSE] Warning: Pathgrid in cell '" << pathGrid->parentCell->getEditorName() <<
				"' has mismatching path node count. nodeCount=" << pathGrid->nodeCount << ", node data count=" << pathGrid->nodes.count << std::endl;

			pathGrid->nodeCount = static_cast<unsigned short>(pathGrid->nodes.count);
		}

		// Perform overwritten code.
		if (TES3::WorldController::get()->menuController->gameplayFlags & 0x800000) {
			pathGrid->show();
		}
	}

	void install() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;
		using se::memory::writeDoubleWordUnprotected;
		using se::memory::writeValueEnforced;
		using se::memory::writePatchCodeUnprotected;

		// Patch: Respect targets when searching for symlinks.
		writeDoubleWordUnprotected(0x746114, reinterpret_cast<DWORD>(&PatchFindFirstFileA));
		writeDoubleWordUnprotected(0x746118, reinterpret_cast<DWORD>(&PatchFindNextFileA));
		writeDoubleWordUnprotected(0x74611C, reinterpret_cast<DWORD>(&PatchFindClose));

		// Patch: Don't save VFX manager if there are no valid visual effects.
		genCallEnforced(0x4BD149, 0x469CC0, reinterpret_cast<DWORD>(PatchSaveVisualEffects));

		// Patch: Ensure VFX with maxAge <= 0.001f are cleared when clearing data on load game, instead of leaking.
		auto VFXManager_reset = &TES3::VFXManager::reset;
		genCallEnforced(0x4C6F00, 0x469390, *reinterpret_cast<DWORD*>(&VFXManager_reset));

		// Patch: Do not load VFX with maxAge <= 0.001f from save games.
		genCallEnforced(0x46A04B, 0x468620, reinterpret_cast<DWORD>(PatchVFXManagerCreateFromSaveData));

		// Patch: LTEX loading/unloading array overflow bug. Increase array from 500 to 4096 elements and fix bounds checks.
		writeValueEnforced(0x4CDF09, 500 / 4, Land_LTEX_isLoaded_size / 4);
		writeValueEnforced(0x4CDF0E, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));
		writePatchCodeUnprotected(0x4CDF58, (BYTE*)&PatchLandUnloadTexturesBoundsCheck, PatchLandUnloadTexturesBoundsCheck_size);
		writeValueEnforced(0x4CDF6D, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));
		writeValueEnforced(0x4CDF7E, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));

		writeValueEnforced(0x4CECAE, 500 / 4, Land_LTEX_isLoaded_size / 4);
		writeValueEnforced(0x4CECB3, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));
		writePatchCodeUnprotected(0x4CECD3, (BYTE*)&PatchLandLoadTexturesBoundsCheck, PatchLandLoadTexturesBoundsCheck_size);
		writeValueEnforced(0x4CECE9, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));
		writeValueEnforced(0x4CEDB1, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));

		// Patch: Resolve node count mismatch when loading pathgrid records with missing subrecords.
		writePatchCodeUnprotected(0x4F444E, (BYTE*)&PatchPathGridLoader, PatchPathGridLoader_size);
		genCallUnprotected(0x4F444E + 4, reinterpret_cast<DWORD>(PatchPathGridLoaderCheckNodeData));
	}
}
