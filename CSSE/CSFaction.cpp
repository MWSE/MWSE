#include "CSFaction.h"

#include "StringUtil.h"

namespace se::cs {
	bool Faction::search(std::string_view needle, const SearchSettings& settings, std::regex* regex) const {
		if (BaseObject::search(needle, settings, regex)) {
			return true;
		}

		if (string::complex_contains(name, needle, settings, regex)) {
			return true;
		}

		return std::ranges::any_of(rankNames, [&](const auto& rank) {
			return string::complex_contains(rank, needle, settings, regex);
		});
	}
}
