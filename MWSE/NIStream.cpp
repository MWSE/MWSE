#include "NIStream.h"

namespace NI {
	const auto NI_Stream_ctor = reinterpret_cast<void(__thiscall*)(const Stream*)>(0x51AD20);
	const auto NI_Stream_dtor = reinterpret_cast<void(__thiscall*)(const Stream*)>(0x51AD60);
	const auto NI_Stream_load = reinterpret_cast<bool(__thiscall*)(const Stream*, const char*)>(0x6C41D0);

	Stream::Stream() {
		NI_Stream_ctor(this);
	}

	Stream::~Stream() {
		NI_Stream_dtor(this);
	}

	bool Stream::load(const char * path) {
		return NI_Stream_load(this, path);
	}
}
