module;

#include "NIDefines.h"

#include "NIPointer.h"
#include "NITArray.h"

#include "SolUtil.h"

export module NIObject;

namespace NI {
	export struct Object {
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

		static void* operator new(size_t size);
		static void operator delete(void* block);

		//
		// vTable Wrappers
		//

		RTTI* getRunTimeTypeInformation() const;
		bool registerStreamables(Stream* stream);
		void loadBinary(Stream* stream);
		bool isEqual(Object* other) const;

		//
		// Other related this-call functions.
		//

		Object* createClone();

		void release();

		// Slow, but name-based lookup of nodes.
		bool isOfType(const RTTI*) const;
		bool isOfType(uintptr_t rtti) const { return isOfType(reinterpret_cast<RTTI*>(rtti)); }
		bool isInstanceOfType(const RTTI*) const;
		bool isInstanceOfType(uintptr_t rtti) const { return isInstanceOfType(reinterpret_cast<RTTI*>(rtti)); }

		//
		// Custom functions.
		//

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		// Storage for cached userdata.
		sol::object getOrCreateLuaObject(lua_State* L);
		static void clearCachedLuaObject(const Object* object);
		static void clearCachedLuaObjects();
#endif

		//
		// Custom functions.
		//

		bool saveBinary(const char* filename);

		std::string toString() const;
		std::string toJson() const;

#if defined(SE_NI_OBJECT_FNADDR_REGISTERSTREAMABLES) && SE_NI_OBJECT_FNADDR_REGISTERSTREAMABLES > 0
		static constexpr auto _registerStreamables = reinterpret_cast<bool(__thiscall*)(Object*, Stream*)>(SE_NI_OBJECT_FNADDR_REGISTERSTREAMABLES);
#endif

	};
	static_assert(sizeof(Object) == 0x8, "NI::Object failed size validation");

	export struct Object_vTable {
		void(__thiscall* destructor)(Object*, int); // 0x0
		RTTI* (__thiscall* getRTTI)(const Object*); // 0x4
		Object* (__thiscall* createClone)(Object*); // 0x8
		void(__thiscall* loadBinary)(Object*, Stream*); // 0xC
		void(__thiscall* linkObject)(Object*, Stream*); // 0x10
		bool(__thiscall* registerStreamables)(Object*, Stream*); // 0x14
		void(__thiscall* saveBinary)(Object*, Stream*); // 0x18
		bool(__thiscall* isEqual)(const Object*, const Object*); // 0x1C
		void(__thiscall* addViewerStrings)(Object*, TArray<char*>*); // 0x20
		void(__thiscall* processClone)(Object*); // 0x24
		void(__thiscall* createRendererData)(Object*); // 0x28
	};
	static_assert(sizeof(Object_vTable) == 0x2C, "NI::Object's vtable failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Object)
#endif
