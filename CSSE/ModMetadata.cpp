#include "ModMetadata.h"

#include "PathUtil.h"

namespace se::cs::metadata {
	ModMetadata::ModMetadata(const std::string_view& file, GameFile* gameFile) {
		m_FilePath = path::getDataFilesPath() / file;
		m_Toml = {};
		m_GameFile = gameFile;
		reload();
	}

	void ModMetadata::reload() {
		if (!std::filesystem::exists(m_FilePath)) {
			throw std::exception("The metadata file does not exist.");
		}

		m_Toml = toml::parse(m_FilePath);
	}

	GameFile* ModMetadata::gameFile() const {
		return m_GameFile;
	}

	void ModMetadata::gameFile(GameFile* gameFile) {
		gameFile = m_GameFile;
	}

	toml::value& ModMetadata::toml() {
		return m_Toml;
	}
}
