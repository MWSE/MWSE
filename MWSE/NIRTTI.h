#pragma once

#include "NIDefines.h"

namespace NI {
	struct RTTI {
		char* name; // 0x0
		RTTI * baseRTTI; // 0x4

		//
		// 
		//

		RTTI(const char* name, RTTI* parent = nullptr);
		RTTI(const char* name, RTTIStaticPtr::RTTIStaticPtr parent);

		//
		//
		//

		RTTI* ctor(const char* name, RTTI* parent = nullptr);
	};
	static_assert(sizeof(RTTI) == 0x8, "NI::RTTI failed size validation");
}
