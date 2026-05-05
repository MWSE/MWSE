#pragma once

#include "NIGeometry.h"

#include "NILinesData.h"

namespace NI {
	struct Lines : Geometry {
		// Engine-dispatch ctor/dtor (impls in SharedSE/NILines.cpp). Only
		// declared on targets where SharedSE/NILines.cpp is in the build
		// (currently CSSE; MWSE doesn't need them and never calls them, so
		// gating the decls avoids unresolved-external link errors when
		// MWSE-private code stamps out a sol usertype<Lines>).
#if defined(SE_NI_LINES_FNADDR_CTOR) && SE_NI_LINES_FNADDR_CTOR > 0
		Lines(unsigned short vertexCount, Vector3* vertices, PackedColor* colors, Vector2* textureCoords, bool* lineSegmentFlags);
		~Lines();
#endif

		Pointer<LinesData> getModelData() const;

		static Pointer<Lines> create(unsigned short vertexCount, bool useColors = false, bool useTextureCoords = false);
		static Pointer<Lines> create(unsigned short vertexCount, Vector3* vertices, PackedColor* colors, Vector2* textureCoords, bool* lineSegmentFlags);

	};
	static_assert(sizeof(Lines) == 0xAC, "NI::Lines failed size validation");
}