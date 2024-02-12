#include "PathUtil.h"

#include "StringUtil.h"

namespace se::cs::path {
	std::filesystem::path getDocumentsPath() {
		CHAR path[MAX_PATH] = {};
		if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path) != S_OK) {
			return {};
		}
		return std::filesystem::canonical(path);
	}

	std::filesystem::path getInstallPath() {
		CHAR path[MAX_PATH] = {};
		if (GetModuleFileNameA(NULL, path, MAX_PATH) <= 0) {
			return {};
		}
		return std::filesystem::canonical(path).parent_path();
	}

	std::filesystem::path getDataFilesPath() {
		return getInstallPath() / "Data Files";
	}

	std::filesystem::path getSavePath() {
		return getInstallPath() / "Saves";
	}

	std::optional<std::string> getLowerPath(const std::filesystem::directory_entry& path) {
		try {
			auto lowerPath = path.path().string();
			string::to_lower(lowerPath);

			return lowerPath;
		}
		catch (std::exception&) {
			return {};
		}
	}

	namespace openmw {
		std::filesystem::path getConfigPath() {
			return getDocumentsPath() / "My Games" / "OpenMW";
		}

		std::filesystem::path getSavePath() {
			return getConfigPath() / "saves";
		}

		std::filesystem::path getTemporaryConfigPath() {
			return getDataFilesPath() / "MWSE" / "tmp" / "csse" / "openmw";
		}
	}
}
