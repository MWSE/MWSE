#include "TES3UILuaData.h"

namespace TES3::UI {
	LuaData::LuaData(sol::this_state& ts) {
		sol::state_view state = ts;
		data = state.create_table();
	}

	LuaData::~LuaData() {
		data = sol::nil;
	}

	sol::object LuaData::getData() const {
		return data;
	}

	sol::object LuaData::getValue(const std::string_view& key) {
		return data[key];
	}

	void LuaData::setValue(const std::string_view& key, sol::object value) {
		data[key] = value;
	}
}
