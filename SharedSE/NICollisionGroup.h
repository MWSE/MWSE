#pragma once

#include "NIAVObject.h"
#include "NITArray.h"
#include "NIPoint3.h"

namespace NI {
	// NiCollisionGroup::Intersect, hoisted so AVObject can name it.
	struct CollisionIntersect {
		NI::AVObject* sgCollisionRoot0;
		NI::AVObject* sgCollisionRoot1;
		NI::AVObject* sgObject0;
		NI::AVObject* sgObject1;
		float fTime;
		Point3 point;
		Point3 normal0;
		Point3 normal1;
	};
	static_assert(sizeof(CollisionIntersect) == 0x38, "NI::CollisionIntersect failed size validation");

	struct CollisionGroup {
		struct Record {
			NI::AVObject* sgCollisionRoot;
			NI::AVObject* sgObject;
			int maxDepth;
			int binSize;
		};
		static_assert(sizeof(CollisionGroup::Record) == 0x10, "NI::CollisionGroup::Record failed size validation");

		using Intersect = CollisionIntersect;

		NI::TArray<Record*> colliders;
		NI::TArray<Record*> collidees;

		bool containsCollider(AVObject* obj);
		void addCollider(AVObject* obj);
		void removeCollider(AVObject* obj);
		void removeCollidee(AVObject* obj);
		void removeAll();

		// Reimplemented: colliders with a root-only volume skip the subtree walk.
		void updateWorldData();

		//
		// Custom functions.
		//

		// True when the collider carries no collision volume below its root.
		static bool isRootOnlyCollider(const AVObject* collider);
	};
	static_assert(sizeof(CollisionGroup) == 0x30, "NI::CollisionGroup failed size validation");
}
