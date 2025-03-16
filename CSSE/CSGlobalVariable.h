#pragma once

#include "CSBaseObject.h"

namespace se::cs {
	struct GlobalVariable : BaseObject {
		char name[32]; // 0x10
		char valueType; // 0x30
		float value; // 0x34

		GlobalVariable();
		GlobalVariable(const char* id);
		~GlobalVariable();

		void setObjectID(const char* id);
	};
	static_assert(sizeof(GlobalVariable) == 0x38, "GlobalVariable failed size validation");
}