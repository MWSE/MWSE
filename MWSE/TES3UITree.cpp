#include "TES3UITree.h"

#include "TES3UIManager.h"

namespace TES3::UI {
	const char* TreeItem::getName() {
		return TES3::UI::lookupID(key);
	}

	PropertyType TreeItem::getType() const {
		return (PropertyType)(unsigned int(valueType) & 0x7F);
	}

	sol::object TreeItem::getValue_lua(sol::this_state ts) const {
		const auto type = getType();

		switch (type) {
		case TES3::UI::PropertyType::Integer:
			return sol::make_object(ts, value.integerValue);
		case TES3::UI::PropertyType::Float:
			return sol::make_object(ts, value.floatValue);
		case TES3::UI::PropertyType::Property:
			if (value.propertyValue == TES3::UI::Property::boolean_true) {
				return sol::make_object(ts, true);
			}
			else if (value.propertyValue == TES3::UI::Property::boolean_false) {
				return sol::make_object(ts, false);
			}
			else {
				return sol::make_object(ts, value.propertyValue);
			}
		}
		return sol::nil;
	}
}
