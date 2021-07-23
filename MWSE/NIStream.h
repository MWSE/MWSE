#pragma once

#include "NIDefines.h"

#include "NIPointer.h"
#include "NITArray.h"

#include "TES3HashMap.h"

namespace NI {
	struct Stream {
		int unknown_0x0;
		int unknown_0x4;
		int unknown_0x8;
		int unknown_0xC;
		int unknown_0x10;
		int unknown_0x14;
		int unknown_0x18;
		void* unknown_0x1C;
		int unknown_0x20;
		int unknown_0x24;
		int unknown_0x28;
		int unknown_0x2C;
		TArray<Pointer<Object>> unknown_0x30;
		TES3::HashMap<int, Object*> unknown_0x48;
		BinaryStream* inStream; // 0x58
		BinaryStream* outStream; // 0x5C
		TArray<Pointer<Object>> loadedObjects;
		Object* unknown_0x78;
		TArray<void*> unknown_0x7C;
		TES3::HashMap<int, Object*> unknown_0x94;
		int unknown_0xA4;
		TES3::HashMap<int, Object*>* unknown_0xA8;

		Stream();
		~Stream();

		bool load(const char* file);
		bool save(const char* file);

		void insertObject(Object* object);

		void readString(char** out_string);
		void writeString(const char* string);

		typedef Object* (__cdecl *CreateFunction)(Stream*);
		static void __cdecl registerLoader(const char* className, CreateFunction createObjectFunction);

		//
		// Custom functions.
		//

		std::string readStdString();

		//
		// Access to this type's raw functions.
		//

		static constexpr auto _readString = reinterpret_cast<void(__thiscall*)(Stream*, char**)>(0x6C4C20);
		static constexpr auto _writeString = reinterpret_cast<void(__thiscall*)(Stream*, const char*)>(0x6C4C80);
		static constexpr auto _registerLoader = reinterpret_cast<void(__cdecl*)(const char*, CreateFunction)>(0x6C4460);

	};
	static_assert(sizeof(Stream) == 0xAC, "NI::Stream failed size validation");
}
