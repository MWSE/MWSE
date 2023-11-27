#include "NiTriBasedGeometryData.h"

namespace NI {
	Triangle* TriBasedGeometryData::getTriList() {
		return vTable.asTriBasedGeometryData->getTriList(this);
	}

	const Triangle* TriBasedGeometryData::getTriList() const {
		return vTable.asTriBasedGeometryData->getTriList_const(this);
	}

	unsigned short TriBasedGeometryData::getActiveTriangleCount() const {
		return vTable.asTriBasedGeometryData->getActiveTriangleCount(this);
	}

	void TriBasedGeometryData::setActiveTriangleCount(unsigned short count) {
		return vTable.asTriBasedGeometryData->setActiveTriangleCount(this, count);
	}
}
