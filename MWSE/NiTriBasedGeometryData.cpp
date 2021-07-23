#include "NiTriBasedGeometryData.h"

namespace NI {
	unsigned short TriBasedGeometryData::getTriangleCount() {
		return vTable.asTriBasedGeometryData->getTriangleCount(this);
	}
}
