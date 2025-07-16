#include "StringUtilLua.h"

#include "LuaManager.h"
#include "StringUtil.h"

namespace mwse::lua {
	void bindStringUtil() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

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

		state["mwse"]["string"] = state.create_table();

		state["mwse"]["string"]["create"] = [](std::string value) -> int {
			return mwse::string::store::getOrCreate(value.c_str());
		};

		state["mwse"]["string"]["get"] = [](double value) -> sol::optional<std::string> {
			try {
				return { mwse::string::store::get(value).c_str() };
			}
			catch (std::exception&) {
				return {};
			}
		};
	}
}
