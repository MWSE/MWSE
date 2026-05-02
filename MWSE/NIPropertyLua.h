#pragma once

#include "NIProperty.h"

#include "NIObjectLua.h"

namespace mwse::lua {
	template <typename T>
	void setUserdataForNIProperty(sol::usertype<T>& usertypeDefinition) {
		setUserdataForNIObjectNET(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["propertyFlags"] = &NI::Property::flags;
		usertypeDefinition["type"] = sol::readonly_property(&NI::Property::getType);

		// Basic function binding.
		usertypeDefinition["setFlag"] = &NI::Property::setFlag;
	}

	void bindNIProperties();
}
