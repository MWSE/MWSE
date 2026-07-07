#include "NITriBasedGeometryData.h"

#include "NIPoint3.h"
#include "NITriangle.h"

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

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
	//
	// Candidate triangle enumeration, backed by a cached per-mesh BVH.
	//

	static_assert(sizeof(Point3) == 12, "NI::Point3 must be three packed floats; vertices are indexed as float[3].");

	// Below this size the exhaustive loop is cheaper than the BVH.
	constexpr auto minimumTriangleCount = 64u;

	// Safety valve; the cache rebuilds on demand after a wholesale drop.
	constexpr auto maximumCacheEntries = size_t(4096);

	// Larger leaves shrink the tree; smaller leaves shorten the candidate lists
	// handed back to the engine-exact tests, which is where the time goes.
	constexpr auto maximumLeafTriangles = 4u;

	// Median splits halve each range, so tree depth never exceeds
	// log2(triangleCount) and this covers any 32-bit triangle count.
	constexpr auto maximumTraversalStack = 64u;

	struct AABB {
		float min[3];
		float max[3];
	};

	// Candidate enumeration must never miss a triangle the engine's exact tests
	// would hit; padding absorbs float rounding in the traversal tests, scaled
	// with coordinate magnitude so large model-space meshes stay covered.
	static float roundingPadding(const AABB& box) {
		auto maxAbs = 0.0f;
		for (auto axis = 0; axis < 3; ++axis) {
			maxAbs = std::max({ maxAbs, std::fabs(box.min[axis]), std::fabs(box.max[axis]) });
		}
		return 1e-4f * (1.0f + maxAbs);
	}

	struct BVHNode {
		AABB bounds;
		// Leaf when triangleCount > 0: firstOrRight indexes triangleIndices.
		// Internal otherwise: the left child follows this node in the array and
		// firstOrRight is the right child's index.
		unsigned int firstOrRight = 0;
		unsigned int triangleCount = 0;
	};

	// Conservative segment-vs-AABB slab test over t in [0, 1]. Axes with zero
	// delta cannot produce finite slab times and fall back to a containment test.
	static bool segmentIntersectsBounds(const float origin[3], const float invDelta[3], const bool axisIsFlat[3], const AABB& bounds) {
		auto tEnter = 0.0f;
		auto tExit = 1.0f;
		for (auto axis = 0; axis < 3; ++axis) {
			if (axisIsFlat[axis]) {
				if (origin[axis] < bounds.min[axis] || origin[axis] > bounds.max[axis]) {
					return false;
				}
				continue;
			}
			auto t1 = (bounds.min[axis] - origin[axis]) * invDelta[axis];
			auto t2 = (bounds.max[axis] - origin[axis]) * invDelta[axis];
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
	}

	class TriangleBVH {
	public:
		void build(const Triangle* triList, unsigned int triangleCount, const Point3* vertices);

		void collectRayCandidates(const Point3& from, const Point3& to, std::vector<unsigned int>& out_candidates) const;
		void collectBoxCandidates(const Point3& aabbMin, const Point3& aabbMax, std::vector<unsigned int>& out_candidates) const;

	private:
		unsigned int buildNode(unsigned int begin, unsigned int end, const std::vector<AABB>& triangleBounds);

		std::vector<BVHNode> nodes;
		std::vector<unsigned int> triangleIndices;
	};

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

		// Split at the centroid median of the widest axis. The median keeps the
		// tree balanced regardless of triangle distribution.
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

	void TriangleBVH::build(const Triangle* triList, unsigned int triangleCount, const Point3* vertices) {
		std::vector<AABB> triangleBounds(triangleCount);
		for (auto i = 0u; i < triangleCount; ++i) {
			const auto& triangle = triList[i];
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
			const auto padding = roundingPadding(box);
			for (auto axis = 0; axis < 3; ++axis) {
				box.min[axis] -= padding;
				box.max[axis] += padding;
			}
		}

		triangleIndices.resize(triangleCount);
		for (auto i = 0u; i < triangleCount; ++i) {
			triangleIndices[i] = i;
		}

		nodes.clear();
		nodes.reserve(triangleCount);
		buildNode(0, triangleCount, triangleBounds);
	}

	void TriangleBVH::collectRayCandidates(const Point3& from, const Point3& to, std::vector<unsigned int>& out_candidates) const {
		const float origin[3] = { from.x, from.y, from.z };
		const float delta[3] = { to.x - from.x, to.y - from.y, to.z - from.z };
		float invDelta[3] = {};
		bool axisIsFlat[3] = {};
		for (auto axis = 0; axis < 3; ++axis) {
			axisIsFlat[axis] = delta[axis] == 0.0f;
			invDelta[axis] = axisIsFlat[axis] ? 0.0f : 1.0f / delta[axis];
		}

		unsigned int stack[maximumTraversalStack];
		auto stackSize = 0u;
		stack[stackSize++] = 0u;
		while (stackSize > 0) {
			const auto nodeIndex = stack[--stackSize];
			const auto& node = nodes[nodeIndex];
			if (!segmentIntersectsBounds(origin, invDelta, axisIsFlat, node.bounds)) {
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

	void TriangleBVH::collectBoxCandidates(const Point3& aabbMin, const Point3& aabbMax, std::vector<unsigned int>& out_candidates) const {
		const float queryMin[3] = { aabbMin.x, aabbMin.y, aabbMin.z };
		const float queryMax[3] = { aabbMax.x, aabbMax.y, aabbMax.z };

		unsigned int stack[maximumTraversalStack];
		auto stackSize = 0u;
		stack[stackSize++] = 0u;
		while (stackSize > 0) {
			const auto nodeIndex = stack[--stackSize];
			const auto& node = nodes[nodeIndex];
			auto overlaps = true;
			for (auto axis = 0; axis < 3; ++axis) {
				if (node.bounds.min[axis] > queryMax[axis] || node.bounds.max[axis] < queryMin[axis]) {
					overlaps = false;
					break;
				}
			}
			if (!overlaps) {
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

	static bool isSameTriangle(const Triangle& a, const Triangle& b) {
		return a.vertices[0] == b.vertices[0]
			&& a.vertices[1] == b.vertices[1]
			&& a.vertices[2] == b.vertices[2];
	}

	struct CacheEntry {
		// The BVH stores triangle indices and bounds only; it is only consulted
		// after revalidation against the engine arrays via the fields below.
		TriangleBVH bvh;
		const Triangle* triList = nullptr;
		const Point3* vertices = nullptr;
		unsigned int triangleCount = 0;
		unsigned int vertexCount = 0;
		unsigned short revisionID = 0;

		// Content probe. Engine meshes usually carry revisionID 0, so a recycled
		// allocation could match every pointer and count above while holding a
		// different mesh; the model bound and edge triangles must also match.
		Bound modelBound = {};
		Triangle firstTriangle;
		Triangle lastTriangle;

		// Failed builds stay cached so they aren't retried every query.
		bool usable = false;

		bool matches(const TriBasedGeometryData* data, const Triangle* triList_, unsigned int triangleCount_) const {
			return triList == triList_
				&& vertices == data->vertex
				&& triangleCount == triangleCount_
				&& vertexCount == data->vertexCount
				&& revisionID == data->revisionID
				&& modelBound.center == data->bounds.center
				&& modelBound.radius == data->bounds.radius
				&& isSameTriangle(firstTriangle, triList_[0])
				&& isSameTriangle(lastTriangle, triList_[triangleCount_ - 1]);
		}
	};

	static std::unordered_map<const TriBasedGeometryData*, CacheEntry> cache;
	static std::vector<unsigned int> candidateScratch;

	static void buildEntry(CacheEntry& entry, const TriBasedGeometryData* data, const Triangle* triList, unsigned int triangleCount) {
		entry.usable = false;
		entry.triList = triList;
		entry.vertices = data->vertex;
		entry.triangleCount = triangleCount;
		entry.vertexCount = data->vertexCount;
		entry.revisionID = data->revisionID;
		entry.modelBound = data->bounds;
		entry.firstTriangle = triList[0];
		entry.lastTriangle = triList[triangleCount - 1];

		try {
			entry.bvh.build(triList, triangleCount, data->vertex);
			entry.usable = true;
		}
		catch (...) {
			entry.bvh = {};
		}
	}

	static CacheEntry* getUsableEntry(const TriBasedGeometryData* data, const Triangle* triList, unsigned int triangleCount) {
		if (!triList || !data->vertex || triangleCount < minimumTriangleCount) {
			return nullptr;
		}

		if (cache.size() >= maximumCacheEntries) {
			cache.clear();
		}

		auto& entry = cache[data];
		if (!entry.matches(data, triList, triangleCount)) {
			buildEntry(entry, data, triList, triangleCount);
		}
		return entry.usable ? &entry : nullptr;
	}

	const std::vector<unsigned int>* TriBasedGeometryData::getRayCandidateTriangles(const Point3& modelOrigin, const Point3& modelDirection) const {
		const auto entry = getUsableEntry(this, getTriList(), getActiveTriangleCount());
		if (!entry) {
			return nullptr;
		}

		// The traversal wants a finite segment. Every vertex lies inside the model
		// bound sphere, so any hittable triangle is hit within [origin, sphere exit].
		const auto dd = modelDirection.dotProduct(&modelDirection);
		if (dd <= 0.0f) {
			return nullptr;
		}
		const auto originToCenter = bounds.center - modelOrigin;
		const auto b = modelDirection.dotProduct(&originToCenter);
		const auto radius = bounds.radius * 1.01f + 1.0f;
		const auto c = originToCenter.dotProduct(&originToCenter) - radius * radius;
		const auto discriminant = b * b - dd * c;
		candidateScratch.clear();
		if (discriminant < 0.0f) {
			return &candidateScratch;
		}
		const auto exitDistance = (b + std::sqrt(discriminant)) / dd;
		if (exitDistance <= 0.0f) {
			return &candidateScratch;
		}

		const auto rayEnd = modelOrigin + modelDirection * exitDistance;
		entry->bvh.collectRayCandidates(modelOrigin, rayEnd, candidateScratch);

		// Ascending order keeps results identical to the exhaustive loop.
		std::sort(candidateScratch.begin(), candidateScratch.end());

		return &candidateScratch;
	}

	const std::vector<unsigned int>* TriBasedGeometryData::getAabbCandidateTriangles(const Point3& modelAabbMin, const Point3& modelAabbMax) const {
		const auto entry = getUsableEntry(this, getTriList(), triangleCount);
		if (!entry) {
			return nullptr;
		}

		candidateScratch.clear();
		entry->bvh.collectBoxCandidates(modelAabbMin, modelAabbMax, candidateScratch);

		// Ascending order keeps results identical to the exhaustive loop.
		std::sort(candidateScratch.begin(), candidateScratch.end());

		return &candidateScratch;
	}
#endif
}
