#include "NIStream.h"

#include "MemoryUtil.h"

namespace NI {
	const auto NI_Stream_ctor = reinterpret_cast<void(__thiscall*)(Stream*)>(0x6C2B60);
	Stream::Stream() {
		NI_Stream_ctor(this);
	}

	const auto NI_Stream_dtor = reinterpret_cast<void(__thiscall*)(Stream*)>(0x6C2DD0);
	Stream::~Stream() {
		NI_Stream_dtor(this);
	}

	const auto NI_Stream_load = reinterpret_cast<bool(__thiscall*)(Stream*, const char*)>(0x6C41D0);
	bool Stream::load(const char* path) {
		return NI_Stream_load(this, path);
	}

	const auto NI_Stream_save = reinterpret_cast<bool(__thiscall*)(Stream*, const char*)>(0x6C4710);
	bool Stream::save(const char* path) {
		return NI_Stream_save(this, path);
	}

	const auto NI_Stream_insertObject = reinterpret_cast<void(__thiscall*)(Stream*, Object*)>(0x6C2FD0);
	void Stream::insertObject(Object* object) {
		NI_Stream_insertObject(this, object);
	}

	void Stream::readString(char** string) {
		_readString(this, string);
	}

	void Stream::writeString(const char* string) {
		_writeString(this, string);
	}

	void __cdecl Stream::registerLoader(const char* className, CreateFunction createObjectFunction) {
		_registerLoader(className, createObjectFunction);
	}

	std::string Stream::readStdString() {
		char* string = nullptr;
		readString(&string);

		std::string out_string = string;

		mwse::tes3::_delete(string);

		return std::move(out_string);
	}
}
