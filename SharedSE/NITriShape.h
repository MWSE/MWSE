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
		void* unknown_0xA4;
		void* unknown_0xA8;
	};
	static_assert(sizeof(TriShape_vTable) == 0xAC, "NI::TriShape's vtable failed size validation");

	struct TriShape : TriBasedGeometry {

		TriShape(TriBasedGeometryData* data);

		TriShape(unsigned short vertexCount, Point3* vertices, Point3* normals, Color* colors, Point2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags);
		~TriShape();

		//
		// vTable type overwriting.
		//

		Pointer<TriShapeData> getModelData() const;
		void linkObject(Stream* stream);

		//
		// Custom functions.
		//

		static Pointer<TriShape> create(unsigned short vertexCount, bool hasNormals, bool hasColors, unsigned short textureCoordSets, unsigned short triangleCount);

#if !defined(SE_IS_MWSE) || SE_IS_MWSE == 0
		static Pointer<TriShape> create(unsigned short vertexCount, Point3* vertices, Point3* normals, Color* colors, Point2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags);
#endif

		std::span<Point3> getVertices() const;
		std::span<Point3> getNormals() const;

	};
	static_assert(sizeof(TriShape) == 0xAC, "NI::TriShape failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::TriShape)
#endif
