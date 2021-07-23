#pragma once

#include "TES3Defines.h"

#include "TES3IteratedList.h"

namespace TES3 {
	namespace CompilerSource {
		enum CompilerSource : uintptr_t {
			Default,
			Console,
			Dialogue,

			FirstSource = Default,
			LastSource = Dialogue,
		};
	}

	namespace CompilerFunction {
		enum CompilerFunction : int {
			BEGIN = 0x100,
			END = 0x101,
			INVALID = 0xFFFF,
		};
	}

	struct ScriptCompiler {
		typedef char Variable;

		char ** shortVarNames; // 0x0
		char ** longVarNames; // 0x4
		char ** floatVarNames; // 0x8
		char scriptOrCompilerId[32]; // 0xC
		int shortVarCount;  // 0x2C
		int longVarCount; // 0x30
		int floatVarCount; // 0x34
		int scriptDataSize; // 0x38
		unsigned int localVariableCount; // 0x3C
		IteratedList<Variable*>* shortVarList; // 0x40
		IteratedList<Variable*>* longVarList; // 0x44
		IteratedList<Variable*>* floatVarList; // 0x48
		unsigned char * scriptLineBuffer; // 0x4C
		int unknown_0x50;
		int compiledScriptLength; // 0x54
		unsigned char * scriptBuffer; // 0x58
		char * commandIterator; // 0x5C
		char * command; // 0x60
		int unknown_0x64;
		int unknown_0x68;
		unsigned char unknown_0x6C[512];
		int unknown_0x26C;
		int unknown_0x270;
		int unknown_0x274;
		int unknown_0x278;
		int unknown_0x27C;
		int unknown_0x280;
		int unknown_0x284;
		int unknown_0x288;
		int unknown_0x28C;
		int unknown_0x290;
		int unknown_0x294;
		int unknown_0x298;
		int unknown_0x29C;
		int unknown_0x2A0;
		int unknown_0x2A4;
		int unknown_0x2A8;
		int unknown_0x2AC;
		int unknown_0x2B0;
		int unknown_0x2B4;
		int unknown_0x2B8;
		int unknown_0x2BC;
		int unknown_0x2C0;
		int unknown_0x2C4;
		int unknown_0x2C8;
		int unknown_0x2CC;
		int unknown_0x2D0;
		int unknown_0x2D4;
		int unknown_0x2D8;
		int unknown_0x2DC;
		int unknown_0x2E0;
		int unknown_0x2E4;
		int unknown_0x2E8;
		int unknown_0x2EC;
		int unknown_0x2F0;
		int unknown_0x2F4;
		int unknown_0x2F8;
		int unknown_0x2FC;
		int unknown_0x300;
		int unknown_0x304;
		int unknown_0x308;
		int unknown_0x30C;
		int unknown_0x310;
		int unknown_0x314;
		int unknown_0x318;
		int unknown_0x31C;
		int unknown_0x320;
		int unknown_0x324;
		int unknown_0x328;
		int unknown_0x32C;
		int unknown_0x330;
		int unknown_0x334;
		int unknown_0x338;
		int unknown_0x33C;
		int unknown_0x340;
		int unknown_0x344;
		int unknown_0x348;
		int unknown_0x34C;
		int unknown_0x350;
		int unknown_0x354;
		int unknown_0x358;
		int unknown_0x35C;
		int unknown_0x360;
		int unknown_0x364;
		int unknown_0x368;
		int unknown_0x36C;
		int unknown_0x370;
		int unknown_0x374;
		int unknown_0x378;
		int unknown_0x37C;
		int unknown_0x380;
		int unknown_0x384;
		int unknown_0x388;
		int unknown_0x38C;
		int unknown_0x390;
		int unknown_0x394;
		int unknown_0x398;
		int unknown_0x39C;
		int unknown_0x3A0;
		int unknown_0x3A4;
		int unknown_0x3A8;
		int unknown_0x3AC;
		int unknown_0x3B0;
		int unknown_0x3B4;
		int unknown_0x3B8;
		int unknown_0x3BC;
		int unknown_0x3C0;
		int unknown_0x3C4;
		int unknown_0x3C8;
		int unknown_0x3CC;
		int unknown_0x3D0;
		int unknown_0x3D4;
		int unknown_0x3D8;
		int unknown_0x3DC;
		int unknown_0x3E0;
		int unknown_0x3E4;
		int unknown_0x3E8;
		int unknown_0x3EC;
		int unknown_0x3F0;
		int unknown_0x3F4;
		int unknown_0x3F8;
		int unknown_0x3FC;
		int unknown_0x400;
		int unknown_0x404;
		int unknown_0x408;
		int unknown_0x40C;
		int unknown_0x410;
		int unknown_0x414;
		int unknown_0x418;
		int unknown_0x41C;
		int unknown_0x420;
		int unknown_0x424;
		int unknown_0x428;
		int unknown_0x42C;
		int unknown_0x430;
		int unknown_0x434;
		int unknown_0x438;
		int unknown_0x43C;
		int unknown_0x440;
		int unknown_0x444;
		int unknown_0x448;
		int unknown_0x44C;
		int unknown_0x450;
		int unknown_0x454;
		int unknown_0x458;
		int unknown_0x45C;
		int unknown_0x460;
		int unknown_0x464;
		int unknown_0x468;
		Reference * reference; // 0x46C
		int unknown_0x470;
		int unknown_0x474;
		int unknown_0x478;

		bool compile(char* scriptText);
		bool compileFunction(int function);

		int parseFunctionName();

		void linkVariable(Variable*);
	};
	static_assert(sizeof(ScriptCompiler) == 0x47C, "TES3::ScriptCompiler failed size validation");
}
