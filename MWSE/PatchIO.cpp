#include "PatchIO.h"

#include "Log.h"
#include "MemoryUtil.h"

#include "TES3Cell.h"
#include "TES3DataHandler.h"
#include "TES3GameFile.h"
#include "TES3UIMenuController.h"
#include "TES3VFXManager.h"
#include "TES3WorldController.h"

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

	//
	// Patch: Land textures loading/unloading flag array overflow bug. Increase array from 500 to 4096 elements and fix bounds checks.
	//

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

	//
	// Patch: Optimize DontThreadLoad, prevent multi-thread loading from lua.
	//
	// Every time the game wants to load, it checks the ini file from disk for the DontThreadLoad value.
	// This patch caches the value so it only needs to be read once.
	//
	// Additionally, this provides a way to suppress thread loading from lua, if it is causing an issue in
	// a script (namely, a lua state deadlock).
	//

	UINT WINAPI	OverrideDontThreadLoad(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName) {
		return TES3::DataHandler::suppressThreadLoad || TES3::DataHandler::dontThreadLoad;
	}


	//
	// Patch: Support loading existing moved references.
	//
	// The following records have been modified:
	//  - CELL.FRMR
	//  - CELL.MVRF
	//  - REFR.FRMR
	//  - SCPT.RNAM
	//

#if MWSE_RAISED_FILE_LIMIT
	namespace PatchRaiseESXLimit {
		// Vanilla offsets and masks.
		constexpr DWORD ModBitsVanilla = 8;
		constexpr DWORD FormBitsVanilla = sizeof(DWORD) * CHAR_BIT - ModBitsVanilla;
		constexpr DWORD ModMaskVanilla = ((1 << ModBitsVanilla) - 1) << FormBitsVanilla;
		constexpr DWORD FormMaskVanilla = (1 << FormBitsVanilla) - 1;
		constexpr DWORD ModCountVanilla = 1 << ModBitsVanilla;

		// New offsets and masks.
		constexpr DWORD ModBitsMWSE = 10;
		constexpr DWORD FormBitsMWSE = sizeof(DWORD) * CHAR_BIT - ModBitsMWSE;
		constexpr DWORD ModMaskMWSE = ((1 << ModBitsMWSE) - 1) << FormBitsMWSE;
		constexpr DWORD FormMaskMWSE = (1 << FormBitsMWSE) - 1;
		constexpr DWORD ModCountMWSE = 1 << ModBitsMWSE;
		constexpr DWORD InvalidFormId = 0xFFFFFFFF;
		static_assert(1 << ModBitsMWSE == sizeof(TES3::NonDynamicData::activeMods) / sizeof(TES3::GameFile*), "Reference FormID bit assignment does not match active game file array size.");

		struct SerializedFormId {
			DWORD modIndex; // 0x0
			DWORD formId; // 0x4
		};

		void __fastcall LoadFormId(TES3::GameFile* file, DWORD edx, DWORD* out_movedFormId, size_t size) {
			// Loading the new format?
			SerializedFormId data;
			if (file->currentChunkHeader.size == sizeof(SerializedFormId)) {
				file->readChunkData(&data);

				// Handle saves where an invalid form ID was incorrectly serialized.
				// This does have the edge case if mod 1023 has a more than ~4.2 million form IDs. Extremely unlikely.
				if (data.modIndex == ModCountMWSE - 1 && data.formId == FormMaskMWSE) {
					*out_movedFormId = InvalidFormId;
					return;
				}
			}
			else {
				// If it's not the new format, we need to convert.
				DWORD oldFormId = 0;
				file->readChunkData(&oldFormId);

				// Preserve invalid form IDs.
				if (oldFormId == InvalidFormId) {
					*out_movedFormId = InvalidFormId;
					return;
				}

				data.modIndex = (oldFormId >> FormBitsVanilla);
				data.formId = (oldFormId & FormMaskVanilla);
			}

			*out_movedFormId = (data.modIndex << FormBitsMWSE) + data.formId;
		}

		void __fastcall SaveFormId(TES3::GameFile* file, DWORD edx, unsigned int tag, DWORD* movedRefId, size_t size) {
			// Preserve invalid form IDs.
			if (*movedRefId == InvalidFormId) {
				DWORD refId = InvalidFormId;
				file->writeChunkValue(tag, refId);
				return;
			}

			// Split out the bitmasked field.
			SerializedFormId data = {};
			data.modIndex = *movedRefId >> FormBitsMWSE;
			data.formId = *movedRefId & FormMaskMWSE;

			// If the mod index is higher than the vanilla limit, save the new format.
			if (data.modIndex >= ModCountVanilla) {
				file->writeChunkValue(tag, data);
			}
			// If the mod index is below the vanilla limit, use the vanilla save format and masks for compatibility.
			else {
				DWORD refId = (data.modIndex << FormBitsVanilla) + data.formId;
				file->writeChunkValue(tag, refId);
			}
		}
	}
#endif

	//
	// Helper function for raised mod limit.
	//
	// Raise C runtime fopen limit from 512 to 2048. This covers the case where all mods are open during game load.
	// Otherwise, fopen will fail and Morrowind will ignore the error, causing issues.
	//
	bool raiseStdioFileLimit() {
		// Use stdio function from Morrowind's C runtime.
		HINSTANCE hMSVCRT = GetModuleHandleA("msvcrt.dll");
		if (hMSVCRT != NULL) {
			auto msvcrt_setmaxstdio = reinterpret_cast<int(*)(int)>(GetProcAddress(hMSVCRT, "_setmaxstdio"));
			if (msvcrt_setmaxstdio(2048) == 2048) {
				return true;
			}
			else {
				mwse::log::getLog() << "MWSE_RAISED_FILE_LIMIT: msvcrt_setmaxstdio(2048) failed." << std::endl;
			}
		}
		else {
			mwse::log::getLog() << "MWSE_RAISED_FILE_LIMIT: GetModuleHandleA(\"msvcrt.dll\") failed." << std::endl;
		}
		return false;
	}

	void install() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;
		using se::memory::genJumpEnforced;
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

		// Patch: Decrease MO2 load times. Somehow...
		writeDoubleWordUnprotected(0x7462F4, reinterpret_cast<DWORD>(&_stat32));

		// Patch: Cache DontThreadLoad INI value and extend it with a suppression flag.
		TES3::DataHandler::dontThreadLoad = GetPrivateProfileIntA("General", "DontThreadLoad", 0, ".\\Morrowind.ini") != 0;
		genCallUnprotected(0x48539C, reinterpret_cast<DWORD>(OverrideDontThreadLoad), 0x6);
		genCallUnprotected(0x4869DB, reinterpret_cast<DWORD>(OverrideDontThreadLoad), 0x6);
		genCallUnprotected(0x48F489, reinterpret_cast<DWORD>(OverrideDontThreadLoad), 0x6);
		genCallUnprotected(0x4904D0, reinterpret_cast<DWORD>(OverrideDontThreadLoad), 0x6);

#if MWSE_RAISED_FILE_LIMIT
		// Patch: Raise esm/esp limit from 256 to 1024.

		// Change hardcoded 256 checks to 1024.
		writeValueEnforced<DWORD>(0x4B7A22 + 0x1, PatchRaiseESXLimit::ModCountVanilla, PatchRaiseESXLimit::ModCountMWSE);
		if (raiseStdioFileLimit()) {
			// Actually only allow loading more than 256 mods if we were able to raise the fopen limit.
			writeValueEnforced<DWORD>(0x4BB4AE + 0x3, PatchRaiseESXLimit::ModCountVanilla, PatchRaiseESXLimit::ModCountMWSE);
			writeValueEnforced<DWORD>(0x4BB588 + 0x3, PatchRaiseESXLimit::ModCountVanilla, PatchRaiseESXLimit::ModCountMWSE);
		}

		// Fix accesses into the active mods list to point to the new array.
		writeValueEnforced<DWORD>(0x4B7A27 + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4B87A9 + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BB498 + 0x3, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BB56F + 0x3, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BB5ED + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BB650 + 0x3, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BBD21 + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BD252 + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4C8B92 + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));

		// Change of form ID: 8 bit to 10 bit game file mask.
		writeValueEnforced<BYTE>(0x4DD03F + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x4DD2A7 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x4DD31E + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x4DD813 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x4DDA09 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x4DDBB1 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x7367A0 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x736809 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x73685A + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x736890 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x7368D7 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x736B56 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x736B75 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<DWORD>(0x4B54DD + 0x1, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x4DD030 + 0x1, PatchRaiseESXLimit::ModMaskVanilla, PatchRaiseESXLimit::ModMaskMWSE);
		writeValueEnforced<DWORD>(0x4DD089 + 0x1, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x4DD107 + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x4DD80B + 0x2, PatchRaiseESXLimit::ModMaskVanilla, PatchRaiseESXLimit::ModMaskMWSE);
		writeValueEnforced<DWORD>(0x4DD829 + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x4E0C8B + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x4E0C91 + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x7367A3 + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x73680C + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x736B78 + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);

		// Patch loading to support either the old or new format.
		genCallEnforced(0x4C01B1, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x4DCE01, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x4DD027, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x4DE197, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x4E0C2F, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x4E0C6D, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genJumpEnforced(0x7367BA, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x736B48, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));

		// Patch saving to try to use the old format if possible, and use the new format if it can't.
		genCallEnforced(0x4E1144, 0x4B6BA0, reinterpret_cast<DWORD>(PatchRaiseESXLimit::SaveFormId));
		genCallEnforced(0x4E14D5, 0x4B6BA0, reinterpret_cast<DWORD>(PatchRaiseESXLimit::SaveFormId));
		genCallEnforced(0x4E1B15, 0x4B6BA0, reinterpret_cast<DWORD>(PatchRaiseESXLimit::SaveFormId));
		genCallEnforced(0x4E1E78, 0x4B6BA0, reinterpret_cast<DWORD>(PatchRaiseESXLimit::SaveFormId));
		genCallEnforced(0x4FFB78, 0x4B6BA0, reinterpret_cast<DWORD>(PatchRaiseESXLimit::SaveFormId));
#endif
	}
}
