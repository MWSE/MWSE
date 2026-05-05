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

		// MWSE-canonical engine-dispatch ctor (delegates to TriBasedGeometry's
		// engine ctor; sets vTable to NiTriShape's).
		TriShape(TriBasedGeometryData* data);

		// SharedSE/CSSE many-arg ctor + dtor. Engine-dispatch impls live in
		// SharedSE/NITriShape.cpp -- which doesn't currently exist. Gate on
		// !SE_IS_MWSE so MWSE-side sol usertype<TriShape> doesn't trigger
		// LNK2001 on a dtor that has no impl in either build.
#if !defined(SE_IS_MWSE) || SE_IS_MWSE == 0
		TriShape(unsigned short vertexCount, Vector3* vertices, Vector3* normals, Color* colors, Vector2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags);
		~TriShape();
#endif

		//
		// vTable type overwriting.
		//

		Pointer<TriShapeData> getModelData() const;
		void linkObject(Stream* stream);

		//
		// Custom functions.
		//

		// MWSE-canonical create: simple-flags variant; allocates a
		// TriShapeData via TriShapeData::create(...) and wraps it.
		static Pointer<TriShape> create(unsigned short vertexCount, bool hasNormals, bool hasColors, unsigned short textureCoordSets, unsigned short triangleCount);

#if !defined(SE_IS_MWSE) || SE_IS_MWSE == 0
		// SharedSE/CSSE create overload paralleling the many-arg ctor above.
		static Pointer<TriShape> create(unsigned short vertexCount, Vector3* vertices, Vector3* normals, Color* colors, Vector2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags);
#endif

		// Convenient access to model data.
		nonstd::span<Vector3> getVertices() const;
		nonstd::span<Vector3> getNormals() const;

	};
	static_assert(sizeof(TriShape) == 0xAC, "NI::TriShape failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::TriShape)
#endif
