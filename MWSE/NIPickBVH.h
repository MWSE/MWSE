#pragma once

#include "NIDefines.h"

namespace mwse::pickbvh {
	// Returns the indices (ascending) of triangles whose bounds a model-space
	// ray may pass through, or nullptr when no acceleration structure is
	// available and the caller must test every triangle.
	//
	// The returned vector is a shared scratch buffer, only valid until the next
	// call. Picks are engine main-thread only; this module is not thread-safe.
	const std::vector<unsigned int>* getCandidateTriangles(
		const NI::TriBasedGeometryData* data,
		const NI::Triangle* triList,
		unsigned int triangleCount,
		const NI::Point3* vertices,
		unsigned int vertexCount,
		const NI::Point3& modelOrigin,
		const NI::Point3& modelDirection);

	// Drops all cached acceleration structures. They rebuild lazily on demand.
	void clearCache();
}
