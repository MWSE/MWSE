#include "StringUtilLua.h"

#include "sol.hpp"

#include "LuaManager.h"
#include "StringUtil.h"

namespace mwse {
	namespace lua {
		void bindStringUtil() {
			sol::state& state = LuaManager::getInstance().getState();

			//
			// Extend mwse library with extra functions to replace %L in MWSE.
			//

			state["mwse"]["stringToLong"] = [](std::string value) {
				if (value.length() != 4) {
					return 0;
				}

				return *reinterpret_cast<const int*>(value.c_str());
			};

			state["mwse"]["longToString"] = [](int value) {
				return std::string(reinterpret_cast<char*>(&value), 4);
			};

			//
			// Also provide a way to interact with the string storage.
			//

			state["mwse"]["string"] = LuaManager::getInstance().createTable();

			state["mwse"]["string"]["create"] = [](std::string value) -> int {
				return mwse::string::store::getOrCreate(value.c_str());
			};

			state["mwse"]["string"]["get"] = [](double value) -> sol::object {
				try {
					sol::state& state = LuaManager::getInstance().getState();
					return sol::make_object(state, (std::string)mwse::string::store::get(value));
				}
				catch (std::exception& e) {
					return sol::nil;
				}
			};
		}
	}
}
