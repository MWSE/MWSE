#pragma once

#include "NIObjectLua.h"

namespace mwse::lua {
	template <typename T>
	void setUserdataForNIExtraData(sol::usertype<T>& usertypeDefinition) {
		setUserdataForNIObject(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["genericData"] = sol::readonly_property(&NI::ExtraData::getGenericData);
		usertypeDefinition["next"] = sol::property(&NI::ExtraData::getNext, &NI::ExtraData::setNext);
	}

	void bindNIExtraData();
}
