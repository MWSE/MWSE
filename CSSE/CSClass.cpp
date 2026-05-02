#include "CSClass.h"

#include "StringUtil.h"

namespace se::cs {
	bool Class::search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex) const {
		if (BaseObject::search(needle, settings, regex)) {
			return true;
		}

		if (string::complex_contains(name, needle, settings, regex)) {
			return true;
		}

		if (description && string::complex_contains(description, needle, settings, regex)) {
			return true;
		}

		return false;
	}
}
