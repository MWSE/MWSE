#pragma once

namespace mwse {
	namespace lua {
		// Table of typename-keyed functions to convert from specific pointers to DWORDs.
		extern sol::table convertFrom;

		// Table of typename-keyed functions to convert from DWORDs to the appropriate sol::object type.
		extern sol::table convertTo;

		void bindMWSEMemoryUtil();
	}
}
