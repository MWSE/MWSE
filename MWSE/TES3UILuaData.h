#pragma once

namespace TES3::UI {
	class LuaData {
	public:
		LuaData(sol::this_state& ts);
		~LuaData();

		sol::object getData() const;
		sol::object getValue(std::string_view key);
		void setValue(std::string_view key, sol::object value);

	private:
		sol::table data;
	};
}
