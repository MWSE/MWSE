#include "NITriBasedGeometryData.h"

#include "NIBoundingBox.h"
#include "NIPoint3.h"
#include "NITriangle.h"
#include "NITriangleBVH.h"

#include "ExceptionUtil.h"

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

	//
	// Candidate triangle enumeration, backed by a cached per-mesh BVH.
	//

	// Below this size the exhaustive loop is cheaper than the BVH.
	constexpr auto minimumTriangleCount = 64u;

	struct CandidateCacheEntry {
		TriangleBVH bvh;

		// Entries die with their object (dtor), so this only has to catch a live
		// object whose arrays were reallocated or edited. In-place edits bump
		// revisionID (NiGeomMorpherController::OnPreDisplay, markAsChanged).
		const Triangle* triList = nullptr;
		const Point3* vertices = nullptr;
		unsigned int triangleCount = 0;
		unsigned int vertexCount = 0;
		unsigned short revisionID = 0;

		// Failed builds are not retried.
		bool usable = false;

		bool matches(const TriBasedGeometryData* data, const Triangle* triList_, unsigned int triangleCount_) const {
			return triList == triList_
				&& vertices == data->vertex
				&& triangleCount == triangleCount_
				&& vertexCount == data->vertexCount
				&& revisionID == data->revisionID;
		}
	};

	static std::unordered_map<const TriBasedGeometryData*, CandidateCacheEntry> candidateCache;
	static size_t candidateBuildCount = 0;
	static size_t candidateEvictionCount = 0;

	// One tree per mesh over the full triangle list; ray queries filter to the active count afterwards.
	static CandidateCacheEntry* getUsableCandidateCacheEntry(const TriBasedGeometryData* data) {
		const auto triList = data->getTriList();
		const auto triangleCount = static_cast<unsigned int>(data->triangleCount);
		if (!triList || !data->vertex || triangleCount < minimumTriangleCount) {
			return nullptr;
		}

		auto& entry = candidateCache[data];
		if (!entry.matches(data, triList, triangleCount)) {
			entry.usable = false;
			entry.triList = triList;
			entry.vertices = data->vertex;
			entry.triangleCount = triangleCount;
			entry.vertexCount = data->vertexCount;
			entry.revisionID = data->revisionID;
			++candidateBuildCount;

			try {
				entry.bvh.build(triList, triangleCount, data->vertex);
				entry.usable = true;
			}
			catch (const std::bad_alloc&) {
				entry.bvh = {};
			}
		}
		return entry.usable ? &entry : nullptr;
	}

	void TriBasedGeometryData::dtor() {
#if defined(SE_NI_TRIBASEDGEOMETRYDATA_VTBL) && SE_NI_TRIBASEDGEOMETRYDATA_VTBL > 0 && defined(SE_NI_GEOMETRYDATA_FNADDR_DTOR) && SE_NI_GEOMETRYDATA_FNADDR_DTOR > 0
		if (candidateCache.erase(this) > 0) {
			++candidateEvictionCount;
		}

		// Mirrors NiTriBasedGeomData::~NiTriBasedGeomData.
		vTable.asObject = reinterpret_cast<Object_vTable*>(SE_NI_TRIBASEDGEOMETRYDATA_VTBL);
		GeometryData::dtor();
#else
		throw not_implemented_exception();
#endif
	}

	bool TriBasedGeometryData::getRayCandidateTriangles(const Point3& modelOrigin, const Point3& modelDirection, std::vector<unsigned int>& outCandidates) const {
		const auto entry = getUsableCandidateCacheEntry(this);
		if (!entry) {
			return false;
		}

		outCandidates.clear();
		entry->bvh.collectRayCandidates(modelOrigin, modelDirection, outCandidates);

		// Raytests only consider the active triangles.
		const auto activeTriangleCount = getActiveTriangleCount();
		std::erase_if(outCandidates, [=](unsigned int index) { return index >= activeTriangleCount; });

		// Ascending order keeps results identical to the exhaustive loop.
		std::ranges::sort(outCandidates);
		return true;
	}

	bool TriBasedGeometryData::getAabbCandidateTriangles(const BoundingBox& modelBounds, std::vector<unsigned int>& outCandidates) const {
		const auto entry = getUsableCandidateCacheEntry(this);
		if (!entry) {
			return false;
		}

		outCandidates.clear();
		entry->bvh.collectBoxCandidates(modelBounds, outCandidates);

		// Ascending order keeps results identical to the exhaustive loop.
		std::ranges::sort(outCandidates);
		return true;
	}

	TriBasedGeometryData::CandidateCacheStats TriBasedGeometryData::getCandidateCacheStats() {
		CandidateCacheStats stats;
		stats.entries = candidateCache.size();
		for (const auto& [data, entry] : candidateCache) {
			stats.bytes += sizeof(CandidateCacheEntry) + entry.bvh.getMemoryUsage();
		}
		stats.builds = candidateBuildCount;
		stats.evictions = candidateEvictionCount;
		return stats;
	}
}
