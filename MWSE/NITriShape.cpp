#include "NITriShape.h"

#include "NiTriShapeData.h"

namespace NI {
	TriShapeData* TriShape::getModelData() const {
		return static_cast<TriShapeData*>(modelData.get());
	}

	nonstd::span<TES3::Vector3> TriShape::getVertices() const {
		return getModelData()->getVertices();
	}

	nonstd::span<TES3::Vector3> TriShape::getNormals() const {
		return getModelData()->getNormals();
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TriShape)
