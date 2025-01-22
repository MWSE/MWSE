#include "NITriShape.h"

#include "NiTriShapeData.h"

namespace NI {
	TriShape::TriShape(TriBasedGeometryData* data) : TriBasedGeometry(data) {
		vTable.asObject = reinterpret_cast<Object_vTable*>(0x7508B0);
	}

	TriShapeData* TriShape::getModelData() const {
		return static_cast<TriShapeData*>(modelData.get());
	}

	const auto NI_Geometry_LinkObject = reinterpret_cast<void(__thiscall*)(NI::Geometry*, NI::Stream*)>(0x6F47D0);
	void TriShape::linkObject(NI::Stream* stream) {
		NI_Geometry_LinkObject(this, stream);

		// Set MWSE-added software skinning flag in linked data.
		if (flags & TriShapeFlags::SoftwareSkinningFlag) {
			auto data = static_cast<NI::TriBasedGeometryData*>(modelData.get());
			data->patchRenderFlags |= TriShapeFlags::SoftwareSkinningFlag;
		}
	}

	Pointer<TriShape> TriShape::create(unsigned short vertexCount, bool hasNormals, bool hasColors, unsigned short textureCoordSets, unsigned short triangleCount) {
		auto data = TriShapeData::create(vertexCount, hasNormals, hasColors, textureCoordSets, triangleCount);
		auto shape = new TriShape(data);

		return shape;
	}

	nonstd::span<TES3::Vector3> TriShape::getVertices() const {
		return getModelData()->getVertices();
	}

	nonstd::span<TES3::Vector3> TriShape::getNormals() const {
		return getModelData()->getNormals();
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TriShape)
