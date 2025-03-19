#include "TES3StdString.h"

namespace TES3 {
	const auto TES3_StdString_ctor = reinterpret_cast<void(__thiscall**)(StdString*)>(0x74617C);
	const auto TES3_StdString_dtor = reinterpret_cast<void(__thiscall**)(StdString*)>(0x7461C4);
	const auto TES3_StdString_assign = reinterpret_cast<void(__thiscall**)(StdString*, const char*, size_t)>(0x7461CC);
	const auto TES3_StdString_grow = reinterpret_cast<void(__thiscall**)(StdString*, size_t, bool)>(0x7461D4);

	StdString::StdString() {
		(*TES3_StdString_ctor)(this);
	}

	StdString::StdString(const char* c_str) : StdString() {
		(*TES3_StdString_assign)(this, c_str, strlen(c_str));
	}

	StdString::~StdString() {
		(*TES3_StdString_dtor)(this);
	}

	void StdString::operator=(const char* c_str) {
		(*TES3_StdString_assign)(this, c_str, strlen(c_str));
	}
}
