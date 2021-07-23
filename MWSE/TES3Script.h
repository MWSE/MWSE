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
		};
		short * shortVarValues; // 0x0
		long * longVarValues; // 0x4
		float * floatVarValues; // 0x8
		int unknown_0xC;
		IteratedList<WeaponHitFlags*> * hitWeapons; // 0x10
	};
	static_assert(sizeof(ScriptVariables) == 0x14, "TES3::ScriptVariables failed size validation");

	struct GlobalScript {
		Reference * reference; // 0x0
		Script * script; // 0x4

		//
		// Custom functions.
		//

		std::shared_ptr<mwse::lua::ScriptContext> createContext() const;

	};
	static_assert(sizeof(GlobalScript) == 0x8, "TES3::GlobalScript failed size validation");

	struct StartScript : BaseObject {
		char id[32]; // 0x10
		Script * script; // 0x30
	};
	static_assert(sizeof(StartScript) == 0x34, "TES3::StartScript failed size validation");

	struct Script : BaseObject {
		char name[32]; // 0x10
		long shortCount; // 0x30
		long longCount; // 0x34
		long floatCount; // 0x38
		long dataLength; // 0x3C
		int localVarLength; // The length of the local var names concatenated (includes \0 for string endings)
		char ** shortVarNamePointers; // 0x44
		char ** longVarNamePointers; // 0x48
		char ** floatVarNamePointers; // 0x4C
		char * varNamePointers; // 0x50
		int unknown_0x54; // equals zero?
		void * machineCode; // 0x58
		ScriptVariables varValues; // 0x5C

		//
		// Other related this-call functions.
		//

		void getScriptParams(bool unknown = true);
		float executeScriptOpCode(unsigned int opCode, char charParam, BaseObject * objectParam);

		char getLocalVarIndexAndType(const char*, unsigned int*);

		short getShortValue(unsigned int, bool);
		int getLongValue(unsigned int, bool);
		float getFloatValue(unsigned int, bool);
		
		void doCommand(ScriptCompiler * compiler, const char* command, int source = TES3::CompilerSource::Default, Reference * reference = nullptr, ScriptVariables * variables = nullptr, DialogueInfo * info = nullptr, Dialogue * dialogue = nullptr);

		void execute(Reference* reference, ScriptVariables* data, DialogueInfo* info, Reference* reference2);

		//
		// Custom functions.
		//

		sol::table getLocalVars_lua(sol::this_state ts, sol::optional<bool> useLocals = false);
		std::shared_ptr<mwse::lua::ScriptContext> createContext();

		//
		// Debug values.
		//

		static Script* currentlyExecutingScript;
		static Reference* currentlyExecutingScriptReference;

	};
	static_assert(sizeof(Script) == 0x70, "TES3::Script failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Script)
