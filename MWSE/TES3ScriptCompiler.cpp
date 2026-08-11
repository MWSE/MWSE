#include "TES3ScriptCompiler.h"

#include "Log.h"

namespace TES3 {
	ScriptCompiler::ScriptCompiler() {
		const auto TES3_ScriptCompiler_ctor = reinterpret_cast<void(__thiscall*)(ScriptCompiler*)>(0x4F7260);
		TES3_ScriptCompiler_ctor(this);
	}

	ScriptCompiler::~ScriptCompiler() {
		const auto TES3_ScriptCompiler_dtor = reinterpret_cast<void(__thiscall*)(ScriptCompiler*)>(0x4F72C0);
		TES3_ScriptCompiler_dtor(this);
	}

	bool ScriptCompiler::compile(const char* scriptText) {
		currentLine = 0;
		compiledScriptLength = 0;
		scriptBuffer[0] = 0;
		memset(&scriptHeader, 0, sizeof(ScriptHeader));
		commandIterator = scriptText;
		command = scriptText;

		auto function = parseFunctionName();
		if (function == CompilerFunction::INVALID) {
			return false;
		}
		else if (function == CompilerFunction::BEGIN) {

			shortVarList = new NI::IteratedList<Variable*>();
			longVarList = new NI::IteratedList<Variable*>();
			floatVarList = new NI::IteratedList<Variable*>();

			while (compileFunction(function)) {
				if (compiledScriptLength >= 0x4000) {
					delete shortVarList;
					shortVarList = nullptr;
					delete longVarList;
					longVarList = nullptr;
					delete floatVarList;
					floatVarList = nullptr;
					return false;
				}

				if (function == CompilerFunction::END) {
					scriptLineBuffer[0] = 0;

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

				function = parseFunctionName();
				if (function == CompilerFunction::INVALID) {
					mwse::log::getLog() << "tes3script:recompile: Could not recompile provided script " << scriptHeader.name 
						<< ". Script needs to end with `end` keyword." << std::endl;
					delete shortVarList;
					shortVarList = nullptr;
					delete longVarList;
					longVarList = nullptr;
					delete floatVarList;
					floatVarList = nullptr;
					return false;
				}
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
			mwse::log::getLog() << "tes3script:recompile: Could not recompile provided script " << scriptHeader.name
				<< ". Script needs to start with `begin` keyword." << std::endl;
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
		if (scriptLineBuffer && scriptHeader.localVarNameSize < 4096) {
			while (scriptHeader.localVarNameSize < 4096) {
				if (!isalnum(*var)) {
					if (*var != '-' && *var != '_') {
						scriptLineBuffer[scriptHeader.localVarNameSize++] = 0;
						return;
					}
				}

				scriptLineBuffer[scriptHeader.localVarNameSize] = *var;
				var = var + 1;
				scriptHeader.localVarNameSize++;
			}
		}
	}
}
