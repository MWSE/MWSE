#pragma once

#include "MemoryUtil.h"
#include "TES3FileLoadSource.h"

namespace se::cs {
	struct Archive {
		struct CaseInsensitiveFileHash {
			DWORD unknown_0x0;
			DWORD unknown_0x4;
		};
		struct OffsetSizeData {
			unsigned int fileSize; // 0x0
			unsigned int offsetInArchive; // 0x4

			OffsetSizeData() = delete;
			~OffsetSizeData() = delete;
		};
		BYTE baseClass[0xB0];
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
		BYTE criticalSection[0x24];

		Archive() = delete;
		~Archive() = delete;
	};
	static_assert(sizeof(Archive) >= 0x178, "Archive failed size validation");
	static_assert(sizeof(Archive::CaseInsensitiveFileHash) >= 0x8, "Archive::FileHash failed size validation");

	struct BSALoader {
		Archive* lastLoadedArchive; // 0x0
		Archive* archiveIterator; // 0x4

		BSALoader() = delete;
		~BSALoader() = delete;
	};
	static_assert(sizeof(BSALoader) >= 0x8, "BSALoader failed size validation");

	using gBSALoader = memory::ExternalGlobal<BSALoader*, 0x6D1F9C>;
}