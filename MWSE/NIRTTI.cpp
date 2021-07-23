#include "NIRTTI.h"

namespace NI {
	RTTI::RTTI(const char* name, RTTI* parent) {
		ctor(name, parent);
	}

	RTTI::RTTI(const char* name, RTTIStaticPtr::RTTIStaticPtr parent) {
		ctor(name, (RTTI*)parent);
	}

	const auto NI_RTTI_ctor = reinterpret_cast<RTTI * (__thiscall*)(RTTI*, const char*, RTTI*)>(0x6F9850);
	RTTI* RTTI::ctor(const char* name, RTTI* parent) {
		return NI_RTTI_ctor(this, name, parent);
	}
}
