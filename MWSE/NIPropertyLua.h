#pragma once

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForNIProperty(sol::usertype<T>& usertypeDefinition) {
			setUserdataForNIObjectNET(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["propertyFlags"] = &NI::Property::flags;
			usertypeDefinition["type"] = sol::readonly_property(&NI::Property::getType);
		}

		void bindNIProperties();
	}
}
