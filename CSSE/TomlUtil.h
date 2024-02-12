#pragma once

namespace se::cs::toml_util {
	/// <summary>
	/// Determines if two toml values are equal, ignoring comments.
	/// </summary>
	bool valuesEqual(const toml::value& lhs, const toml::value& rhs);

	/// <summary>
	/// Attempts to match comments from one toml value and copy them into another toml value.
	/// </summary>
	void copyComments(toml::value& to, const toml::value& from);
}
