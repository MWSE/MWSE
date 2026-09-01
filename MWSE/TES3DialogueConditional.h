#pragma once

#include "TES3Defines.h"
#include "TES3DialogueFlags.h"

namespace TES3 {
	struct DialogueConditional {
		union {
			BaseObject* object;
			GlobalVariable* globalVariable;
			Dialogue* journal;
			Item* item;
			Actor* actor;
			Faction* faction;
			Class* class_;
			Cell* cell;
			Race* race;
			const char* localVarName;
			DialogueConditionalFunction function;
		};
		DialogueConditionalType type;
		DialogueConditionalConstantType constantType;
		short localVarIndex;
		DialogueConditionalComparator compareOperator;
		float compareValue;

		DialogueConditional();
		~DialogueConditional();
	};
	static_assert(sizeof(DialogueConditional) == 0x10, "TES3::DialogueConditional failed size validation");
}
