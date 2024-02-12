#pragma once

#include "CSDefines.h"

namespace se::cs::metadata {
	class ModMetadata {
	public:
		ModMetadata(const std::string_view& file, GameFile* gameFile);

		void reload();

		GameFile* gameFile() const;
		void gameFile(GameFile* gameFile);

		toml::value& toml();

	private:
		std::filesystem::path m_FilePath;
		toml::value m_Toml;
		GameFile* m_GameFile;
	};
}
