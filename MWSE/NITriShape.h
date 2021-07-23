#pragma once

#include "NIAVObject.h"
#include "NiTriBasedGeometry.h"

namespace NI {
	struct TriShape_vTable : TriBasedGeometry_vTable {
		void* unknown_0x9C;
		void* unknown_0xA0;
		void* unknown_0xA4;
		void* unknown_0xA8;
	};
	static_assert(sizeof(TriShape_vTable) == 0xAC, "NI::TriShape's vtable failed size validation");

	struct TriShape : TriBasedGeometry {

		//
		// vTable type overwriting.
		//

		TriShapeData* getModelData() const;

		// Convenient access to model data.
		nonstd::span<TES3::Vector3> getVertices() const;
		nonstd::span<TES3::Vector3> getNormals() const;

	};
	static_assert(sizeof(TriShape) == 0xAC, "NI::TriShape failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::TriShape)
