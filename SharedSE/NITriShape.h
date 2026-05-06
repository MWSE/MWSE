#pragma once

#include "NIAVObject.h"
#include "NITriBasedGeometry.h"

namespace NI {
	namespace TriShapeFlags {
		typedef unsigned short value_type;

		enum TriShapeFlags {
			SoftwareSkinningFlag = 0x200	// Added by MWSE.
		};
	}

	struct TriShape_vTable : TriBasedGeometry_vTable {
		void* unknown_0x9C;
		void* unknown_0xA0;
		void* unknown_0xA4;
		void* unknown_0xA8;
	};
	static_assert(sizeof(TriShape_vTable) == 0xAC, "NI::TriShape's vtable failed size validation");

	struct TriShape : TriBasedGeometry {

		// Engine-dispatch ctor: delegates to TriBasedGeometry's engine ctor and
		// patches in the TriShape vTable. Body throws not_implemented_exception
		// when the per-target engine address is 0x0.
		TriShape(TriBasedGeometryData* data);

		// Many-arg engine ctor + dtor. Engine dispatch via SE_NI_TRISHAPE_FNADDR_CTOR;
		// body throws not_implemented_exception when address is 0x0.
		TriShape(unsigned short vertexCount, Vector3* vertices, Vector3* normals, Color* colors, Vector2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags);
		~TriShape();

		//
		// vTable type overwriting.
		//

		Pointer<TriShapeData> getModelData() const;
		void linkObject(Stream* stream);

		//
		// Custom functions.
		//

		// Simple-flags create: allocates a TriShapeData via TriShapeData::create(...) and wraps it.
		static Pointer<TriShape> create(unsigned short vertexCount, bool hasNormals, bool hasColors, unsigned short textureCoordSets, unsigned short triangleCount);

		// Many-arg create overload paralleling the many-arg ctor above. Gated
		// out of MWSE because the lua usertype binds &TriShape::create by name
		// (no static_cast disambiguation), and the simple-flags overload is the
		// one MWSE callers want. CSSE-side code drives the many-arg path.
#if !defined(SE_IS_MWSE) || SE_IS_MWSE == 0
		static Pointer<TriShape> create(unsigned short vertexCount, Vector3* vertices, Vector3* normals, Color* colors, Vector2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags);
#endif

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// Convenient access to model data. Depends on MWSE-side
		// non-const accessors in NIGeometryData (CSSE branch lacks
		// getNormals and uses a const-only getVertices).
		nonstd::span<Vector3> getVertices() const;
		nonstd::span<Vector3> getNormals() const;
#endif

	};
	static_assert(sizeof(TriShape) == 0xAC, "NI::TriShape failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::TriShape)
#endif
