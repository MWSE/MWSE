module;

#include "ExceptionUtil.h"

module NIRTTI;

namespace NI {
	RTTI::RTTI(const char* name, RTTI* parent) {
#if defined(SE_NI_RTTI_ctor) && SE_NI_RTTI_ctor > 0
		const auto NI_RTTI_ctor = reinterpret_cast<RTTI * (__thiscall*)(RTTI*, const char*, RTTI*)>(SE_NI_RTTI_ctor);
		NI_RTTI_ctor(this, name, parent);
#else
		throw not_implemented_exception();
#endif
	}

	RTTI::RTTI(const char* name, RTTIStaticPtr::RTTIStaticPtr parent) :
		RTTI(name, (RTTI*)parent)
	{

	}

	const char* RTTI::toString() const {
		return name;
	}
}

