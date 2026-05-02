#pragma once

#include "NIGeometry.h"

#include "NILinesData.h"

namespace NI {
	struct Lines : Geometry {
		Lines(unsigned short vertexCount, Vector3* vertices, PackedColor* colors, Vector2* textureCoords, bool* lineSegmentFlags);
		~Lines();

		Pointer<LinesData> getModelData() const;

		static Pointer<Lines> create(unsigned short vertexCount, bool useColors = false, bool useTextureCoords = false);
		static Pointer<Lines> create(unsigned short vertexCount, Vector3* vertices, PackedColor* colors, Vector2* textureCoords, bool* lineSegmentFlags);

	};
	static_assert(sizeof(Lines) == 0xAC, "NI::Lines failed size validation");
}