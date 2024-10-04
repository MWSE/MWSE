#pragma once

#include "TES3ObjectLua.h"

namespace mwse::lua {
	template <typename T>
	void setUserDataForTES3Item(sol::usertype<T>& usertypeDefinition) {
		setUserdataForTES3PhysicalObject(usertypeDefinition);

		// Functions exposed as properties.
		usertypeDefinition["isUsableByBeasts"] = sol::readonly_property(&TES3::Item::isUsableByBeasts);
	}
}