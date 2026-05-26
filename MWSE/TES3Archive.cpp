#include "TES3Archive.h"

namespace TES3 {

	//
	// TES3::BSALoader
	//

	BSALoader* BSALoader::get() {
		return *reinterpret_cast<BSALoader**>(0x7C9F74);
	}

	const auto TES3_BSALoader_findFile = reinterpret_cast<bool(__thiscall*)(BSALoader*, const char*, unsigned int*)>(0x4936A0);
	bool BSALoader::findFile(const char* path, unsigned int* out_thing) {
		return TES3_BSALoader_findFile(this, path, out_thing);
	}

	bool BSALoader::findFile(const char* path, unsigned int& out_index, Archive** out_archive) {
		Archive::CaseInsensitiveFileHash hash = path;
		for (auto archive = lastLoadedArchive; archive; archive = archive->nextArchive) {
			if (archive->findFileHash(hash, out_index)) {
				if (out_archive) {
					*out_archive = archive;
				}
				return true;
			}
		}
		return false;
	}

	sol::table BSALoader::getArchives(sol::this_state ts) const {
		sol::state_view state = ts;
		sol::table archives = state.create_table();
		for (auto archive = lastLoadedArchive; archive; archive = archive->nextArchive) {
			archives.add(archive);
		}
		return archives;
	}

	sol::optional<std::tuple<Archive*, unsigned int>> BSALoader::findFile_lua(const char* path) {
		unsigned int index = -1;
		Archive* foundArchive = nullptr;
		if (findFile(path, index, &foundArchive)) {
			return { { foundArchive, index } };
		}
		return {};
	}

	//
	// TES3::Archive
	//

	const auto TES3_Archive_CaseInsensitiveFileHash_ctor = reinterpret_cast<void(__thiscall*)(Archive::CaseInsensitiveFileHash*, const char*)>(0x494380);
	Archive::CaseInsensitiveFileHash::CaseInsensitiveFileHash(const char* path) {
		TES3_Archive_CaseInsensitiveFileHash_ctor(this, path);
	}

	const auto TES3_Archive_findFileHash = reinterpret_cast<bool(__thiscall*)(Archive*, Archive::CaseInsensitiveFileHash&, unsigned int&, const char*)>(0x493B20);
	bool Archive::findFileHash(CaseInsensitiveFileHash& hash, unsigned int& out_result, const char* validationPath) {
		return TES3_Archive_findFileHash(this, hash, out_result, validationPath);
	}

	sol::optional<unsigned int> Archive::findFileIndex_lua(const char* path) {
		CaseInsensitiveFileHash hash = path;
		unsigned int index = -1;
		if (findFileHash(hash, index, path)) {
			return index;
		}
		return {};
	}

	std::span<Archive::OffsetSizeData> Archive::getFileOffsetSizeDataSpan() const {
		return std::span(fileOffsetSizeData, fileCount);
	}

	std::span<Archive::CaseInsensitiveFileHash> Archive::getFileHashesSpan() const {
		return std::span(fileHashes, fileCount);
	}
}
