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

	const auto NI_Bound_ComputeFromData = reinterpret_cast<void(__thiscall*)(NI::Bound*, int, TES3::Vector3*, unsigned int)>(0x6ED170);
	void GeometryData::updateModelBound() {
		NI_Bound_ComputeFromData(&bounds, vertexCount, vertex, sizeof(TES3::Vector3));
	}

	nonstd::span<PackedColor> GeometryData::getColors() {
		if (color) {
			return nonstd::span(color, vertexCount);
		}
		return {};
	}

	nonstd::span<TES3::Vector3> GeometryData::getVertices() {
		if (vertex) {
			return nonstd::span(vertex, vertexCount);
		}
		return {};
	}

	nonstd::span<TES3::Vector3> GeometryData::getNormals() {
		if (normal) {
			return nonstd::span(normal, vertexCount);
		}
		return {};
	}

	nonstd::span<TES3::Vector2> GeometryData::getTextureCoordinates() {
		if (textureCoords) {
			return nonstd::span(textureCoords, vertexCount * textureSets);
		}
		return {};
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::GeometryData)
