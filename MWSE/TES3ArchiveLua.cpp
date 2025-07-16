#include "TES3ArchiveLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Archive.h"

namespace mwse::lua {
	void bindTES3Archive() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Bindings for TES3::BSALoader
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::BSALoader>("tes3bsaLoader");

			// Property bindings.
			usertypeDefinition["archives"] = sol::readonly_property(&TES3::BSALoader::getArchives);
			usertypeDefinition["firstArchive"] = sol::readonly_property(&TES3::BSALoader::lastLoadedArchive);

			// Function bindings.
			usertypeDefinition["findFile"] = &TES3::BSALoader::findFile_lua;
		}

		// Bindings for TES3::Archive::OffsetSizeData
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Archive::OffsetSizeData>("tes3archiveOffsetSizeData");

			// Property bindings.
			usertypeDefinition["offset"] = sol::readonly_property(&TES3::Archive::OffsetSizeData::offsetInArchive);
			usertypeDefinition["size"] = sol::readonly_property(&TES3::Archive::OffsetSizeData::fileSize);
		}

		// Bindings for TES3::Archive
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Archive>("tes3archive");

			// Property bindings.
			usertypeDefinition["fileCount"] = sol::readonly_property(&TES3::Archive::fileCount);
			usertypeDefinition["nextArchive"] = sol::readonly_property(&TES3::Archive::nextArchive);
			usertypeDefinition["offsetToFileNameHashes"] = sol::readonly_property(&TES3::Archive::offsetToFileNameHashes);
			usertypeDefinition["path"] = sol::readonly_property(&TES3::Archive::path);
			usertypeDefinition["sizesAndOffsets"] = sol::readonly_property(&TES3::Archive::getFileOffsetSizeDataSpan);

			// Function bindings.
			usertypeDefinition["findFileIndex"] = &TES3::Archive::findFileIndex_lua;
		}
	}
}
