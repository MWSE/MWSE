#pragma once

#include "NIDefines.h"
#include "NIPointer.h"
#include "NITArray.h"
#include "NINode.h"

#include "NIColor.h"
#include "NIPoint2.h"
#include "NIPoint3.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "TES3Defines.h"
#endif

namespace NI {
	enum class PickType {
		FIND_ALL,
		FIND_FIRST
	};

	enum class PickSortType {
		SORT,
		NO_SORT
	};

	enum class PickIntersectType {
		BOUND_INTERSECT,
		TRIANGLE_INTERSECT,
		UNKNOWN_2,
	};

	enum class PickCoordinateType {
		MODEL_COORDINATES,
		WORLD_COORDINATES
	};

	struct Pick {
		PickType pickType;
		PickSortType sortType;
		PickIntersectType intersectType;
		PickCoordinateType coordinateType;
		bool frontOnly;
		bool observeAppCullFlag;
		bool unknown_0x12;
		Pointer<Node> root;
		TArray<PickRecord*> results;
		PickRecord* lastAddedRecord;
		bool returnTexture;
		bool returnNormal;
		bool returnSmoothNormal;
		bool returnColor;

		//
		// Other related this-call functions.
		//

		// MWSE-style engine-allocated factory (uses se::memory::malloc + engine ctor).
		// MWSE-only: requires SE_MEMORY_FNADDR_MALLOC/FREE which CSSE doesn't provide.
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		static Pick* malloc();
		void free();
#endif

		PickRecord* addRecord();

		// Engine ctor/dtor via per-target SE_NI_PICK_FNADDR_CTOR/DTOR macros.
		// Body throws not_implemented_exception when address is 0x0.
		Pick();
		~Pick();

		bool pickObjects(const Point3* origin, const Point3* direction, bool append = false, float maxDistance = 0.0f);
		bool pickObjectsWithSkinDeforms(const Point3* origin, const Point3* direction, bool append = false, float maxDistance = 0.0f);
		void clearResults();

		PickRecord* getFirstUnskinnedResult() const;

	};
	static_assert(sizeof(Pick) == 0x38, "NI::Pick failed size validation");

	struct PickRecord {
		Pointer<Geometry> object;
		Pointer<AVObject> proxyParent;
		Point3 intersection;
		float distance;
		unsigned short triangleIndex;
		unsigned short vertexIndex[3];
		Point2 texture;
		Point3 normal;
		PackedColor color;

		static void* operator new(size_t size);
		static void operator delete(void* block);

		//
		// Custom functions.
		//

		std::reference_wrapper<unsigned short[3]> getVertexIndex();

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		TES3::Reference* getTES3Reference();
#endif

	};
	static_assert(sizeof(PickRecord) == 0x38, "NI::PickRecord failed size validation");
}
