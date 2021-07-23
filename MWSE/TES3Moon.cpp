#include "TES3Moon.h"

namespace TES3 {
	const char* Moon::getTexturePath() const {
		return texturePath;
	}

	void Moon::setTexturePath(const char* path) {
		if (strlen(path) >= sizeof(texturePath)) {
			throw std::invalid_argument("Path cannot be 260 or more characters.");
		}
		strncpy_s(texturePath, path, sizeof(texturePath));
	}
}
