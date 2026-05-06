#pragma once

#include "TES3Defines.h"
#include "TES3HashMap.h"
#include "TES3Stream.h"

#include "TES3CriticalSection.h"

namespace TES3 {
	namespace FileLoadSource {
		enum FileLoadSource : int {
			Missing,
			File,
			BSA,
		};
	}

	struct BSALoader {
		Archive* lastLoadedArchive; // 0x0
		Archive* archiveIterator; // 0x4

		BSALoader() = delete;
		~BSALoader() = delete;

		bool findFile(const char* path, unsigned int* out_index = nullptr);

		//
		// Custom functions.
		//

		// A custom findFile that allows retrieving the found archive.
		bool findFile(const char* path, unsigned int& out_index, Archive** out_archive);

		sol::table getArchives(sol::this_state ts) const;
		sol::optional<std::tuple<Archive*, unsigned int>> findFile_lua(const char* path);

		static BSALoader* get();
	};
	static_assert(sizeof(BSALoader) >= 0x8, "TES3::BSALoader failed size validation");

	struct Archive : Stream {
		struct CaseInsensitiveFileHash {
			DWORD unknown_0x0;
			DWORD unknown_0x4;

			CaseInsensitiveFileHash(const char* path);
		};
		struct OffsetSizeData {
			unsigned int fileSize; // 0x0
			unsigned int offsetInArchive; // 0x4

			OffsetSizeData() = delete;
			// Destructible for template interoperability; instances remain engine-owned.
			~OffsetSizeData() = default;
		};
		char path[128]; // 0xB0
		unsigned int offsetToFileNameHashes; // 0x130
		unsigned int fileCount; // 0x134
		unsigned int lastFoundFileIndex; // 0x138
		void* niFile; // 0x13C
		OffsetSizeData* fileOffsetSizeData; // 0x140
		CaseInsensitiveFileHash* fileHashes; // 0x144
		const char** unknown_0x148; // Typically deleted after initial load to preserve memory.
		const char** unknown_0x14C; // Typically deleted after initial load to preserve memory.
		Archive* nextArchive; // 0x150
		CriticalSection criticalSection; // 0x154

		Archive() = delete;
		~Archive() = delete;

		bool findFileHash(CaseInsensitiveFileHash& hash, unsigned int& out_result, const char* validationPath = nullptr);

		//
		// Custom functions.
		//

		sol::optional<unsigned int> findFileIndex_lua(const char* path);

		nonstd::span<OffsetSizeData> getFileOffsetSizeDataSpan() const;
		nonstd::span<CaseInsensitiveFileHash> getFileHashesSpan() const;
	};
	static_assert(sizeof(Archive) >= 0x178, "TES3::Archive failed size validation");
	static_assert(sizeof(Archive::CaseInsensitiveFileHash) >= 0x8, "TES3::Archive::FileHash failed size validation");
}
