#pragma once

#include "TES3ObjectLua.h"

#include "TES3Enchantment.h"

namespace mwse::lua {
	template <typename T>
	void setUserdataForTES3Item(sol::usertype<T>& usertypeDefinition) {
		static_assert(std::is_base_of<TES3::Item, T>::value, "Type must inherit from TES3::Item.");
		setUserdataForTES3PhysicalObject(usertypeDefinition);

		// Functions exposed as properties.
		usertypeDefinition["icon"] = sol::property(&T::getIconPath, &T::setIconPath);
		usertypeDefinition["isCarriable"] = sol::readonly_property(&T::getIsCarriable);
		usertypeDefinition["mesh"] = sol::property(&T::getModelPath, &T::setModelPath);
		usertypeDefinition["name"] = sol::property(&T::getName, &T::setName);
		usertypeDefinition["promptsEquipmentReevaluation"] = sol::readonly_property(&T::promptsEquipmentReevaluation);
		usertypeDefinition["stolenList"] = sol::readonly_property(&T::getStolenList_lua);

		// TODO: Deprecated. Remove before 2.1-stable.
		usertypeDefinition["model"] = sol::property(&T::getModelPath, &T::setModelPath);
	}
}
