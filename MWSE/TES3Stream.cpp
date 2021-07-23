#include "TES3Stream.h"

namespace TES3 {
	const auto TES3_Stream_ctor = reinterpret_cast<void(__thiscall*)(Stream*)>(0x51AD20);
	Stream::Stream() {
		TES3_Stream_ctor(this);
	}

	const auto TES3_Stream_dtor = reinterpret_cast<void(__thiscall*)(Stream*)>(0x51AD60);
	Stream::~Stream() {
		TES3_Stream_dtor(this);
	}
}
