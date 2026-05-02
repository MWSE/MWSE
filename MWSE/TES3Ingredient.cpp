#include "TES3Ingredient.h"

namespace TES3 {
	void Ingredient::setName(const char* n) {
		if (strlen(n) >= 32) {
			throw std::length_error("Input string is more than 31 characters long.");
		}

		strcpy(name, n);
	}

	void Ingredient::setIconPath(const char* path) {
		if (strlen(path) >= 32) {
			throw std::invalid_argument("Path cannot be 32 or more characters.");
		}
		strncpy_s(texture, path, 32);
	}

	std::reference_wrapper<int[4]> Ingredient::getEffects() {
		return std::ref(effects);
	}

	std::reference_wrapper<int[4]> Ingredient::getEffectSkillIds() {
		return std::ref(effectSkillIds);
	}

	std::reference_wrapper<int[4]> Ingredient::getEffectAttributeIds() {
		return std::ref(effectAttributeIds);
	}

	int Ingredient::getFirstIndexOfEffect(int effectId) const {
		for (size_t i = 0; i < 4; ++i) {
			if (effects[i] == effectId) {
				return i;
			}
		}
		return -1;
	}

	bool Ingredient::hasEffect(int effectId) const {
		return getFirstIndexOfEffect(effectId) != -1;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Ingredient)
