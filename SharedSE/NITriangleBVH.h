#pragma once

#include "NIDefines.h"

namespace NI {
	// Static BVH over an indexed triangle list. Queries return conservative
	// candidate sets in traversal order: every triangle an exact test could
	// hit is included.
	class TriangleBVH {
	public:
		// The arrays are only read during the build; the tree stores indices.
		void build(const Triangle* triangles, unsigned int triangleCount, const Point3* vertices);

		bool empty() const { return nodes.empty(); }
		size_t getMemoryUsage() const;

		// Ray origin + t * direction, t >= 0.
		void collectRayCandidates(const Point3& origin, const Point3& direction, std::vector<unsigned int>& out_candidates) const;
		void collectBoxCandidates(const Point3& minimum, const Point3& maximum, std::vector<unsigned int>& out_candidates) const;

	private:
		struct AABB {
			float min[3];
			float max[3];
		};

		struct Node {
			AABB bounds;
			// Leaf when triangleCount > 0: firstOrRight indexes triangleIndices.
			// Internal otherwise: the left child follows this node, firstOrRight is the right child.
			unsigned int firstOrRight = 0;
			unsigned int triangleCount = 0;
		};

		unsigned int buildNode(unsigned int begin, unsigned int end, const std::vector<AABB>& triangleBounds);

		template <typename BoundsTest>
		void collect(BoundsTest&& intersectsBounds, std::vector<unsigned int>& out_candidates) const;

		std::vector<Node> nodes;
		std::vector<unsigned int> triangleIndices;
	};
}
