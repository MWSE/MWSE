#include "TES3RepairTool.h"

namespace TES3 {
	void RepairTool::setIconPath(const char* path) {
		if (strlen(path) >= 32) {
			throw std::invalid_argument("Path cannot be 32 or more characters.");
		}
		strncpy_s(icon, path, 32);
	}

	void RepairTool::setName(const char* newName) {
		if (strlen(newName) >= 32) {
			throw std::invalid_argument("Name cannot be 32 or more characters.");
		}
		strncpy_s(name, newName, sizeof(name));
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::RepairTool)
