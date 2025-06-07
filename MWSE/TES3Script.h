#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"
#include "TES3ScriptCompiler.h"

namespace mwse::lua {
	class ScriptContext;
}

namespace TES3 {
	struct ScriptVariables {
		struct WeaponHitFlags {
			Weapon * weapon; // 0x0
			unsigned char flags; // 0x4 // 0x10 is hit, 0x20 is hit attempt.

			WeaponHitFlags() = delete;
			~WeaponHitFlags() = delete;
		};
		short * shortVarValues; // 0x0
		long * longVarValues; // 0x4
		float * floatVarValues; // 0x8
		int unknown_0xC;
		IteratedList<WeaponHitFlags*> * hitWeapons; // 0x10

		ScriptVariables() = delete;
		~ScriptVariables() = delete;
	};
	static_assert(sizeof(ScriptVariables) == 0x14, "TES3::ScriptVariables failed size validation");

	struct GlobalScript {
		Reference * reference; // 0x0
		Script * script; // 0x4

		GlobalScript() = delete;
		~GlobalScript() = delete;

		//
		// Custom functions.
		//

		std::shared_ptr<mwse::lua::ScriptContext> createContext() const;

	};
	static_assert(sizeof(GlobalScript) == 0x8, "TES3::GlobalScript failed size validation");

	struct StartScript : BaseObject {
		char id[32]; // 0x10
		Script * script; // 0x30

		StartScript() = delete;
		~StartScript() = delete;
	};
	static_assert(sizeof(StartScript) == 0x34, "TES3::StartScript failed size validation");

	struct Script : BaseObject {
		ScriptHeader header; // 0x10
		char** shortVarNamePointers; // 0x44
		char** longVarNamePointers; // 0x48
		char** floatVarNamePointers; // 0x4C
		char* varNamePointers; // 0x50 // Buffer containing all variable names, separated by null terminators. Is of header.localVarNameSize length.
		int unknown_0x54; // equals zero?
		BYTE * machineCode; // 0x58
		ScriptVariables varValues; // 0x5C

		Script() = delete;
		~Script() = delete;

		//
		// Other related this-call functions.
		//

		float executeScriptOpCode(unsigned int opCode, char charParam, BaseObject * objectParam);

		char getLocalVarIndexAndType(const char* name, unsigned int* out_index);
		sol::optional<unsigned int> getShortVarIndex(const char* name) const;

		short getShortValue(unsigned int index, bool useLocalVars);
		int getLongValue(unsigned int index, bool useLocalVars);
		float getFloatValue(unsigned int index, bool useLocalVars);

		void doCommand(ScriptCompiler * compiler, const char* command, int source = TES3::CompilerSource::Default, Reference * reference = nullptr, ScriptVariables * variables = nullptr, DialogueInfo * info = nullptr, Dialogue * dialogue = nullptr);

		void execute(Reference* reference, ScriptVariables* data, DialogueInfo* info, Reference* reference2);
		void setToCompiledResult(ScriptHeader* header, void* byteCode, const char* varNames, bool resolveParams = true);

		//
		// Custom functions.
		//

		sol::optional<std::string> getScriptText() const;
		nonstd::span<BYTE> getByteCode() const;
		bool recompile(const char* text);

		sol::table getLocalVars_lua(sol::this_state ts, sol::optional<bool> useLocals = false);
		std::shared_ptr<mwse::lua::ScriptContext> createContext();

		unsigned int getShortVariableCount() const;
		unsigned int getLongVariableCount() const;
		unsigned int getFloatVariableCount() const;

		//
		// Debug values.
		//

		static Script* currentlyExecutingScript;
		static Reference* currentlyExecutingScriptReference;

	};
	static_assert(sizeof(Script) == 0x70, "TES3::Script failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Script)
