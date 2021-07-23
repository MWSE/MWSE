#pragma once

#include "NIDefines.h"
#include "NIPointer.h"

// Must be added to header files that declare Ni types that can be derived.
#define MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(T) \
int sol_lua_push(sol::types<T>, lua_State* L, T& obj); \
int sol_lua_push(sol::types<T*>, lua_State* L, T* obj); \
int sol_lua_push(sol::types<NI::Pointer<T>>, lua_State* L, NI::Pointer<T>& obj); \
int sol_lua_push(sol::types<NI::Pointer<T>*>, lua_State* L, NI::Pointer<T>* obj);

// Must be added to source files that declare Ni types that can be derived.
#define MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(T) \
int sol_lua_push(sol::types<T>, lua_State* L, T& obj) { return obj.getOrCreateLuaObject(L).push(L); } \
int sol_lua_push(sol::types<T*>, lua_State* L, T* obj) { return obj->getOrCreateLuaObject(L).push(L); } \
int sol_lua_push(sol::types<NI::Pointer<T>>, lua_State* L, NI::Pointer<T>& obj) { return obj->getOrCreateLuaObject(L).push(L); } \
int sol_lua_push(sol::types<NI::Pointer<T>*>, lua_State* L, NI::Pointer<T>* obj) { return obj->get()->getOrCreateLuaObject(L).push(L); } 

#include "NITArray.h"

namespace NI {
	struct Object {
		union {
			AVObject_vTable* asAVObject;
			DynamicEffect_vTable* asDynamicEffect;
			Geometry_vTable* asGeometry;
			GeometryData_vTable* asGeometryData;
			Node_vTable* asNode;
			Object_vTable* asObject;
			Property_vTable* asProperty;
			Renderer_vTable* asRenderer;
			SourceTexture_vTable* asSourceTexture;
			Texture_vTable* asTexture;
			TimeController_vTable* asController;
			TriBasedGeometryData_vTable* asTriBasedGeometryData;
			TriShape_vTable* asTriShape;
		} vTable; // 0x0
		int refCount; // 0x4

		//
		// Basic operators.
		//

		Object();
		~Object();

		static void *operator new(size_t size);
		static void operator delete(void *block);

		//
		// vTable Wrappers
		//

		RTTI * getRunTimeTypeInformation() const;

		Object * createClone();

		//
		// Other related this-call functions.
		//

		void release();

		// Slow, but name-based lookup of nodes.
		bool isOfType(const RTTI*) const;
		bool isOfType(uintptr_t rtti) const { return isOfType(reinterpret_cast<RTTI*>(rtti)); }
		bool isInstanceOfType(const RTTI*) const;
		bool isInstanceOfType(uintptr_t rtti) const { return isInstanceOfType(reinterpret_cast<RTTI*>(rtti)); }

		//
		// Custom functions.
		//

		// Storage for cached userdata.
		sol::object getOrCreateLuaObject(lua_State* L);
		static void clearCachedLuaObject(const Object* object);
		static void clearCachedLuaObjects();

		//
		// Custom functions.
		//

		bool saveBinary(const char* filename);

		//
		// Access to this type's raw functions.
		//

		static constexpr auto _registerStreamables = reinterpret_cast<bool(__thiscall*)(Object*, Stream*)>(0x6E9BF0);

	};
	static_assert(sizeof(Object) == 0x8, "NI::Object failed size validation");

	struct Object_vTable {
		void (__thiscall * destructor)(Object*, int); // 0x0
		RTTI * (__thiscall * getRTTI)(const Object*); // 0x4
		Object * (__thiscall * createClone)(Object*); // 0x8
		void (__thiscall * loadBinary)(Object*, Stream*); // 0xC
		void * linkObject; // 0x10
		bool(__thiscall* registerStreamables)(Object*, Stream*); // 0x14
		void (__thiscall* saveBinary)(Object*, Stream*); // 0x18
		bool(__thiscall* isEqual)(Object*, Object*); // 0x1C
		void(__thiscall* addViewerStrings)(Object*, TArray<char*>*); // 0x20
		void * getViewerStrings; // 0x24
		void * unknown_0x28; // 0x28
	};
	static_assert(sizeof(Object_vTable) == 0x2C, "NI::Object's vtable failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Object)
