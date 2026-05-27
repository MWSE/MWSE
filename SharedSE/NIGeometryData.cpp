#include "NIGeometryData.h"

#include "NIColor.h"

namespace NI {
	unsigned short GeometryData::getActiveVertexCount() const {
		return vTable.asGeometryData->getActiveVertexCount(this);
	}

	void GeometryData::markAsChanged() {
		++revisionID;
		// Avoid revisionID 0, which implies static data
		if (revisionID == 0) {
			++revisionID;
		}
	}

	void GeometryData::updateModelBound() {
		bounds.computeFromData(vertexCount, vertex, sizeof(Point3));
	}

	std::span<PackedColor> GeometryData::getColors() {
		if (color) {
			return std::span(color, vertexCount);
		}
		return {};
	}

	std::span<Point3> GeometryData::getVertices() {
		if (vertex) {
			return std::span(vertex, vertexCount);
		}
		return {};
	}

	std::span<Point3> GeometryData::getActiveVertices() {
		const auto count = getActiveVertexCount();
		if (vertex && count > 0) {
			return std::span(vertex, count);
		}
		return {};
	}

	std::span<Point3> GeometryData::getNormals() {
		if (normal) {
			return std::span(normal, vertexCount);
		}
		return {};
	}

	std::span<Point2> GeometryData::getTextureCoordinates() {
		if (textureCoords) {
			return std::span(textureCoords, vertexCount * textureSets);
		}
		return {};
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::GeometryData)
#endif
