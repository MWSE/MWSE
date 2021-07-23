#pragma once

#include "NIGeometryData.h"

namespace NI {
	struct TriBasedGeometryData_vTable : GeometryData_vTable {
		void* unknown_0x38;
		unsigned short(__thiscall* getTriangleCount)(TriBasedGeometryData*); // 0x3C
		void* unknown_0x40;
		void* unknown_0x44;
		void* unknown_0x48;
	};
	static_assert(sizeof(TriBasedGeometryData_vTable) == 0x4C, "NI::TriBasedGeometryData_vTable failed size validation");

	struct TriBasedGeometryData : GeometryData {
		unsigned short triangleCount; // 0x34

		//
		// vTable wrappers.
		//

		unsigned short getTriangleCount();

	};
	static_assert(sizeof(TriBasedGeometryData) == 0x38, "NI::TriBasedGeometryData failed size validation");
}
