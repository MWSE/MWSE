#include "CSGlobalVariable.h"

namespace se::cs {
	GlobalVariable::GlobalVariable() {
		const auto GlobalVariable_ctor = reinterpret_cast<GlobalVariable * (__thiscall*)(GlobalVariable*)>(0x403F62);
		GlobalVariable_ctor(this);
	}

	GlobalVariable::GlobalVariable(const char* id) : GlobalVariable() {
		setObjectID(id);
	}

	GlobalVariable::~GlobalVariable() {
		const auto GlobalVariable_dtor = reinterpret_cast<void(__thiscall*)(GlobalVariable*)>(0x402F1D);
		GlobalVariable_dtor(this);
	}

	void GlobalVariable::setObjectID(const char* id) {
		strncpy_s(name, id, 31);
	}
}
