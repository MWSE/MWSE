#include "TES3ScriptCompiler.h"

namespace TES3 {
	bool ScriptCompiler::compile(char* scriptText) {
		unknown_0x50 = 0;
		compiledScriptLength = 0;
		scriptBuffer[0] = 0;
		memset(scriptOrCompilerId, 0, 0x34);
		commandIterator = scriptText;
		command = scriptText;

		auto function = parseFunctionName();
		if (function == CompilerFunction::INVALID) {
			return false;
		}
		else if (function == CompilerFunction::BEGIN) {
			shortVarList = new IteratedList<Variable*>();
			longVarList = new IteratedList<Variable*>();
			floatVarList = new IteratedList<Variable*>();

			while (compileFunction(function)) {
				if (compiledScriptLength >= 0x4000) {
					return false;
				}

				if (function == CompilerFunction::END) {
					scriptBuffer[0] = 0;

					for (auto& var : *shortVarList) {
						linkVariable(var);
					}
					for (auto& var : *longVarList) {
						linkVariable(var);
					}
					for (auto& var : *floatVarList) {
						linkVariable(var);
					}

					delete shortVarList;
					shortVarList = nullptr;
					delete longVarList;
					longVarList = nullptr;
					delete floatVarList;
					floatVarList = nullptr;

					return true;
				}
			}

			function = parseFunctionName();
			if (function == CompilerFunction::INVALID) {
				// TODO: Error. Need to end with `end`
				return false;
			}

			delete shortVarList;
			shortVarList = nullptr;
			delete longVarList;
			longVarList = nullptr;
			delete floatVarList;
			floatVarList = nullptr;

			return false;
		}
		else {
			// TODO: Error. Need to start with `begin`
			return false;
		}
	}

	const auto TES3_ScriptCompiler_compileFunction = reinterpret_cast<bool(__thiscall*)(ScriptCompiler*, int)>(0x4F73E0);
	bool ScriptCompiler::compileFunction(int function) {
		return TES3_ScriptCompiler_compileFunction(this, function);
	}

	const auto TES3_ScriptCompiler_parseFunctionName = reinterpret_cast<int(__thiscall*)(ScriptCompiler*)>(0x4FCD10);
	int ScriptCompiler::parseFunctionName() {
		return TES3_ScriptCompiler_parseFunctionName(this);
	}

	void ScriptCompiler::linkVariable(Variable* var) {
		if (scriptLineBuffer && localVariableCount < 4096) {
			while (localVariableCount < 4096) {
				if (!isalnum(*var)) {
					if (*var != '-' && *var != '_') {
						scriptLineBuffer[localVariableCount++] = 0;
						return;
					}
				}

				scriptLineBuffer[localVariableCount] = *var;
				var = var + 1;
				localVariableCount++;
			}
		}
	}
}
