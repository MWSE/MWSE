#include "NIGeometry.h"

namespace NI {
	Pointer<GeometryData> Geometry::getModelData() const {
		return modelData;
	}

	void Geometry::setModelData(GeometryData* data) {
		vTable.asGeometry->setModelData(this, data);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Geometry)
