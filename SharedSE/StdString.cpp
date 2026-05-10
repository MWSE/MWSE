#include "StdString.h"

#include "ExceptionUtil.h"

namespace se {
	StdString::StdString() {
#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == TRUE
		const auto TES3_StdString_ctor = reinterpret_cast<void(__thiscall**)(StdString*)>(0x74617C);
		(*TES3_StdString_ctor)(this);
#else
		throw not_implemented_exception();
#endif
	}

	StdString::StdString(const char* c_str) : StdString() {
#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == TRUE
		const auto TES3_StdString_assign = reinterpret_cast<void(__thiscall**)(StdString*, const char*, size_t)>(0x7461CC);
		(*TES3_StdString_assign)(this, c_str, strlen(c_str));
#else
		throw not_implemented_exception();
#endif
	}

	StdString::~StdString() {
#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == TRUE
		const auto TES3_StdString_dtor = reinterpret_cast<void(__thiscall**)(StdString*)>(0x7461C4);
		(*TES3_StdString_dtor)(this);
#else
		throw not_implemented_exception();
#endif
	}

	void StdString::operator=(const char* c_str) {
#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == TRUE
		const auto TES3_StdString_assign = reinterpret_cast<void(__thiscall**)(StdString*, const char*, size_t)>(0x7461CC);
		(*TES3_StdString_assign)(this, c_str, strlen(c_str));
#else
		throw not_implemented_exception();
#endif
	}
}
