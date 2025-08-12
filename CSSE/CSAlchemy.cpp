#include "CSAlchemy.h"

namespace se::cs {
	bool Alchemy::search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex) const {
		if (Object::search(needle, settings, regex)) {
			return true;
		}

		if (settings.effect) {
			for (const auto& effect : effects) {
				if (effect.search(needle, settings, regex)) {
					return true;
				}
			}
		}

		return false;
	}
}
