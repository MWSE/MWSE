#pragma once

#include "NIDefines.h"
#include "NITArray.h"
#include "NIColor.h"
#include "NIPointer.h"

#include "TES3Defines.h"
#include "TES3Vectors.h"

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
		PickRecord * lastAddedRecord;
		bool returnTexture;
		bool returnNormal;
		bool returnSmoothNormal;
		bool returnColor;

		//
		// Other related this-call functions.
		//

		static Pick* malloc();
		void free();

		PickRecord* addRecord();

		bool pickObjects(const TES3::Vector3 * origin, const TES3::Vector3 * direction, bool append = false, float maxDistance = 0.0f);
		void clearResults();

	};
	static_assert(sizeof(Pick) == 0x38, "NI::Pick failed size validation");

	struct PickRecord {
		Pointer<Geometry> object;
		Pointer<AVObject> proxyParent;
		TES3::Vector3 intersection;
		float distance;
		unsigned short triangleIndex;
		unsigned short vertexIndex[3];
		TES3::Vector2 texture;
		TES3::Vector3 normal;
		PackedColor color;

		static void* operator new(size_t size);
		static void operator delete(void* block);

		//
		// Custom functions.
		//
		
		std::reference_wrapper<unsigned short[3]> getVertexIndex();

		TES3::Reference* getTES3Reference();

	};
	static_assert(sizeof(PickRecord) == 0x38, "NI::PickRecord failed size validation");
}
