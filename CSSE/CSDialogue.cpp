#include "CSDialogue.h"

#include "StringUtil.h"

namespace se::cs {
	bool Dialogue::search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex) const {
		return id && string::complex_contains(id, needle, settings, regex);
	}
}