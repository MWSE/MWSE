#include "StdString.h"

#include "Config.h"
#include "ExceptionUtil.h"
#include "MemoryUtil.h"

namespace se {
	StdString::StdString() {
#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1
		const auto TES3_StdString_ctor = reinterpret_cast<void(__thiscall**)(StdString*)>(0x74617C);
		(*TES3_StdString_ctor)(this);
#else
		throw not_implemented_exception();
#endif
	}

	StdString::StdString(const char* c_str) : StdString() {
#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1
		const auto TES3_StdString_assign = reinterpret_cast<void(__thiscall**)(StdString*, const char*, size_t)>(0x7461CC);
		(*TES3_StdString_assign)(this, c_str, strlen(c_str));
#else
		throw not_implemented_exception();
#endif
	}

	StdString::~StdString() {
#if defined(SE_STDSTRING_DTOR) && SE_STDSTRING_DTOR > 0
		const auto TES3_StdString_dtor = reinterpret_cast<void(__thiscall**)(StdString*)>(SE_STDSTRING_DTOR);
		(*TES3_StdString_dtor)(this);
#else
		throw not_implemented_exception();
#endif
	}


	void* StdString::operator new(size_t size) {
#if defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
		return se::memory::_new(size);
#else
		throw not_implemented_exception();
#endif
	}

	void StdString::operator delete(void* block) {
#if defined(SE_MEMORY_FNADDR_DELETE) && SE_MEMORY_FNADDR_DELETE > 0
		se::memory::_delete(block);
#else
		throw not_implemented_exception();
#endif
	}

	void StdString::operator=(const char* c_str) {
#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1
		const auto TES3_StdString_assign = reinterpret_cast<void(__thiscall**)(StdString*, const char*, size_t)>(0x7461CC);
		(*TES3_StdString_assign)(this, c_str, strlen(c_str));
#else
		throw not_implemented_exception();
#endif
	}
}
