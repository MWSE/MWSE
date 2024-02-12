#include "TomlUtil.h"

namespace se::cs::toml_util {
	bool valuesEqual(const toml::value& lhs, const toml::value& rhs) {
		if (lhs.type() != rhs.type()) { return false; }

		switch (lhs.type())
		{
		case toml::value_t::boolean:
		{
			return lhs.as_boolean() == rhs.as_boolean();
		}
		case toml::value_t::integer:
		{
			return lhs.as_integer() == rhs.as_integer();
		}
		case toml::value_t::floating:
		{
			return lhs.as_floating() == rhs.as_floating();
		}
		case toml::value_t::string:
		{
			return lhs.as_string() == rhs.as_string();
		}
		case toml::value_t::offset_datetime:
		{
			return lhs.as_offset_datetime() == rhs.as_offset_datetime();
		}
		case toml::value_t::local_datetime:
		{
			return lhs.as_local_datetime() == rhs.as_local_datetime();
		}
		case toml::value_t::local_date:
		{
			return lhs.as_local_date() == rhs.as_local_date();
		}
		case toml::value_t::local_time:
		{
			return lhs.as_local_time() == rhs.as_local_time();
		}
		case toml::value_t::array:
		{
			if (rhs.as_array().size() != lhs.as_array().size()) { return false; }
			for (size_t i = 0; i < rhs.as_array().size(); ++i) {
				if (!valuesEqual(lhs.as_array().at(i), rhs.as_array().at(i))) { return false; }
			}
			return true;
		}
		case toml::value_t::table:
		{
			for (const auto& [key, value] : rhs.as_table()) {
				if (!lhs.contains(key)) { return false; }
				if (!valuesEqual(lhs.at(key), value)) { return false; }
			}
			return true;
		}
		case toml::value_t::empty: {return true; }
		default: {return false; }
		}
	}

	void copyComments(toml::value& to, const toml::value& from) {
		// Copy basic comments for the property.
		for (const auto& comment : from.comments()) {
			to.comments().push_back(comment);
		}

		// Copy table-children comments. We preserve comments recursively for children.
		if (to.is_table() && from.is_table()) {
			for (const auto& [key, value] : from.as_table()) {
				if (to.contains(key)) {
					copyComments(to.at(key), value);
				}
			}
		}

		// Copy array-children comments. We try to preserve comments by value.
		if (to.is_array() && from.is_array()) {
			for (const auto& fromValue : from.as_array()) {
				for (auto& toValue : to.as_array()) {
					if (valuesEqual(toValue, fromValue)) {
						copyComments(toValue, fromValue);
						break;
					}
				}
			}
		}
	}
}
