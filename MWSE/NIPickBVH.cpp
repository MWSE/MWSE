#include "NIPickBVH.h"

#include "NIPoint3.h"
#include "NITriangle.h"
#include "NITriBasedGeometryData.h"

#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btTriangleCallback.h"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"

namespace mwse::pickbvh {
	static_assert(sizeof(NI::Triangle) == 6, "NI::Triangle must be three packed 16-bit indices for zero-copy Bullet meshes.");
	static_assert(sizeof(NI::Point3) == 12, "NI::Point3 must be three packed floats for zero-copy Bullet meshes.");

	// Below this size the exhaustive loop is cheaper than the BVH.
	constexpr auto minimumTriangleCount = 64u;

	// Safety valve; the cache rebuilds on demand after a wholesale drop.
	constexpr auto maximumCacheEntries = size_t(4096);

	struct CacheEntry {
		// Bullet structures reference the engine's vertex/index arrays in place;
		// they are only dereferenced after revalidation against the fields below.
		std::unique_ptr<btTriangleIndexVertexArray> meshInterface;
		std::unique_ptr<btBvhTriangleMeshShape> shape;
		const NI::Triangle* triList = nullptr;
		const NI::Point3* vertices = nullptr;
		unsigned int triangleCount = 0;
		unsigned int vertexCount = 0;
		unsigned short revisionID = 0;

		// Failed builds stay cached so they aren't retried every query.
		bool isUsable() const { return shape != nullptr; }

		bool matches(const NI::TriBasedGeometryData* data, const NI::Triangle* triList_, unsigned int triangleCount_, const NI::Point3* vertices_, unsigned int vertexCount_) const {
			return triList == triList_
				&& vertices == vertices_
				&& triangleCount == triangleCount_
				&& vertexCount == vertexCount_
				&& revisionID == data->revisionID;
		}
	};

	static std::unordered_map<const NI::TriBasedGeometryData*, CacheEntry> cache;
	static std::vector<unsigned int> candidateScratch;

	class CollectTriangleIndices : public btTriangleCallback {
	public:
		void processTriangle(btVector3*, int, int triangleIndex) override {
			candidateScratch.push_back(static_cast<unsigned int>(triangleIndex));
		}
	};

	static void buildEntry(CacheEntry& entry, const NI::TriBasedGeometryData* data, const NI::Triangle* triList, unsigned int triangleCount, const NI::Point3* vertices, unsigned int vertexCount) {
		entry.meshInterface.reset();
		entry.shape.reset();
		entry.triList = triList;
		entry.vertices = vertices;
		entry.triangleCount = triangleCount;
		entry.vertexCount = vertexCount;
		entry.revisionID = data->revisionID;

		btIndexedMesh mesh;
		mesh.m_numTriangles = static_cast<int>(triangleCount);
		mesh.m_triangleIndexBase = reinterpret_cast<const unsigned char*>(triList);
		mesh.m_triangleIndexStride = sizeof(NI::Triangle);
		mesh.m_numVertices = static_cast<int>(vertexCount);
		mesh.m_vertexBase = reinterpret_cast<const unsigned char*>(vertices);
		mesh.m_vertexStride = sizeof(NI::Point3);

		try {
			auto meshInterface = std::make_unique<btTriangleIndexVertexArray>();
			meshInterface->addIndexedMesh(mesh, PHY_SHORT);
			entry.shape = std::make_unique<btBvhTriangleMeshShape>(meshInterface.get(), true);
			entry.meshInterface = std::move(meshInterface);
		}
		catch (...) {
			entry.meshInterface.reset();
			entry.shape.reset();
		}
	}

	static CacheEntry* getUsableEntry(const NI::TriBasedGeometryData* data, const NI::Triangle* triList, unsigned int triangleCount, const NI::Point3* vertices, unsigned int vertexCount) {
		if (triangleCount < minimumTriangleCount) {
			return nullptr;
		}

		if (cache.size() >= maximumCacheEntries) {
			cache.clear();
		}

		auto& entry = cache[data];
		if (!entry.matches(data, triList, triangleCount, vertices, vertexCount)) {
			buildEntry(entry, data, triList, triangleCount, vertices, vertexCount);
		}
		return entry.isUsable() ? &entry : nullptr;
	}

	const std::vector<unsigned int>* getCandidateTriangles(const NI::TriBasedGeometryData* data, const NI::Triangle* triList, unsigned int triangleCount, const NI::Point3* vertices, unsigned int vertexCount, const NI::Point3& modelOrigin, const NI::Point3& modelDirection) {
		const auto entry = getUsableEntry(data, triList, triangleCount, vertices, vertexCount);
		if (!entry) {
			return nullptr;
		}

		// Bullet wants a finite segment. Every vertex lies inside the model bound
		// sphere, so any hittable triangle is hit within [origin, sphere exit].
		const auto& bound = data->bounds;
		const auto dd = modelDirection.dotProduct(&modelDirection);
		if (dd <= 0.0f) {
			return nullptr;
		}
		const auto originToCenter = bound.center - modelOrigin;
		const auto b = modelDirection.dotProduct(&originToCenter);
		const auto radius = bound.radius * 1.01f + 1.0f;
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

		const btVector3 rayFrom(modelOrigin.x, modelOrigin.y, modelOrigin.z);
		const auto rayEnd = modelOrigin + modelDirection * exitDistance;
		const btVector3 rayTo(rayEnd.x, rayEnd.y, rayEnd.z);

		CollectTriangleIndices collector;
		entry->shape->performRaycast(&collector, rayFrom, rayTo);

		// Ascending order keeps results identical to the exhaustive loop.
		std::sort(candidateScratch.begin(), candidateScratch.end());

		return &candidateScratch;
	}

	const std::vector<unsigned int>* getCandidateTrianglesInBox(const NI::TriBasedGeometryData* data, const NI::Triangle* triList, unsigned int triangleCount, const NI::Point3* vertices, unsigned int vertexCount, const NI::Point3& modelAabbMin, const NI::Point3& modelAabbMax) {
		const auto entry = getUsableEntry(data, triList, triangleCount, vertices, vertexCount);
		if (!entry) {
			return nullptr;
		}

		const btVector3 aabbMin(modelAabbMin.x, modelAabbMin.y, modelAabbMin.z);
		const btVector3 aabbMax(modelAabbMax.x, modelAabbMax.y, modelAabbMax.z);

		candidateScratch.clear();
		CollectTriangleIndices collector;
		entry->shape->processAllTriangles(&collector, aabbMin, aabbMax);

		// Ascending order keeps results identical to the exhaustive loop.
		std::sort(candidateScratch.begin(), candidateScratch.end());

		return &candidateScratch;
	}

	void clearCache() {
		cache.clear();
	}
}
