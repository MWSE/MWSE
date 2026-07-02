#pragma once

#include "NIDefines.h"

namespace mwse::pickbvh {
	// Returns ascending indices of triangles a model-space ray may hit, or
	// nullptr when the caller must test every triangle. The result is a shared
	// scratch buffer, valid until the next call. Not thread-safe.
	const std::vector<unsigned int>* getCandidateTriangles(
		const NI::TriBasedGeometryData* data,
		const NI::Triangle* triList,
		unsigned int triangleCount,
		const NI::Point3* vertices,
		unsigned int vertexCount,
		const NI::Point3& modelOrigin,
		const NI::Point3& modelDirection);

	// Same contract as getCandidateTriangles, for a model-space AABB.
	const std::vector<unsigned int>* getCandidateTrianglesInBox(
		const NI::TriBasedGeometryData* data,
		const NI::Triangle* triList,
		unsigned int triangleCount,
		const NI::Point3* vertices,
		unsigned int vertexCount,
		const NI::Point3& modelAabbMin,
		const NI::Point3& modelAabbMax);

	// Drops all cached acceleration structures; they rebuild on demand.
	void clearCache();
}
