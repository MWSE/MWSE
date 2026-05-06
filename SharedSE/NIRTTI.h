#pragma once

#include "NIDefines.h"

namespace NI {
	struct RTTI {
		char* name; // 0x0
		RTTI* baseRTTI; // 0x4

		RTTI(const char* name, RTTI* parent = nullptr);
		RTTI(const char* name, RTTIStaticPtr::RTTIStaticPtr parent);

		// MWSE-pattern in-place initializer: ctor() dispatches to the engine
		// constructor at SE_NI_RTTI_ctor. Implementation lives in MWSE-private
		// NIRTTI.cpp; the declaration is published here so MWSE-private callers
		// (and MWSE/NIRTTI.cpp's own RTTI::RTTI delegation) link cleanly after
		// any redirect from MWSE/NIRTTI.h to this file.
		RTTI* ctor(const char* name, RTTI* parent = nullptr);

		const char* toString() const;
	};
	static_assert(sizeof(RTTI) == 0x8, "NI::RTTI failed size validation");
}
