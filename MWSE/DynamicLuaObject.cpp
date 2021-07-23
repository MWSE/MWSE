#include "DynamicLuaObject.h"

namespace mwse::lua {
	void DynamicLuaObject::dynamic_set(std::string key, sol::stack_object value) {
		auto it = entries.find(key);
		if (it == entries.cend()) {
			entries.insert(it, { std::move(key), std::move(value) });
		}
		else {
			std::pair<const std::string, sol::object>& kvp = *it;
			sol::object& entry = kvp.second;
			entry = sol::object(std::move(value));
		}
	}

	sol::object DynamicLuaObject::dynamic_get(std::string key) const {
		auto it = entries.find(key);
		if (it == entries.cend()) {
			return sol::lua_nil;
		}
		return it->second;
	}

	size_t DynamicLuaObject::size() const {
		return entries.size();
	}
}
