#pragma once

#include "NITriBasedGeometryData.h"

#include "NITriangle.h"

namespace NI {
	struct TriShapeData : TriBasedGeometryData {
		unsigned int triangleListLength; // 0x38
		Triangle* triangleList; // 0x3C
		void* sharedNormals; // 0x40
		unsigned short sharedNormalsArraySize; // 0x44

		//
		// Other related this-call functions.
		//

		static Pointer<TriShapeData> create(unsigned short _vertexCount, Vector3* _vertices, Vector3* _normals, PackedColor* _colors, Vector2* _textureCoords, unsigned short _triangleCount, Triangle* _triangleList, int unused = 0);
		static Pointer<TriShapeData> create(unsigned short vertexCount, bool hasNormals, bool hasColors, unsigned short textureCoordSets, unsigned short triangleCount);

		//
		// Custom functions.
		//

		// Bool-arg overload (engine field copy with toggles).
		Pointer<TriShapeData> copyData(bool copyNormals = true, bool copyColors = true, bool copyTextureCoordinates = true) const;

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		// Lua-bridge overload (sol::table-driven filters).
		Pointer<TriShapeData> copyData(sol::optional<sol::table> filters) const;
#endif

		nonstd::span<Triangle> getTriangles();

	};
	static_assert(sizeof(TriShapeData) == 0x48, "NI::TriShapeData failed size validation");
}
