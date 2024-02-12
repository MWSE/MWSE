#pragma once

namespace se::cs::path {
	// e.g. C:\Users\<user>\Documents
	std::filesystem::path getDocumentsPath();

	// e.g. C:\Games\Morrowind
	std::filesystem::path getInstallPath();

	// e.g. C:\Games\Morrowind\Data Files
	std::filesystem::path getDataFilesPath();

	// e.g. C:\Games\Morrowind\Saves
	std::filesystem::path getSavePath();

	std::optional<std::string> getLowerPath(const std::filesystem::directory_entry& path);

	namespace openmw {
		// e.g. C:\Users\<user>\Documents\My Games\OpenMW
		std::filesystem::path getConfigPath();

		// e.g. C:\Users\<user>\Documents\My Games\OpenMW\Saves
		std::filesystem::path getSavePath();

		// e.g. Morrowind\Data Files\MWSE\tmp\csse\openmw
		std::filesystem::path getTemporaryConfigPath();
	}
}
