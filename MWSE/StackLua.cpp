#include "StackLua.h"

#include "sol.hpp"
#include "LuaUtil.h"
#include "LuaManager.h"

#include "VirtualMachine.h"
#include "Stack.h"

namespace mwse {
	namespace lua {
		void bindMWSEStack() {
			sol::state& state = LuaManager::getInstance().getState();

			state["mwse"]["stack"] = LuaManager::getInstance().createTable();

			//
			// Functions for pushing values.
			//

			state["mwse"]["stack"]["pushShort"] = [](double value) {
				Stack::getInstance().pushShort(value);
			};

			state["mwse"]["stack"]["pushLong"] = [](double value) {
				Stack::getInstance().pushLong(value);
			};

			state["mwse"]["stack"]["pushFloat"] = [](double value) {
				Stack::getInstance().pushFloat(value);
			};

			state["mwse"]["stack"]["pushString"] = [](std::string& value) {
				Stack::getInstance().pushString(value);
			};

			state["mwse"]["stack"]["pushObject"] = [](sol::object value) {
				Stack::getInstance().pushLong((long)value.as<TES3::BaseObject*>());
			};

			//
			// Functions for popping values.
			//

			state["mwse"]["stack"]["popShort"] = []() {
				Stack& stack = Stack::getInstance();
				sol::optional<short> maybe_ret;
				if (!stack.empty()) {
					maybe_ret = stack.popShort();
				}
				return maybe_ret;
			};

			state["mwse"]["stack"]["popLong"] = []() {
				Stack& stack = Stack::getInstance();
				sol::optional<long> maybe_ret;
				if (!stack.empty()) {
					maybe_ret = stack.popLong();
				}
				return maybe_ret;
			};

			state["mwse"]["stack"]["popFloat"] = []() {
				Stack& stack = Stack::getInstance();
				sol::optional<float> maybe_ret;
				if (!stack.empty()) {
					maybe_ret = stack.popFloat();
				}
				return maybe_ret;
			};

			state["mwse"]["stack"]["popString"] = []() {
				Stack& stack = Stack::getInstance();
				sol::optional<std::string> maybe_ret;
				if (!stack.empty()) {
					maybe_ret = mwAdapter::GetVMInstance()->getString(Stack::getInstance().popLong());
				}
				return maybe_ret;
			};

			state["mwse"]["stack"]["popObject"] = []() {
				Stack& stack = Stack::getInstance();
				sol::optional<sol::object> maybe_ret;
				if (!stack.empty()) {
					maybe_ret = lua::makeLuaObject(reinterpret_cast<TES3::BaseObject*>(Stack::getInstance().popLong()));
				}
				return maybe_ret;
			};

			//
			// Other stack functions.
			//

			state["mwse"]["stack"]["empty"] = []() {
				return Stack::getInstance().empty();
			};

			state["mwse"]["stack"]["size"] = []() {
				return Stack::getInstance().size();
			};

			state["mwse"]["stack"]["clear"] = []() {
				Stack::getInstance().clear();
			};

			state["mwse"]["stack"]["dump"] = []() {
				Stack::getInstance().dump();
			};

		}
	}
}
