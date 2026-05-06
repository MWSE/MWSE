#include "NIGeometryData.h"

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
		// SphereBound (MWSE branch) and Bound (CSSE branch) share identical
		// memory layout (Vector3 center + float radius, 0x10 bytes); the
		// engine fn lives on Bound, so reinterpret across the typedef gap.
		reinterpret_cast<Bound*>(&bounds)->computeFromData(vertexCount, vertex, sizeof(Vector3));
	}

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
	nonstd::span<PackedColor> GeometryData::getColors() {
		if (color) {
			return nonstd::span(color, vertexCount);
		}
		return {};
	}

	nonstd::span<Vector3> GeometryData::getVertices() {
		if (vertex) {
			return nonstd::span(vertex, vertexCount);
		}
		return {};
	}

	nonstd::span<Vector3> GeometryData::getActiveVertices() {
		const auto count = getActiveVertexCount();
		if (vertex && count > 0) {
			return nonstd::span(vertex, count);
		}
		return {};
	}

	nonstd::span<Vector3> GeometryData::getNormals() {
		if (normal) {
			return nonstd::span(normal, vertexCount);
		}
		return {};
	}

	nonstd::span<Vector2> GeometryData::getTextureCoordinates() {
		if (textureCoords) {
			return nonstd::span(textureCoords, vertexCount * textureSets);
		}
		return {};
	}
#else
	nonstd::span<Vector3> GeometryData::getVertices() const {
		if (vertex) {
			return nonstd::span(vertex, vertexCount);
		}
		return {};
	}
#endif
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::GeometryData)
#endif
