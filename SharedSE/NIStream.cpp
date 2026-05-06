#include "NIStream.h"

#include "NIBinaryStream.h"

#include "ExceptionUtil.h"
#include "MemoryUtil.h"

namespace NI {
	//
	// NiStream::LoadingObject
	//

	void* Stream::LoadingObject::operator new(size_t size) {
#if defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
		return se::memory::_new<LoadingObject>();
#else
		throw not_implemented_exception();
#endif
	}

	void Stream::LoadingObject::operator delete(void* block) {
#if defined(SE_MEMORY_FNADDR_DELETE) && SE_MEMORY_FNADDR_DELETE > 0
		se::memory::_delete(block);
#else
		throw not_implemented_exception();
#endif
	}

	Stream::LoadingObject::LoadingObject() {
#if defined(SE_NI_STREAM_LOADINGOBJECT_VTBL) && SE_NI_STREAM_LOADINGOBJECT_VTBL > 0
		vTable = reinterpret_cast<VirtualTable*>(SE_NI_STREAM_LOADINGOBJECT_VTBL);
		unknown_0x4 = 0;
		unknown_0x8 = 0;
		linkId = -1;
#else
		throw not_implemented_exception();
#endif
	}

	//
	// NiStream
	//

	Stream::Stream() {
#if defined(SE_NI_STREAM_FNADDR_CTOR) && SE_NI_STREAM_FNADDR_CTOR > 0
		const auto NI_Stream_ctor = reinterpret_cast<Stream * (__thiscall*)(Stream*)>(SE_NI_STREAM_FNADDR_CTOR);
		NI_Stream_ctor(this);
#else
		throw not_implemented_exception();
#endif
	}

	Stream::~Stream() {
#if defined(SE_NI_STREAM_FNADDR_DTOR) && SE_NI_STREAM_FNADDR_DTOR > 0
		const auto NI_Stream_dtor = reinterpret_cast<void(__thiscall*)(Stream*)>(SE_NI_STREAM_FNADDR_DTOR);
		NI_Stream_dtor(this);
#else
		throw not_implemented_exception();
#endif
	}

	bool Stream::load(const char* path) {
#if defined(SE_NI_STREAM_FNADDR_LOAD) && SE_NI_STREAM_FNADDR_LOAD > 0
		const auto NI_Stream_load = reinterpret_cast<bool(__thiscall*)(Stream*, const char*)>(SE_NI_STREAM_FNADDR_LOAD);
		return NI_Stream_load(this, path);
#else
		throw not_implemented_exception();
#endif
	}

	bool Stream::save(const char* path) {
#if defined(SE_NI_STREAM_FNADDR_SAVE) && SE_NI_STREAM_FNADDR_SAVE > 0
		const auto NI_Stream_save = reinterpret_cast<bool(__thiscall*)(Stream*, const char*)>(SE_NI_STREAM_FNADDR_SAVE);
		return NI_Stream_save(this, path);
#else
		throw not_implemented_exception();
#endif
	}

	void Stream::insertObject(Object* object) {
#if defined(SE_NI_STREAM_FNADDR_INSERTOBJECT) && SE_NI_STREAM_FNADDR_INSERTOBJECT > 0
		const auto NI_Stream_insertObject = reinterpret_cast<void(__thiscall*)(Stream*, Object*)>(SE_NI_STREAM_FNADDR_INSERTOBJECT);
		NI_Stream_insertObject(this, object);
#else
		throw not_implemented_exception();
#endif
	}

	void Stream::readString(char** string) {
#if defined(SE_NI_STREAM_FNADDR_READSTRING) && SE_NI_STREAM_FNADDR_READSTRING > 0
		const auto NI_Stream_readString = reinterpret_cast<void(__thiscall*)(Stream*, char**)>(SE_NI_STREAM_FNADDR_READSTRING);
		NI_Stream_readString(this, string);
#else
		throw not_implemented_exception();
#endif
	}

	void Stream::writeString(const char* string) {
#if defined(SE_NI_STREAM_FNADDR_WRITESTRING) && SE_NI_STREAM_FNADDR_WRITESTRING > 0
		const auto NI_Stream_writeString = reinterpret_cast<void(__thiscall*)(Stream*, const char*)>(SE_NI_STREAM_FNADDR_WRITESTRING);
		NI_Stream_writeString(this, string);
#else
		throw not_implemented_exception();
#endif
	}

	Object* Stream::getLinkObject(int id) const {
#if defined(SE_NI_STREAM_FNADDR_GETLINKOBJECT) && SE_NI_STREAM_FNADDR_GETLINKOBJECT > 0
		const auto NI_Stream_getLinkObject = reinterpret_cast<Object * (__thiscall*)(const Stream*, int)>(SE_NI_STREAM_FNADDR_GETLINKOBJECT);
		return NI_Stream_getLinkObject(this, id);
#else
		throw not_implemented_exception();
#endif
	}

	int Stream::getLinkId(const Object* object) const {
#if defined(SE_NI_STREAM_FNADDR_GETLINKID) && SE_NI_STREAM_FNADDR_GETLINKID > 0
		const auto NI_Stream_getObjectIndex = reinterpret_cast<int(__thiscall*)(const Stream*, const Object*)>(SE_NI_STREAM_FNADDR_GETLINKID);
		return NI_Stream_getObjectIndex(this, object);
#else
		throw not_implemented_exception();
#endif
	}

	void Stream::readLinkId(unsigned int id) {
		inStream->read(&currentlyLoading->linkId);
	}

	void Stream::writeLinkId(Object* object) {
		auto linkId = getLinkId(object);
		outStream->write(&linkId);
	}

	Object* Stream::getCopiedObject(const Object* existing) {
#if defined(SE_NI_STREAM_FNADDR_GETCOPIEDOBJECT) && SE_NI_STREAM_FNADDR_GETCOPIEDOBJECT > 0
		const auto recentlyCopiedObjects = *reinterpret_cast<HashMap<const Object*, Object*>**>(SE_NI_STREAM_FNADDR_GETCOPIEDOBJECT);
		if (auto result = recentlyCopiedObjects->getNode(existing)) {
			return result->value;
		}
		return nullptr;
#else
		throw not_implemented_exception();
#endif
	}

	void __cdecl Stream::registerLoader(const char* className, CreateFunction createObjectFunction) {
#if defined(SE_NI_STREAM_FNADDR_REGISTERLOADER) && SE_NI_STREAM_FNADDR_REGISTERLOADER > 0
		const auto NI_Stream_registerLoader = reinterpret_cast<void(__cdecl*)(const char*, Stream::CreateFunction)>(SE_NI_STREAM_FNADDR_REGISTERLOADER);
		NI_Stream_registerLoader(className, createObjectFunction);
#else
		throw not_implemented_exception();
#endif
	}

	std::string Stream::readStdString() {
		char* string = nullptr;
		readString(&string);

		std::string out_string = string;

#if defined(SE_MEMORY_FNADDR_DELETE) && SE_MEMORY_FNADDR_DELETE > 0
		se::memory::_delete(string);
#endif

		return std::move(out_string);
	}
}
