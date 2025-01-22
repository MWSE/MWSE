#include "MetadataUtil.h"

#include "CSBaseObject.h"
#include "CSDataHandler.h"
#include "CSGameFile.h"
#include "CSRecordHandler.h"

#include "LogUtil.h"
#include "PathUtil.h"
#include "StringUtil.h"

namespace se::cs::metadata {
	static std::vector<std::shared_ptr<ModMetadata>> activeMetadata;
	static std::unordered_map<GameFile*, std::shared_ptr<ModMetadata>> gameFileMap;

	std::unordered_set<std::string> deprecatedIds;

	void reloadModMetadata() {
		// Reload any currently loaded metadata.
		for (auto& [gameFile, metadata] : gameFileMap) {
			metadata->reload();
		}

		// Scan for new metadata to load.
		const auto recordHandler = DataHandler::get()->recordHandler;
		for (const auto& gameFile : *recordHandler->availableDataFiles) {
			// Skip any that are already loaded.
			if (gameFileMap.find(gameFile) != gameFileMap.end()) {
				continue;
			}

			// Convert the mod name to a toml file name.
			const std::string gameFileName = gameFile->fileName;
			const std::string metadataPath = std::string(gameFileName.begin(), gameFileName.end() - 4) + "-metadata.toml";
			if (!std::filesystem::exists(path::getDataFilesPath() / metadataPath)) {
				continue;
			}

			try {
				auto metadata = std::make_shared<ModMetadata>(metadataPath, gameFile);
				gameFileMap[gameFile] = metadata;
			}
			catch (toml::syntax_error& e) {
				log::stream << "WARNING: Could not parse toml from metadata file '" << metadataPath << "': " << e.what() << std::endl;
			}
			catch (std::exception& e) {
				log::stream << "WARNING: Unexpected exception when trying to load metadata file '" << metadataPath << "': " << e.what() << std::endl;
			}
		}

		// Rescan for what metadata is active.
		activeMetadata.clear();
		for (const auto& gameFile : recordHandler->activeGameFiles) {
			const auto itt = gameFileMap.find(gameFile);
			if (itt == gameFileMap.end()) {
				continue;
			}

			activeMetadata.push_back(itt->second);
		}

		if (!activeMetadata.empty()) {
			log::stream << "Loaded metadata for content files: ";
			for (size_t i = 0; i < activeMetadata.size(); ++i) {
				if (i != 0) {
					log::stream << ", ";
				}
				log::stream << activeMetadata[i]->gameFile()->fileName;
			}
			log::stream << std::endl;
		}

		// Go through and figure out what objects are deprecated.
		for (const auto& metadata : activeMetadata) {
			try {
				const auto& data = metadata->toml();
				auto deprecated = toml::find<std::vector<std::string>>(data, "tools", "csse", "deprecated");
				for (auto& id : deprecated) {
					string::to_lower(id);
					deprecatedIds.insert(id);
				}
			}
			catch (...) {
				continue;
			}
		}

		if (!deprecatedIds.empty()) {
			log::stream << "Blacklisting " << deprecatedIds.size() << " IDs that have been deprecated." << std::endl;
		}
	}

	std::shared_ptr<ModMetadata> getMetadataForGameFile(GameFile* gameFile) {
		const auto itt = gameFileMap.find(gameFile);
		if (itt == gameFileMap.end()) {
			return {};
		}

		return itt->second;
	}

	const std::vector<std::shared_ptr<ModMetadata>>& getActiveModMetadata() {
		return activeMetadata;
	}

	bool isDeprecated(const BaseObject* object) {
		std::string id = object->getObjectID();
		if (id.empty()) {
			return false;
		}
		string::to_lower(id);
		return deprecatedIds.find(id) != deprecatedIds.end();
	}
}
