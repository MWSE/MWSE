#include "TES3StdString.h"

namespace TES3 {
	const auto TES3_StdString_assign = reinterpret_cast<void(__thiscall**)(StdString *, const char*, size_t)>(0x7461CC);

	void StdString::operator=(const char* c_str) {
		(*TES3_StdString_assign)(this, c_str, strlen(c_str));
	}
}
