#pragma once

#include "NiTriBasedGeometryData.h"

namespace NI {
	struct TriShapeData : TriBasedGeometryData {
		unsigned int triangleListLength; // 0x38
		unsigned short* triangleList; // 0x3C
		void* sharedNormals; // 0x40
		unsigned short sharedNormalsArraySize; // 0x44

		//
		// Other related this-call functions.
		//

		static Pointer<TriShapeData> create(unsigned short _vertexCount, TES3::Vector3* _vertices, TES3::Vector3* _normals, PackedColor* _colors, TES3::Vector2* _textureCoords, unsigned short _triangleCount, unsigned short* _triangleList, int unused = 0);

		//
		// Custom functions.
		//

		Pointer<TriShapeData> copyData(sol::optional<sol::table> filters) const;

	};
	static_assert(sizeof(TriShapeData) == 0x48, "NI::TriShapeData failed size validation");
}
