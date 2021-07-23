#pragma once

#include "TES3Defines.h"

namespace mwse {
	namespace lua {
		class ScriptContext {
		public:
			ScriptContext(TES3::Script* s, TES3::ScriptVariables* v = NULL) {
				script = s;
				variables = v;
			}

			sol::object index(std::string);
			void new_index(std::string, sol::stack_object);
			size_t length();

			sol::table getVariableData(sol::this_state ts);

		private:
			TES3::Script* script;
			TES3::ScriptVariables* variables;

			TES3::ScriptVariables* getScriptVariables();
		};

		void bindTES3Script();
	}
}