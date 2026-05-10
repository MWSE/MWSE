#include "NITriShapeData.h"

#include "NIColor.h"

#include "ExceptionUtil.h"
#include "MemoryUtil.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "LuaUtil.h"
#endif

namespace NI {
	Pointer<TriShapeData> TriShapeData::create(unsigned short _vertexCount, Point3* _vertices, Point3* _normals, PackedColor* _colors, Point2* _textureCoords, unsigned short _triangleCount, Triangle* _triangleList, int unused) {
#if defined(SE_NI_TRISHAPEDATA_FNADDR_CREATE) && SE_NI_TRISHAPEDATA_FNADDR_CREATE > 0
		const auto NI_TriShapeData_ctor = reinterpret_cast<void(__thiscall*)(TriShapeData*, unsigned short, Point3*, Point3*, PackedColor*, Point2*, unsigned short, Triangle*, int)>(SE_NI_TRISHAPEDATA_FNADDR_CREATE);

		auto ptr = se::memory::_new<TriShapeData>();
		NI_TriShapeData_ctor(ptr, _vertexCount, _vertices, _normals, _colors, _textureCoords, _triangleCount, _triangleList, unused);
		return ptr;
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<TriShapeData> TriShapeData::create(unsigned short vertexCount, bool hasNormals, bool hasColors, unsigned short textureCoordSets, unsigned short triangleCount) {
		Point3* vertices = se::memory::_new<Point3>(vertexCount);
		ZeroMemory(vertices, sizeof(Point3) * vertexCount);

		Point3* normals = nullptr;
		if (hasNormals) {
			normals = se::memory::_new<Point3>(vertexCount);
			ZeroMemory(normals, sizeof(Point3) * vertexCount);
		}

		PackedColor* colors = nullptr;
		if (hasColors) {
			colors = se::memory::_new<PackedColor>(vertexCount);
			ZeroMemory(colors, sizeof(PackedColor) * vertexCount);
		}

		Point2* textureCoords = nullptr;
		if (textureCoordSets > 0) {
			size_t textureCoordTotal = textureCoordSets * vertexCount;
			textureCoords = se::memory::_new<Point2>(textureCoordTotal);
			ZeroMemory(textureCoords, sizeof(Point2) * textureCoordTotal);
		}

		Triangle* triangleList = nullptr;
		if (triangleCount) {
			triangleList = se::memory::_new<Triangle>(triangleCount);
			ZeroMemory(triangleList, sizeof(Triangle) * triangleCount);
		}

		// Create data and update texture set count after creation, as the constructor call assumes there is only 0 or 1 texture set.
		auto result = create(vertexCount, vertices, normals, colors, textureCoords, triangleCount, triangleList);
		result->textureSets = textureCoordSets;
		return result;
	}

	Pointer<TriShapeData> TriShapeData::copyData(bool copyNormals, bool copyColors, bool copyTextureCoordinates) const {
		auto vertexCount = getActiveVertexCount();

		Point3* _vertices = se::memory::_new<Point3>(vertexCount);
		memcpy_s(_vertices, sizeof(Point3) * vertexCount, vertex, sizeof(Point3) * vertexCount);

		Point3* _normals = nullptr;
		if (normal && copyNormals) {
			_normals = se::memory::_new<Point3>(vertexCount);
			memcpy_s(_normals, sizeof(Point3) * vertexCount, normal, sizeof(Point3) * vertexCount);
		}

		PackedColor* _colors = nullptr;
		if (color && copyColors) {
			_colors = se::memory::_new<PackedColor>(vertexCount);
			memcpy_s(_colors, sizeof(PackedColor) * vertexCount, color, sizeof(PackedColor) * vertexCount);
		}

		Point2* _textureCoords = nullptr;
		if (textureCoords && copyTextureCoordinates) {
			_textureCoords = se::memory::_new<Point2>(vertexCount);
			memcpy_s(_textureCoords, sizeof(Point2) * vertexCount, textureCoords, sizeof(Point2) * vertexCount);
		}

		Triangle* _triangleList = nullptr;
		if (triangleList) {
			_triangleList = se::memory::_new<Triangle>(triangleCount);
			memcpy_s(_triangleList, sizeof(Triangle) * triangleCount, triangleList, sizeof(Triangle) * triangleCount);
		}

		auto result = create(vertexCount, _vertices, _normals, _colors, _textureCoords, triangleCount, _triangleList);

		// Copy over any other necessary data.
		result->bounds = bounds;

		return result;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Pointer<TriShapeData> TriShapeData::copyData_lua(sol::optional<sol::table> filters) const {
		auto vertexCount = getActiveVertexCount();

		Point3* _vertices = se::memory::_new<Point3>(vertexCount);
		memcpy_s(_vertices, sizeof(Point3) * vertexCount, vertex, sizeof(Point3) * vertexCount);

		Point3* _normals = nullptr;
		if (normal && mwse::lua::getOptionalParam(filters, "normals", true)) {
			_normals = se::memory::_new<Point3>(vertexCount);
			memcpy_s(_normals, sizeof(Point3) * vertexCount, normal, sizeof(Point3) * vertexCount);
		}

		PackedColor* _colors = nullptr;
		if (color && mwse::lua::getOptionalParam(filters, "colors", true)) {
			_colors = se::memory::_new<PackedColor>(vertexCount);
			memcpy_s(_colors, sizeof(PackedColor) * vertexCount, color, sizeof(PackedColor) * vertexCount);
		}

		Point2* _textureCoords = nullptr;
		if (textureCoords && mwse::lua::getOptionalParam(filters, "texCoords", true)) {
			size_t textureCoordTotal = textureSets * vertexCount;
			_textureCoords = se::memory::_new<Point2>(textureCoordTotal);
			memcpy_s(_textureCoords, sizeof(Point2) * textureCoordTotal, textureCoords, sizeof(Point2) * textureCoordTotal);
		}

		Triangle* _triangleList = nullptr;
		if (triangleList) {
			_triangleList = se::memory::_new<Triangle>(triangleCount);
			memcpy_s(_triangleList, sizeof(Triangle) * triangleCount, triangleList, sizeof(Triangle) * triangleCount);
		}

		// Create data and update texture set count after creation, as the constructor call assumes there is only 0 or 1 texture set.
		auto result = create(vertexCount, _vertices, _normals, _colors, _textureCoords, triangleCount, _triangleList);
		result->textureSets = _textureCoords ? textureSets : 0;
		result->bounds = bounds;

		return result;
	}
#endif

	nonstd::span<Triangle> TriShapeData::getTriangles() {
		if (triangleList) {
			return { triangleList, getActiveTriangleCount() };
		}
		return {};
	}
}
