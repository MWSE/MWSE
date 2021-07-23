#include "NIGeometry.h"

namespace NI {
	GeometryData* Geometry::getModelData() {
		return modelData;
	}

	void Geometry::setModelData(GeometryData* data) {
		vTable.asGeometry->setModelData(this, data);
	}
}
