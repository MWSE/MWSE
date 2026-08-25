#include "NITriangleBVH.h"

#include "NIPoint3.h"
#include "NITriangle.h"

namespace NI {
	static_assert(sizeof(Point3) == 12, "NI::Point3 must be three packed floats; vertices are indexed as float[3].");

	// Smaller leaves shorten the candidate lists handed to the exact tests.
	constexpr auto maximumLeafTriangles = 4u;

	// Median splits bound the depth by log2(triangleCount).
	constexpr auto maximumTraversalDepth = 64u;

	// Absorbs float rounding in the traversal tests, scaled with coordinate magnitude.
	static float roundingPadding(const float* minimum, const float* maximum) {
		auto maxAbs = 0.0f;
		for (auto axis = 0; axis < 3; ++axis) {
			maxAbs = std::max({ maxAbs, std::fabs(minimum[axis]), std::fabs(maximum[axis]) });
		}
		return 1e-4f * (1.0f + maxAbs);
	}

	unsigned int TriangleBVH::buildNode(unsigned int begin, unsigned int end, const std::vector<AABB>& triangleBounds) {
		const auto nodeIndex = static_cast<unsigned int>(nodes.size());
		nodes.emplace_back();

		auto bounds = triangleBounds[triangleIndices[begin]];
		for (auto i = begin + 1; i < end; ++i) {
			const auto& triangle = triangleBounds[triangleIndices[i]];
			for (auto axis = 0; axis < 3; ++axis) {
				bounds.min[axis] = std::min(bounds.min[axis], triangle.min[axis]);
				bounds.max[axis] = std::max(bounds.max[axis], triangle.max[axis]);
			}
		}
		nodes[nodeIndex].bounds = bounds;

		const auto count = end - begin;
		if (count <= maximumLeafTriangles) {
			nodes[nodeIndex].firstOrRight = begin;
			nodes[nodeIndex].triangleCount = count;
			return nodeIndex;
		}

		// Split at the centroid median of the widest axis to keep the tree balanced.
		auto splitAxis = 0;
		for (auto axis = 1; axis < 3; ++axis) {
			if (bounds.max[axis] - bounds.min[axis] > bounds.max[splitAxis] - bounds.min[splitAxis]) {
				splitAxis = axis;
			}
		}
		const auto mid = begin + count / 2;
		std::nth_element(
			triangleIndices.begin() + begin,
			triangleIndices.begin() + mid,
			triangleIndices.begin() + end,
			[&](unsigned int a, unsigned int b) {
				return triangleBounds[a].min[splitAxis] + triangleBounds[a].max[splitAxis]
					< triangleBounds[b].min[splitAxis] + triangleBounds[b].max[splitAxis];
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

		std::vector<AABB> triangleBounds(triangleCount);
		for (auto i = 0u; i < triangleCount; ++i) {
			const auto& triangle = triangles[i];
			const float* corners[3] = {
				&vertices[triangle.vertices[0]].x,
				&vertices[triangle.vertices[1]].x,
				&vertices[triangle.vertices[2]].x,
			};
			auto& box = triangleBounds[i];
			for (auto axis = 0; axis < 3; ++axis) {
				box.min[axis] = std::min({ corners[0][axis], corners[1][axis], corners[2][axis] });
				box.max[axis] = std::max({ corners[0][axis], corners[1][axis], corners[2][axis] });
			}
			const auto padding = roundingPadding(box.min, box.max);
			for (auto axis = 0; axis < 3; ++axis) {
				box.min[axis] -= padding;
				box.max[axis] += padding;
			}
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
	void TriangleBVH::collect(BoundsTest&& intersectsBounds, std::vector<unsigned int>& out_candidates) const {
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
					out_candidates.push_back(triangleIndices[node.firstOrRight + i]);
				}
			}
			else {
				stack[stackSize++] = nodeIndex + 1;
				stack[stackSize++] = node.firstOrRight;
			}
		}
	}

	void TriangleBVH::collectRayCandidates(const Point3& origin, const Point3& direction, std::vector<unsigned int>& out_candidates) const {
		const float rayOrigin[3] = { origin.x, origin.y, origin.z };
		const float rayDirection[3] = { direction.x, direction.y, direction.z };
		float inverseDirection[3] = {};
		bool axisIsFlat[3] = {};
		for (auto axis = 0; axis < 3; ++axis) {
			axisIsFlat[axis] = rayDirection[axis] == 0.0f;
			inverseDirection[axis] = axisIsFlat[axis] ? 0.0f : 1.0f / rayDirection[axis];
		}

		// Slab test over t in [0, +inf). Axes with zero direction fall back to a containment test.
		collect([&](const AABB& bounds) {
			auto tEnter = 0.0f;
			auto tExit = std::numeric_limits<float>::infinity();
			for (auto axis = 0; axis < 3; ++axis) {
				if (axisIsFlat[axis]) {
					if (rayOrigin[axis] < bounds.min[axis] || rayOrigin[axis] > bounds.max[axis]) {
						return false;
					}
					continue;
				}
				auto t1 = (bounds.min[axis] - rayOrigin[axis]) * inverseDirection[axis];
				auto t2 = (bounds.max[axis] - rayOrigin[axis]) * inverseDirection[axis];
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
		}, out_candidates);
	}

	void TriangleBVH::collectBoxCandidates(const Point3& minimum, const Point3& maximum, std::vector<unsigned int>& out_candidates) const {
		const float queryMin[3] = { minimum.x, minimum.y, minimum.z };
		const float queryMax[3] = { maximum.x, maximum.y, maximum.z };

		collect([&](const AABB& bounds) {
			for (auto axis = 0; axis < 3; ++axis) {
				if (bounds.min[axis] > queryMax[axis] || bounds.max[axis] < queryMin[axis]) {
					return false;
				}
			}
			return true;
		}, out_candidates);
	}
}
