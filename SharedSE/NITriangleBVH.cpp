#include "NITriangleBVH.h"

#include "NIPoint3.h"
#include "NITriangle.h"

namespace NI {
	// Smaller leaves shorten the candidate lists handed to the exact tests.
	constexpr auto maximumLeafTriangles = 4u;

	// Median splits bound the depth by log2(triangleCount).
	constexpr auto maximumTraversalDepth = 64u;

	// Absorbs float rounding in the traversal tests, scaled with coordinate magnitude.
	static float roundingPadding(const BoundingBox& bounds) {
		auto maxAbs = 0.0f;
		for (auto axis = 0u; axis < 3; ++axis) {
			maxAbs = std::max({ maxAbs, std::fabs(bounds.minimum[axis]), std::fabs(bounds.maximum[axis]) });
		}
		return 1e-4f * (1.0f + maxAbs);
	}

	unsigned int TriangleBVH::buildNode(unsigned int begin, unsigned int end, const std::vector<BoundingBox>& triangleBounds) {
		const auto nodeIndex = static_cast<unsigned int>(nodes.size());
		nodes.emplace_back();

		auto bounds = triangleBounds[triangleIndices[begin]];
		for (auto i = begin + 1; i < end; ++i) {
			bounds.merge(triangleBounds[triangleIndices[i]]);
		}
		const auto padding = roundingPadding(bounds);
		bounds.minimum = bounds.minimum - padding;
		bounds.maximum = bounds.maximum + padding;
		nodes[nodeIndex].bounds = bounds;

		const auto count = end - begin;
		if (count <= maximumLeafTriangles) {
			nodes[nodeIndex].firstOrRight = begin;
			nodes[nodeIndex].triangleCount = count;
			return nodeIndex;
		}

		// Split at the centroid median of the widest axis to keep the tree balanced.
		const auto extent = bounds.maximum - bounds.minimum;
		auto splitAxis = 0u;
		for (auto axis = 1u; axis < 3; ++axis) {
			if (extent[axis] > extent[splitAxis]) {
				splitAxis = axis;
			}
		}
		const auto mid = begin + count / 2;
		std::nth_element(
			triangleIndices.begin() + begin,
			triangleIndices.begin() + mid,
			triangleIndices.begin() + end,
			[&](unsigned int a, unsigned int b) {
				return triangleBounds[a].minimum[splitAxis] + triangleBounds[a].maximum[splitAxis]
					< triangleBounds[b].minimum[splitAxis] + triangleBounds[b].maximum[splitAxis];
			});

		buildNode(begin, mid, triangleBounds);
		nodes[nodeIndex].firstOrRight = buildNode(mid, end, triangleBounds);
		return nodeIndex;
	}

	void TriangleBVH::build(const Triangle* triangles, unsigned int triangleCount, const Point3* vertices) {
		nodes.clear();
		triangleIndices.clear();
		if (triangleCount == 0) {
			return;
		}

		std::vector<BoundingBox> triangleBounds(triangleCount);
		for (auto i = 0u; i < triangleCount; ++i) {
			const auto& triangle = triangles[i];
			const auto& vertex0 = vertices[triangle.vertices[0]];
			auto& box = triangleBounds[i];
			box = BoundingBox(vertex0, vertex0);
			box.merge(vertices[triangle.vertices[1]]);
			box.merge(vertices[triangle.vertices[2]]);
		}

		triangleIndices.resize(triangleCount);
		std::iota(triangleIndices.begin(), triangleIndices.end(), 0u);

		nodes.reserve(triangleCount);
		buildNode(0, triangleCount, triangleBounds);
	}

	size_t TriangleBVH::getMemoryUsage() const {
		return nodes.capacity() * sizeof(Node) + triangleIndices.capacity() * sizeof(unsigned int);
	}

	template <typename BoundsTest>
	void TriangleBVH::collect(BoundsTest&& intersectsBounds, std::vector<unsigned int>& outCandidates) const {
		if (nodes.empty()) {
			return;
		}

		unsigned int stack[maximumTraversalDepth];
		auto stackSize = 0u;
		stack[stackSize++] = 0u;
		while (stackSize > 0) {
			const auto nodeIndex = stack[--stackSize];
			const auto& node = nodes[nodeIndex];
			if (!intersectsBounds(node.bounds)) {
				continue;
			}
			if (node.triangleCount > 0) {
				for (auto i = 0u; i < node.triangleCount; ++i) {
					outCandidates.push_back(triangleIndices[node.firstOrRight + i]);
				}
			}
			else {
				stack[stackSize++] = nodeIndex + 1;
				stack[stackSize++] = node.firstOrRight;
			}
		}
	}

	void TriangleBVH::collectRayCandidates(const Point3& origin, const Point3& direction, std::vector<unsigned int>& outCandidates) const {
		Point3 inverseDirection;
		bool axisIsFlat[3] = {};
		for (auto axis = 0u; axis < 3; ++axis) {
			axisIsFlat[axis] = direction[axis] == 0.0f;
			inverseDirection[axis] = axisIsFlat[axis] ? 0.0f : 1.0f / direction[axis];
		}

		// Slab test over t in [0, +inf). Axes with zero direction fall back to a containment test.
		collect([&](const BoundingBox& bounds) {
			auto tEnter = 0.0f;
			auto tExit = std::numeric_limits<float>::infinity();
			for (auto axis = 0u; axis < 3; ++axis) {
				if (axisIsFlat[axis]) {
					if (origin[axis] < bounds.minimum[axis] || origin[axis] > bounds.maximum[axis]) {
						return false;
					}
					continue;
				}
				auto t1 = (bounds.minimum[axis] - origin[axis]) * inverseDirection[axis];
				auto t2 = (bounds.maximum[axis] - origin[axis]) * inverseDirection[axis];
				if (t1 > t2) {
					std::swap(t1, t2);
				}
				tEnter = std::max(tEnter, t1);
				tExit = std::min(tExit, t2);
				if (tEnter > tExit) {
					return false;
				}
			}
			return true;
		}, outCandidates);
	}

	void TriangleBVH::collectBoxCandidates(const BoundingBox& box, std::vector<unsigned int>& outCandidates) const {
		collect([&](const BoundingBox& bounds) { return bounds.intersects(box); }, outCandidates);
	}
}
