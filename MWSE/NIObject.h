#pragma once

#include "NIDefines.h"

namespace NI {
	struct Object {
		union {
			Object_vTable * asObject;
			AVObject_vTable * asAVObject;
			Node_vTable * asNode;
		} vTable; // 0x0
		int references; // 0x4

		//
		// vTable Wrappers
		//

		RTTI * getRunTimeTypeInformation();

		Object * createClone();

		//
		// Other related this-call functions.
		//

		void release();

		// Slow, but name-based lookup of nodes.
		bool isOfType(const RTTI*);
		bool isOfType(uintptr_t rtti) { return isOfType(reinterpret_cast<RTTI*>(rtti)); }
		bool isInstanceOfType(const RTTI*);
		bool isInstanceOfType(uintptr_t rtti) { return isInstanceOfType(reinterpret_cast<RTTI*>(rtti)); }

	};
	static_assert(sizeof(Object) == 0x8, "NI::Object failed size validation");

	struct Object_vTable {
		void (__thiscall * destructor)(Object*, int); // 0x0
		RTTI * (__thiscall * getRTTI)(Object*); // 0x4
		Object * (__thiscall * createClone)(Object*); // 0x8
		void * loadBinary; // 0xC
		void * linkObject; // 0x10
		void * registerStreamables; // 0x14
		void * saveBinary; // 0x18
		void * isEqual; // 0x1C
		void * getViewerStrings; // 0x20
		void * addViewerStrings; // 0x24
		void * unknown_0x28; // 0x28
	};
	static_assert(sizeof(Object_vTable) == 0x2C, "NI::Object's vtable failed size validation");
}
