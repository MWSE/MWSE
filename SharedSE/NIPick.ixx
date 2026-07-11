module;

#include "NIDefines.h"
#include "NIPointer.h"
#include "NITArray.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "TES3Defines.h"
#endif

export module NIPick;

import NIColor;
import NINode;
import NIPoint2;
import NIPoint3;

namespace NI {
	export enum class PickType {
		FIND_ALL,
		FIND_FIRST
	};

	export enum class PickSortType {
		SORT,
		NO_SORT
	};

	export enum class PickIntersectType {
		BOUND_INTERSECT,
		TRIANGLE_INTERSECT,
		UNKNOWN_2,
	};

	export enum class PickCoordinateType {
		MODEL_COORDINATES,
		WORLD_COORDINATES
	};

	export struct Pick {
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

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		static Pick* malloc();
		void free();
#endif

		PickRecord* addRecord();

		Pick();
		~Pick();

		bool pickObjects(const Point3* origin, const Point3* direction, bool append = false, float maxDistance = 0.0f);
		bool pickObjectsWithSkinDeforms(const Point3* origin, const Point3* direction, bool append = false, float maxDistance = 0.0f);
		void clearResults();

		PickRecord* getFirstUnskinnedResult() const;

	};
	static_assert(sizeof(Pick) == 0x38, "NI::Pick failed size validation");

	export struct PickRecord {
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
