#pragma once

#include "NIAVObject.h"
#include "NITriBasedGeometry.h"

namespace NI {
	struct TriShape_vTable : TriBasedGeometry_vTable {
		void* unknown_0xA4;
		void* unknown_0xA8;
	};
	static_assert(sizeof(TriShape_vTable) == 0xAC, "NI::TriShape's vtable failed size validation");

	struct TriShape : TriBasedGeometry {

		TriShape(unsigned short vertexCount, Vector3* vertices, Vector3* normals, Color* colors, Vector2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags);
		~TriShape();

		static Pointer<TriShape> create(unsigned short vertexCount, Vector3* vertices, Vector3* normals, Color* colors, Vector2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags);

		//
		// vTable type overwriting.
		//

		Pointer<TriShapeData> getModelData() const;

		// Convenient access to model data.
		nonstd::span<Vector3> getVertices() const;
		nonstd::span<Vector3> getNormals() const;

	};
	static_assert(sizeof(TriShape) == 0xAC, "NI::TriShape failed size validation");
}
